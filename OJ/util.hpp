#include <vector>
#include <stdint.h>
#include <sys/time.h>
#include <unistd.h>
#include <string>
#include <stdio.h>
#include <boost/algorithm/string.hpp>
#include <iostream>
#include <assert.h>
#include <fstream>
#include <map>
class TimeUtil
{
    //获取当前的时间戳
    public:
        //获取秒
    static int64_t TimeStamp()
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return tv.tv_sec;
    }
    //获取毫秒
    static int64_t TimeStampMs()
    {
        struct timeval tv;
        gettimeofday(&tv,NULL);
        return tv.tv_sec*1000+tv.tv_usec/1000;
    } 
};

//打印日志的工具
//格式为I+时间戳+文件名+行号+输入的内容
//日志的使用方法为LOG(INFO)<<"hello"<<"\n";
//日志的级别：FATAL 致命 ERROR 错误 WARNING 警告 INFO 提示
enum Lever
{
    INFO,
    FATAL,
    ERROR,
    WARNING
};

inline std::ostream& Log(Lever lever,const std::string& file_name,int line_num)
{
    std:: string prefix="[";
    if(lever==INFO)
    {
        prefix+="I";
    }else if(lever==FATAL)
    {
        prefix+="F";
    }else if(lever==ERROR)
    {
        prefix+="E";
    }else 
    {
        prefix+="W";
    }
    prefix+=std::to_string(TimeUtil::TimeStamp());
    prefix+=" ";
    prefix+=file_name;
    prefix+=":";
    prefix=std::to_string(line_num);
    prefix+="] ";
    std::cout<<prefix;
    return std::cout;
}

#define LOG(lever) Log(lever,__FILE__,__LINE__)

class FileUtil
{
    public:
    //给定一个文件路径，将文件中的内容存到content中
    static bool  Read(const std:: string& File_Path,std::string * content)
    {
        content->clear();
        std::ifstream file(File_Path.c_str());
        if(!file.is_open())
        {
            return false;
        }
        std:: string line;
        while(std::getline(file,line))
        {
            *content+=line+"\n";
        }
        file.close();
        return true;
    }
    static bool  Write(const std:: string& File_Path,const std::string & content)
    {
        std::ofstream file(File_Path.c_str());
        if(!file.is_open())
        {
            return false;
        }
        file.write(content.c_str(),content.size());
        file.close();
    return true;
    }
};
class Urlutil
{
    public:
    static unsigned char ToHex(unsigned char x)   
    {   
        return  x > 9 ? x + 55 : x + 48;   
    }  
      
   static unsigned char FromHex(unsigned char x)   
    {   
        unsigned char y;  
        if (x >= 'A' && x <= 'Z') y = x - 'A' + 10;  
        else if (x >= 'a' && x <= 'z') y = x - 'a' + 10;  
        else if (x >= '0' && x <= '9') y = x - '0';  
        else assert(0);  
        return y;  
    }  
      
 static   std::string UrlEncode(const std::string& str)  
    {  
        std::string strTemp = "";  
        size_t length = str.length();  
        for (size_t i = 0; i < length; i++)  
        {  
            if (isalnum((unsigned char)str[i]) ||   
                (str[i] == '-') ||  
                (str[i] == '_') ||   
                (str[i] == '.') ||   
                (str[i] == '~'))  
                strTemp += str[i];  
            else if (str[i] == ' ')  
                strTemp += "+";  
            else  
            {  
                strTemp += '%';  
                strTemp += ToHex((unsigned char)str[i] >> 4);  
                strTemp += ToHex((unsigned char)str[i] % 16);  
            }  
        }  
        return strTemp;  
    }  
      
static   std::string UrlDecode(const std::string& str)  
    {  
        std::string strTemp = "";  
        size_t length = str.length();  
        for (size_t i = 0; i < length; i++)  
        {  
            if (str[i] == '+') strTemp += ' ';  
            else if (str[i] == '%')  
            {  
                assert(i + 2 < length);  
                unsigned char high = FromHex((unsigned char)str[++i]);  
                unsigned char low = FromHex((unsigned char)str[++i]);  
                strTemp += high*16 + low;  
            }  
            else strTemp += str[i];  
        }  
        return strTemp;  
    }
//分割的字符串是input,分割字符为split_char,分割之后放入output中
 static void _Split (std::string input,std::string split_char,std::vector<std::string> *output)
 {
     boost::split(*output,input,boost::is_any_of(split_char),boost::token_compress_off);
 }

static void my_prase(std::string body,std::map<std::string,std::string>* kv_body)
{
    //先按照&进行分割
    std::vector<std::string> kvs;
    _Split(body,"&",&kvs);
    //此时body已经被分割为多个键值对
    
    for(size_t i=0;i<kvs.size();++i)
    {
//再进行=的分割
        std::vector<std::string> kv;
        _Split(kvs[i],"=",&kv);
       (* kv_body)[kv[0]]=UrlDecode(kv[1]);
        std::cout<<kv[1]<<std::endl;
    }
    //再按照=进行分割并存入kv_body中
}
};





