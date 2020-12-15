#include <cstdio>
#include <iostream>
#include <cstdlib>
#include <string>
#include <mysql/mysql.h>
#include <jsoncpp/json/json.h>
#include <mutex> 
#define MYSQL_HOST  "127.0.0.1"
#define MYSQL_DB    "blog_system"
#define MYSQL_USER  "root"
#define MYSQL_PASS  "123456789ma"

namespace blog_system
{
    static std::mutex _mutex;
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
    class TableTag
    {
        public:
          TableTag(MYSQL *mysql):_mysql(mysql){}
          ~TableTag(){}
	bool Insert(const Json::Value &tag)
  {
     #define INSERT_TAG "insert table_tag values(null,'%s');"
    char tmp[4096]={0};
    //Json::Value::asCstring 返回c风格的字符串
    //sprintf 按照指定格式将多个数据对象组织成为一个字符串放到tmp中
    sprintf(tmp,INSERT_TAG, tag["name"].asCString());
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
    return true;
  }
	bool Delete(int tag_id)
  {
#define DELETE_TAG "delete from table_tag where id=%d;"
     char tmp[4096]={0};
     sprintf(tmp,DELETE_TAG,tag_id);
     bool ret=MysqlQuery(_mysql,tmp);
     if(ret==false)
       return false;
     return true;
  }
	bool Update(int tag_id,const Json::Value &tag)
  {
#define UPDATE_TAG "update table_tag set name='%s' where id=%d;"
    char tmp[4096]={0};
    sprintf(tmp,UPDATE_TAG,tag["name"].asCString(),tag_id);
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
    return true;
  //
  }
	bool GetAll(Json::Value *tags)
  {
#define SELECT_TAG_ALL "select *from table_tag;"
      //实例化lock_guard对象，并且直接对_mutex进行枷锁
      //lock_guard对象时一个局部对象，函数退出时析构函数自动进行_mutex解锁操作
     std::lock_guard<std::mutex> lck(_mutex);
      bool ret=MysqlQuery(_mysql,SELECT_TAG_ALL);
    if(ret==false)
      return false;
    //获取结果集就相当于在一定条件下获取查询的所有结果
     MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集
     //获取结果集中查询结果的条数
     //获取条数就相当于获取查询结果的行数

     int num_row=mysql_num_rows(res);
     if(num_row<=0)
     {
        printf("No tag infomation\n");
        //释放结果集
        mysql_free_result(res);
        return false;
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
         Json::Value tag;
         tag["id"]=std::stoi(row[0]);
         tag["name"]=row[1];
         tags->append(tag);//将Json::Value对象--》append,添加数组的各个元素
     }
     mysql_free_result(res);//释放结果集
    return true;
  }
	bool GetOne(int tag_id,Json::Value *tag)
  {
#define SELECT_TAG_ONE "select name from table_tag where id=%d;"
     char tmp[1024]={0};
     std::lock_guard<std::mutex> lck(_mutex);
     sprintf(tmp,SELECT_TAG_ONE,tag_id);
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
     MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集
     //获取结果集中查询结果的条数
     int num_row=mysql_num_rows(res);
     if(num_row!=1)
     {
        printf("tag count error\n");
        //释放结果集
        mysql_free_result(res);
        return false;
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
        
        (*tag)["id"]=tag_id;
         (*tag)["name"]=row[0];

     }
     mysql_free_result(res);
    return true;   
  }
    private:
          MYSQL *_mysql;
    };
    class TableUser
    {
        public:
          TableUser(MYSQL*mysql):_mysql(mysql){}
          ~TableUser(){}
	bool Insert(const Json::Value &user)
  {
     #define INSERT_USER "insert table_user values(null,'%s');"
    char tmp[4096]={0};
    sprintf(tmp,INSERT_USER, user["name"].asCString());
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
    return true;
  }
	bool Delete(int user_id)
  {
#define DELETE_USER "delete from table_user where id=%d;"
     char tmp[4096]={0};
     sprintf(tmp,DELETE_USER,user_id);
     bool ret=MysqlQuery(_mysql,tmp);
     if(ret==false)
       return false;
     return true;
  }
	bool Update(int user_id,const Json::Value &user)
  {
#define UPDATE_USER "update table_user set name='%s' where id=%d;"
    char tmp[4096]={0};
    sprintf(tmp,UPDATE_USER,user["name"].asCString(),user_id);
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
    return true;
  }
	bool GetAll(Json::Value *users)
  {
#define SELECT_USER_ALL "select *from table_user;"
     std::lock_guard<std::mutex> lck(_mutex);
    bool ret=MysqlQuery(_mysql,SELECT_USER_ALL);
    if(ret==false)
      return false;
     MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集
     //获取结果集中查询结果的条数
     int num_row=mysql_num_rows(res);
     if(num_row<=0)
     {
        printf("No user infomation\n");
        //释放结果集
        mysql_free_result(res);
        return false;
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
         Json::Value user;
         user["id"]=std::stoi(row[0]);
         user["name"]=row[1];
         users->append(user);//将Json::Value对象--》append,添加数组的各个元素
     }
     mysql_free_result(res);//释放结果集
    return true;
  }
	bool GetOne(int user_id,Json::Value *user)
  {
#define SELECT_USER_ONE "select name from table_user where id=%d;"
     char tmp[1024]={0};
     sprintf(tmp,SELECT_USER_ONE,user_id);
     std::lock_guard<std::mutex> lck(_mutex);
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
     MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集
     //获取结果集中查询结果的条数
     int num_row=mysql_num_rows(res);
     if(num_row!=1)
     {
        printf("user count error\n");
        //释放结果集
        mysql_free_result(res);
        return false;
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
        
        (*user)["id"]=user_id;
         (*user)["name"]=row[0];

     }
     mysql_free_result(res);
    return true;   
  }
    private:
          MYSQL *_mysql;
      
    };
    class TableBlog
    {
        public:
          TableBlog(MYSQL *mysql):_mysql(mysql){

          }
          ~TableBlog(){

          }
	bool Insert(const Json::Value &blog)
  {
     #define INSERT_BLOG "insert table_blog values(null,%d,%d,'%s','%s',now());"
    char tmp[4096]={0};
    sprintf(tmp,INSERT_BLOG,blog["tag_id"].asInt(),blog["user_id"].asInt(), blog["title"].asCString(),blog["content"].asCString());
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
    return true;
  }
	bool Delete(int blog_id)
  {
#define DELETE_BLOG "delete from table_blog where id=%d;"
     char tmp[4096]={0};
     sprintf(tmp,DELETE_BLOG,blog_id);
     bool ret=MysqlQuery(_mysql,tmp);
     if(ret==false)
       return false;
     return true;
  }
	bool Update(int blog_id,const Json::Value &blog)
  {
#define UPDATE_BLOG "update table_blog set tag_id=%d,title='%s',content='%s' where id=%d;"
    char tmp[4096]={0};
    sprintf(tmp,UPDATE_BLOG,blog["tag_id"].asInt(),blog["title"].asCString(),blog["content"].asCString(),blog_id);
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
    return true;
  }
	bool GetAll(Json::Value *blogs)
  {
#define SELECT_BLOG_ALL "select *from table_blog;"
     std::lock_guard<std::mutex> lck(_mutex);
    bool ret=MysqlQuery(_mysql,SELECT_BLOG_ALL);
    if(ret==false)
      return false;
     MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集
     //获取结果集中查询结果的条数
     int num_row=mysql_num_rows(res);
     if(num_row<=0)
     {
        printf("No blog infomation\n");
        //释放结果集
        mysql_free_result(res);
        return false;
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
         Json::Value blog;
         blog["id"]=std::stoi(row[0]);
         blog["tag_id"]=std::atoi(row[1]);
         blog["user_id"]=std::stoi(row[2]);
         blog["title"]=row[3];
         blog["content"]=row[4];
         blog["ctime"]=row[5];
         blogs->append(blog);//将Json::Value对象--》append,添加数组的各个元素
     }
     mysql_free_result(res);//释放结果集
    return true;
  }
	bool GetTag(int tag_id,Json::Value *blogs)
  {
#define SELECT_BLOG_TAG "select * from table_blog where tag_id=%d;"
     char tmp[1024]={0}; 
     sprintf(tmp,SELECT_BLOG_TAG,tag_id);
     std::lock_guard<std::mutex> lck(_mutex);
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
     MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集
     //获取结果集中查询结果的条数
     int num_row=mysql_num_rows(res);
     if(num_row<=0)
     {
        printf("blog result info error\n");
        //释放结果集
        mysql_free_result(res);
        return false;
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
         Json::Value blog;
         blog["id"]=std::stoi(row[0]);
         blog["tag_id"]=std::atoi(row[1]);
         blog["user_id"]=std::stoi(row[2]);
         blog["title"]=row[3];
         blog["content"]=row[4];
         blog["ctime"]=row[5];
         blogs->append(blog);
     }
     mysql_free_result(res);
    return true;   
  }
	bool GetUser(int user_id,Json::Value *blogs)
  {
#define SELECT_BLOG_USER "select * from table_blog where user_id=%d;"
     char tmp[1024]={0}; 
     sprintf(tmp,SELECT_BLOG_USER,user_id);
     std::lock_guard<std::mutex> lck(_mutex);
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
     MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集
     //获取结果集中查询结果的条数
     int num_row=mysql_num_rows(res);
     if(num_row<=0)
     {
        printf("blog result info error\n");
        //释放结果集
        mysql_free_result(res);
        return false;
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
         Json::Value blog;
         blog["id"]=std::stoi(row[0]);
         blog["tag_id"]=std::atoi(row[1]);
         blog["user_id"]=std::stoi(row[2]);
         blog["title"]=row[3];
         blog["content"]=row[4];
         blog["ctime"]=row[5];
         blogs->append(blog);
     }
     mysql_free_result(res);
    return true;   
  } 
	bool GetOne(int blog_id,Json::Value *blog)
  {
#define SELECT_BLOG_ONE "select * from table_blog where id=%d;"
     char tmp[1024]={0};
     sprintf(tmp,SELECT_BLOG_ONE,blog_id);
     std::lock_guard<std::mutex> lck(_mutex);
    bool ret=MysqlQuery(_mysql,tmp);
    if(ret==false)
      return false;
     MYSQL_RES *res=mysql_store_result(_mysql);//获取结果集
     //获取结果集中查询结果的条数
     int num_row=mysql_num_rows(res);
     if(num_row!=1)
     {
        printf("user count error\n");
        //释放结果集
        mysql_free_result(res);
        return false;
     }
     for(int i=0;i<num_row;i++)
     {
         MYSQL_ROW row=mysql_fetch_row(res);//遍历结果集，获取每一行数据
         (*blog)["id"]=std::atoi(row[0]);
         (*blog)["tag_id"]=std::atoi(row[1]);
         (*blog)["user_id"]=std::atoi(row[2]);
         (*blog)["title"]=row[3];
         (*blog)["content"]=row[4];
         (*blog)["ctime"]=row[5];
     }
     mysql_free_result(res);
    return true;   
  }

        private:
          MYSQL *_mysql;
    };

}
