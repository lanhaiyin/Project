#include <cstdio>
#include <cstdlib>
#include <string>
#include <iostream>
#include <vector>
#include <map>
#include <unistd.h>
#include <jsoncpp/json/json.h>
#include <mysql/mysql.h>
#define MYSQL_HOST  "127.0.0.1"
#define MYSQL_DB    "oj"
#define MYSQL_USER  "root"
#define MYSQL_PASS  "123456789ma"
using namespace std;
class my_mysql
{
//对数据库进行的操作
public:
static MYSQL *MysqlInit()
  {
      MYSQL *mysql=NULL;
      //1.初始化mysql句柄
      mysql=mysql_init(NULL);
      if(mysql==NULL)
      {
          printf("mysql init error\n");
          return NULL;
      }
      //2.连接mysql服务器
      if(mysql_real_connect(mysql,MYSQL_HOST,MYSQL_USER,MYSQL_PASS,MYSQL_DB,0,NULL,0)==NULL)
      {
          printf("connect mysql server failed: %s\n",mysql_error(mysql));
          mysql_close(mysql);
          return NULL;
      }

      //3.选择数据库 mysql_select_db
      //4.设置字符编码集 
      if(mysql_set_character_set(mysql,"utf8")!=0)
      {
          printf("set character failed:%s\n",mysql_error(mysql));
          mysql_close(mysql);
          return NULL;
      }
      return mysql;
  }
    static void MysqlRelease(MYSQL *mysql)
    {
        if(mysql)
        {
           mysql_close(mysql);
        }
        return;
    }
    static bool MysqlQuery(MYSQL*mysql,const std::string &sql)
    {
       int ret=mysql_query(mysql,sql.c_str());
       if(ret!=0)
       {
         printf("query sql:[%s] error :%s\n",sql.c_str(),mysql_error(mysql));
       }
       return true;
    }

};
class Decribe
{
public:
    Decribe(MYSQL *mysql)
    {
        _mysql=mysql;
    }
    bool get_allquestion(Json::Value* allquestions)
    {
    #define SELECT_allquestin "select *from table_allquestion;"
    bool ret=my_mysql::MysqlQuery(_mysql,SELECT_allquestin);
    if(ret==false)
    {
        return false;
    }
    MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集

     //获取结果集中查询结果的条数
     int num_row=mysql_num_rows(res);
     if(num_row<=0)
     {
        printf("No user infomation\n");
        //释放结果集
        mysql_free_result(res);
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据         
         Json::Value allquestion;
         allquestion["id"]=std::stoi(row[0]);
         allquestion["name"]=row[1];
         allquestion["star"]=row[2];
     allquestions->append(allquestion);//将Json::Value对象--》append,添加数组的各个元素
     }
          mysql_free_result(res);//释放结果集
         return true; 
    }
    bool get_question(int id,Json::Value *question)
    {
    #define SELECT_question "select *from table_question where id=%d;"
    char temp[1024]={0};
    sprintf(temp,SELECT_question,id);
    bool ret=my_mysql::MysqlQuery(_mysql,temp);
    if(ret==false)
    {
        return false;
    }
    MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集

     //获取结果集中查询结果的条数
   /*  int num_row=mysql_num_rows(res);
     if(num_row<=0)
     {
        printf("No user infomation\n");
        //释放结果集
        mysql_free_result(res);
     }
    // for(int i=0;i<num_row;i++)
    */ {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据         
         (*question)["id"]=std::stoi(row[0]);
         (*question)["name"]=row[1];
         (*question)["star"]=row[2];
         (*question)["des"]=row[3];;
    }mysql_free_result(res);//释放结果集
 return true; 
    }
    bool get_Test(int id,Json::Value *Test)
    {
    #define SELECT_Test "select *from table_test where id=%d;"
    char temp[1024]={0};
    sprintf(temp,SELECT_Test,id);
        bool ret=my_mysql::MysqlQuery(_mysql,temp);
    if(ret==false)
    {
        return false;
    }
    MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集

     //获取结果集中查询结果的条数
     int num_row=mysql_num_rows(res);
     if(num_row<=0)
     {
        printf("No user infomation\n");
        //释放结果集
        mysql_free_result(res);
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据         
         Json::Value test;
         test["input"]=row[1];
        test["error"]="";
         test["output"]=row[2];
        Test->append(test);//将Json::Value对象--》append,添加数组的各个元素
     }
          mysql_free_result(res);//释放结果集
return true;

    }
    MYSQL *_mysql;

};
