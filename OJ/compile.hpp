#include <string>
#include <sys/wait.h>
#include <stdlib.h>
#include <atomic>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include "util.hpp"
#include <jsoncpp/json/json.h>
class Compile
{

    public:
        
        //1.源代码文件
    static std::string SRC_Path(std::string name)
    {
        return "./temp_file/"+name+".cpp";
    }
        //2.编译错误文件
    static std::string CompileError_Path(std::string name)
    {
        return "./temp_file/"+name+".compile_error";
    }
        //3.运行文件
    static std::string EXE_Path(std::string name)
    {
        return "./temp_file/"+name+".exe";
    }
        //4.标准输入文件
    static std::string Stdin_Path(std::string name)
    {
        return "./temp_file/"+name+".stdin";
    }
        //5.标准输出文件
   static std::string Stdout_Path(std::string name)
    {
        return "./temp_file/"+name+".stdout";
    }
        //6.标准错误文件
    static std::string Stderr_Path(std::string name)
    {
        return "./temp_file/"+name+".stderr";
    }
        
        
        //借助Json::Value这个类可以进行序列化和反序列化
   static bool ComAndRun(Json::Value &req,Json::Value *resp,Json::Value *Test)
    {
        //1.将Json：：Value当中的内容进行解析然后保存到文件中
        if(req["code"].empty())
        {
            (*resp)["error"]=3;
            (*resp)["reason"]="code empty";
           LOG(ERROR)<<"code empty"<<std::endl; 
            return false;
        }
       const  std::string& code=req["code"].asString();
        //将代码放到代码文件当中
        std::string file_name=WriteTmpFile(code);
        //2.对文件进行编译  采用fork+程序替换
       bool ret=Compile_(file_name);
            if(!ret)
            {
                //编译错误处理
                (*resp)["error"]=1;
                std::string reason;
             FileUtil::Read(CompileError_Path(file_name),&reason);
             LOG(INFO)<<"Compile error"<<std::endl;
                (*resp)["reason"]=reason;
                return false;
            }
        //如果编译错误需要记录下来，重定向到标准错误文件中
        //3.执行可执行程序，将输入的内容重定向到标准输入文件中，
        //将标准输出与标准错误也重定向到对应的文件中进行记录
        
        //如果用户输入内容，则对输入的内容进行运行并且验证
       if(req["stdin"].asString()!="")
       {
           //将输入的内容存放到stdin的文件内
            FileUtil::Write(Stdin_Path(file_name),req["stdin"].asString());
            //对代码进行运行
            int sig=Run(file_name);
        if(sig!=0)
        {
            //运行错误
            (*resp)["error"]=2;
            (*resp)["reason"]="Program exit by sigbal:"+std::to_string(sig);
            LOG(INFO)<<"Program exit by sigbal:"<<std::to_string(sig) <<std::endl;
           return false; 
        }
       }
        //如果用户没有输入内容则进行测试用例的验证
        
        
        //4.将最终的结果进行组织
        //如果运行通过则对代码进行测试用例的检测
        


         

         My_Test(file_name,Test);
        //如果测试不通过，则对测试的结果进行组织并返回
        //如果测试通过则返回解答正确
        
        (*resp)["error"]=0;
        (*resp)["reason"]="";
        std::string str_stderr;
        FileUtil::Read(Stderr_Path(file_name),&str_stderr);
        (*resp)["stderr"]=str_stderr;
        std::string str_stdout;
        FileUtil::Read(Stdout_Path(file_name),&str_stdout);
        str_stdout.pop_back();
        (*resp)["stdout"]=str_stdout;
            return true;
    }
    private:
    //1.将代码放到文件中
    //2.给每次请求的文件设置一个唯一的名字
    //形如 tmp_时间戳.id
    static std:: string WriteTmpFile(const std::string &code)
    {
        static std::atomic_int id(0);
        ++id;
        std::string name="tmp_"+
            std::to_string(TimeUtil::TimeStamp())+"."+std::to_string(id);
        FileUtil::Write(SRC_Path(name),code);
        return name;
    }
    static bool  Compile_(const std::string file_name)
    {
        //对g++进行组织
        char* argv[20]={0};
        char buf[20][50]={{0}};
        for(int i=0;i<20;++i)
        {
            argv[i]=buf[i];
        }
        sprintf(argv[0],"%s","g++");
        sprintf(argv[1],"%s",SRC_Path(file_name).c_str());
        sprintf(argv[2],"%s","-o");
        sprintf(argv[3],"%s",EXE_Path(file_name).c_str());
        argv[4]=NULL;
        //进程程序替换
        int ret=fork();
            if(ret<0)
            {
                return false;
            }else if(ret==0)
            {
               //子进程逻辑
                int fd=open(CompileError_Path(file_name).c_str(),O_WRONLY|O_CREAT,0666);
                if(fd<0)
                {
                    //文件描述符打开失败，上限
                    LOG(ERROR)<<"open Compile file error"<<std::endl;
                    exit(1);
                }
                dup2(fd,2);//将2号文件描述符的内容重定向到文件中
                execvp(argv[0],argv);
                std::cout<<"myth";
                exit(0);
            }else{
                //父进程
              waitpid(ret,NULL,0);  
            }
        //如果没有生成可执行程序的文件则说明编译错误
        struct stat st;
       ret= stat(EXE_Path(file_name).c_str(),&st);
        if(ret<0)
        {
            //说明stat不存在，编译失败
                LOG(INFO)<<"compile failed"<<file_name<<std::endl;
          std::cout<<"编译失败";
          return false;
        }
        LOG(INFO)<<"compile ok"<<file_name<<std::endl;
        return true;
    }
   static int Run(const std::string file_name)
    {
        //运行+测试
        //测试：获取测试的用例，再根据输入之后的运行结果与输出进行比较如果错误则返回测试用例信息
        //1.创建子进程，父进程进行等待，子进程进行程序替换
        //2.将标准输入，标准输出标准错误重定向到对应的文件中
        int ret=fork();
        if(ret<0)
        {
            exit(0);
        }
        else if(ret==0)
        {
            int fd_stdin=open(Stdin_Path(file_name).c_str(),O_RDONLY);
            dup2(fd_stdin,0);
            int fd_stdout=open(Stdout_Path(file_name).c_str(),O_WRONLY|O_CREAT,0666);
            dup2(fd_stdout,1);
            int fd_stderr=open(Stderr_Path(file_name).c_str(),O_WRONLY|O_CREAT,0666);
            dup2(fd_stderr,2);
            //子进程
            //程序替换
            execl(EXE_Path(file_name).c_str(),EXE_Path(file_name).c_str(),NULL);
            exit(0);
        }else{
            //父进程
            int status;
            waitpid(ret,&status,0);
            return status&0x7f;
        }
    }
     static void My_Test(const std::string file_name,Json::Value* Test)
     {
         //使用循环对代码进行多次执行并检验
             //创建子进程，让子进程进行程序代码的执行，父进程阻塞等待
         for(auto it=Test->begin();it!=Test->end();++it)
         {
           int ret=fork();
           if(ret==0)
           {
             //只是将输入之后运行的结果放入Test["error"]中
            
                std::cout<<"test的输入"<<(*it)["input"].asString()<<std::endl;
            
                FileUtil::Write(Stdin_Path(file_name),(*it)["input"].asString());
                std::cout<<"stdin路径"<<Stdin_Path(file_name)<<std::endl;
                int fd_stdin=open(Stdin_Path(file_name).c_str(),O_RDONLY);
            dup2(fd_stdin,0);
            int fd_stdout=open(Stdout_Path(file_name).c_str(),O_WRONLY|O_CREAT,0666);
            dup2(fd_stdout,1);
            int fd_stderr=open(Stderr_Path(file_name).c_str(),O_WRONLY|O_CREAT,0666);
            dup2(fd_stderr,2);
            //子进程
            //程序替换
          execl(EXE_Path(file_name).c_str(),EXE_Path(file_name).c_str(),NULL);
            exit(0);
            }else{
            //父进程
            int status;
           waitpid(ret,&status,0);
            std::string s;
            FileUtil::Read(Stdout_Path(file_name),&s);
            std:: cout<<s<<std::endl;
                s.pop_back();
            (*it)["error"]=s;
        
            }
            //对Test进行组织，将stdout里面对应的文件内容放入Test["error"]中
           //先读出来
     } 

            
}};
