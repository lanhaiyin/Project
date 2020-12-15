#include "db.hpp"
#include "httplib.h"
#define WWWROOT "./www"
blog_system::TableTag* table_tag;
blog_system::TableBlog* table_blog;
blog_system::TableUser* table_user;
/*void test()
{
  //初始化数据库
  MYSQL *mysql=blog_system::MysqlInit();
  //释放数据库，根据数据库的操作句柄
  //创建table_user表的对象
  blog_system::TableUser table_user(mysql);
  //定义JSON对象
 
  Json::Value user;
  user["name"]="马文杰";
  table_user.Insert(user);
  table_user.GetAll(&user);
  Json::Reader reader; 
  Json::StyledWriter writer;//有格式的序列化
  std:: cout<<writer.write(user)<<std::endl;
  blog_system::MysqlRelease(mysql);
}
*/
void InsertUser(const httplib::Request& req,httplib::Response & rsp)
{
  //
    //用户信息在req的body中，是一个json字符串
    //1.获取json字符串
  std:: cout<<req.method<<std::endl;
 std::cout<<req.body<<std::endl;
 std:: cout<<req.path<<std::endl;
  for(auto it=req.params.begin();it!=req.params.end();++it)
  {
    std:: cout<<it->first<<" = "<<it->second<<std::endl; 
  }
    for(auto it=req.headers.begin();it!=req.headers.end();++it)
  {
    std:: cout<<it->first<<" = "<<it->second<<std::endl; 
  }
  std::string json_str=req.body;
    //2.将json字符串解析成json::Value对象
    Json::Reader reader;
    Json::Value root;
    bool ret=reader.parse(json_str,root);
    if(ret==false)
    {
      printf("insert user info parse json failed\n");
      rsp.status=400;
      return;
    }
    //3.调用table_user中的insert接口向数据库插入数据
    ret=table_user->Insert(root);
    if(ret==false)
    {
       printf("insert user info db failed\n");
       rsp.status=500;
    }
    //4.填充rsp响应信息
    rsp.status=200;
    std::cout<<"插入用户成功"<<std::endl;
    return ; 
}
void DeleteUser(const httplib::Request& req,httplib::Response &rsp)
{
    //获取删除用户id， /user/id  /user/(\d+)
    int user_id =std:: stoi(req.matches[1]);//matches[0]放的是整个字符串，剩下的是捕捉到的字符串
    //执行数据库操作
    bool ret= table_user->Delete(user_id);
    if(ret==false)
    {
      printf("delete user failed\n");
      rsp.status=500;
      return ;
    }
    //填充响应
    std::cout<<"删除用户成功"<<std::endl;
    rsp.status =200;
  return ;
}
void UpdateUser(const httplib::Request& req,httplib::Response &rsp)
{
   int user_id =std::stoi(req.matches[1]);
   std::string json_str=req.body;
   Json::Reader reader;
   Json::Value root;
   bool ret = reader.parse(json_str,root);
   if(ret==false)
   {
      printf("update user info,json format error \n");
      rsp.status=400;
      return;
   }
   ret=table_user->Update (user_id,root);
   if(ret==false)
   {
     printf("update user info,db.operator error\n");
     rsp.status =500;
     return ;
   }
    std::cout<<"更新用户成功"<<std::endl;
   rsp.status=200;
   return;
}
void GetAllUser (const  httplib::Request &req,httplib::Response &rsp)
{
  //从数据库获取到所有的json::value对象
  //将json::value字符串作为响应的正文，填充到rsp中 
  Json::Value root;
  bool ret=table_user->GetAll (&root);
  if(ret==false)
  {
    printf("get all user info,db.operator error");
    rsp.status=500;
    return ;
  }
    //将json::value字符串作为响应的正文，填充到rsp中 
  
   Json::FastWriter writer;
   std::string body;
  body=writer.write(root);
   rsp.set_content(&body[0],body.size(),"application/json");
    std::cout<<"获取所有用户成功"<<std::endl;
   return ;
}
void GetOneUser(const httplib::Request& req,httplib::Response &rsp)
{
   int user_id =std::stoi(req.matches[1]);
   Json::Value root;
   bool ret=table_user->GetOne(user_id,&root);
   if(ret==false)
   {
     printf("get one user info,db operator error\n");
     rsp.status=500;
     return ;
   }
   Json::FastWriter writer;
   rsp.set_content(writer.write(root),"application/json");
    std::cout<<"获取单个用户成功"<<std::endl;
   return ;
}
void InsertTag(const httplib::Request& req,httplib::Response &rsp)
{
    //用户信息在req的body中，是一个json字符串
    //1.获取json字符串
    std::string json_str=req.body;
    //2.将json字符串解析成json::Value对象
    Json::Reader reader;
    Json::Value root;
    bool ret=reader.parse(json_str,root);
    if(ret==false)
    {
      printf("insert tag info parse json failed\n");
      rsp.status=400;
      return;
    }
    //3.调用table_tag中的insert接口向数据库插入数据
    ret=table_tag->Insert(root);
    if(ret==false)
    {
       printf("insert tag info db failed\n");
       rsp.status=500;
        return;
    }
    //4.填充rsp响应信息
    std::cout<<"插入标签成功"<<std::endl;
    rsp.status=200;
    return ; 
}
void DeleteTag(const httplib::Request& req,httplib::Response &rsp)
{
    //获取删除用户id， /tag/id  /tag/(\d+)
    int tag_id =std:: stoi(req.matches[1]);//matches[0]放的是整个字符串，剩下的是捕捉到的字符串
    //执行数据库操作
    bool ret= table_tag->Delete(tag_id);
    if(ret==false)
    {
      printf("delete tag failed\n");
      rsp.status=500;
      return ;
    }
    //填充响应
    std::cout<<"删除标签成功"<<std::endl;
    rsp.status =200;
  return ;
}
void UpdateTag(const httplib::Request& req,httplib::Response &rsp)
{
   int tag_id =std::stoi(req.matches[1]);
   std::string json_str=req.body;
   Json::Reader reader;
   Json::Value root;
   bool ret = reader.parse(json_str,root);
   if(ret==false)
   {
      printf("update tag info,json format error \n");
      rsp.status=400;
      return;
   }
   ret=table_tag->Update (tag_id,root);
   if(ret==false)
   {
     printf("update tag info,db.operator error\n");
     rsp.status =500;
     return ;
   }
    std::cout<<"更新标签成功"<<std::endl;
   rsp.status=200;
   return;
}
void GetAllTag(const httplib::Request& req,httplib::Response &rsp)
{
  Json::Value root;
  bool ret=table_tag->GetAll (&root);
  if(ret==false)
  {
    printf("get all tag info,db.operator error");
    rsp.status=500;
    return ;
  }
   Json::FastWriter writer;
   std::string body;
  body=writer.write(root);
  
   rsp.set_content(&body[0],body.size(),"application/json");
    std::cout<<"更新所有标签成功"<<std::endl;
   return ;
}
void GetOneTag(const httplib::Request& req,httplib::Response &rsp)
{
   int tag_id =std::stoi(req.matches[1]);
   Json::Value root;
   bool ret=table_tag->GetOne(tag_id,&root);
   if(ret==false)
   {
     printf("get one tag info,db operator error\n");
     rsp.status=500;
     return ;
   }
   Json::FastWriter writer;
   rsp.set_content(writer.write(root),"application/json");
    std::cout<<"更新单个标签成功"<<std::endl;
    return;
}
void InsertBlog(const httplib::Request& req,httplib::Response &rsp)
{
    //用户信息在req的body中，是一个json字符串
    //1.获取json字符串
    std::string json_str=req.body;
    //2.将json字符串解析成json::Value对象
    Json::Reader reader;
    Json::Value root;
    bool ret=reader.parse(json_str,root);
    if(ret==false)
    {
      printf("insert blog info parse json failed\n");
      rsp.status=400;
      return;
    }
    //3.调用table_blog中的insert接口向数据库插入数据
    ret=table_blog->Insert(root);
    if(ret==false)
    {
       printf("insert blog info db failed\n");
       rsp.status=500;
        return;
    }
    //4.填充rsp响应信息
    std::cout<<"插入博客成功"<<std::endl;
    rsp.status=200;
    return ; 
}
void DeleteBlog(const httplib::Request& req,httplib::Response &rsp)
{
    //获取删除用户id， /blog/id  /blog/(\d+)
    int blog_id =std:: stoi(req.matches[1]);//matches[0]放的是整个字符串，剩下的是捕捉到的字符串
    //执行数据库操作
    bool ret= table_blog->Delete(blog_id);
    if(ret==false)
    {
      printf("delete blog failed\n");
      rsp.status=500;
      return ;
    }
    //填充响应
    std::cout<<"删除博客成功"<<std::endl;
    rsp.status =200;
  return ;
}
void UpdateBlog(const httplib::Request& req,httplib::Response &rsp)
{
   int blog_id =std::stoi(req.matches[1]);
   std::string json_str=req.body;
   Json::Reader reader;
   Json::Value root;
   bool ret = reader.parse(json_str,root);
   if(ret==false)
   {
      printf("update blog info,json format error \n");
      rsp.status=400;
      return;
   }
   ret=table_blog->Update (blog_id,root);
   if(ret==false)
   {
     printf("update blog info,db.operator error\n");
     rsp.status =500;
     return ;
   }
    std::cout<<"更新博客成功"<<std::endl;
   rsp.status=200;
   return;
}
void GetAllBlog(const httplib::Request& req,httplib::Response &rsp)
{
  Json::Value root;
  if(req.has_param("tag_id"))
  {
    int tag_id=std::stoi(req.get_param_value ("tag_id"));
    bool ret=table_blog->GetTag (tag_id,&root);
  
       if(ret==false)
      {
         printf("get tag indo blog failed\n");
         rsp.status=500;
         return ;
      }
  }
    else if(req.has_param("user_id"))
    {
   
        int user_id=std::stoi(req.get_param_value ("user_id"));
    bool ret=table_blog->GetUser (user_id,&root);
    if(ret==false)
    {
       printf("get user  blog  info failed\n");
       rsp.status=500;
       return ;
    }
    }else{
       bool ret=table_blog->GetAll(&root);
    if(ret==false)
    {
       printf("get user  blog  info failed\n");
       rsp.status=500;
       return; 
    }
    }
   Json::FastWriter writer;
   std::string body;
  body=writer.write(root);
 
   rsp.set_content(&body[0],body.size(),"application/json");
    std::cout<<"获取所有博客成功"<<std::endl;
   return ;
}
void GetOneBlog(const httplib::Request& req,httplib::Response &rsp)
{
   int blog_id =std::stoi(req.matches[1]);
   Json::Value root;
   bool ret=table_blog->GetOne(blog_id,&root);
   if(ret==false)
   {
     printf("get one blog info,db operator error\n");
     rsp.status=500;
     return ;
   }
   Json::FastWriter writer;
   rsp.set_content(writer.write(root),"application/json");
    std::cout<<"获取单个博客成功"<<std::endl;
    return;
}

int main(int argc,char* argv[])
{
  //test();
  MYSQL *mysql=blog_system::MysqlInit();
  if(mysql==NULL)
  {
    return -1;
  }
  table_blog= new blog_system::TableBlog(mysql);
  table_tag = new blog_system:: TableTag(mysql);
  table_user= new blog_system::TableUser(mysql);
  //test1();
 
  httplib::Server server;
  server.set_base_dir(WWWROOT);//设置url中资源路径的相对根目录;
  //R"()"-----括号中的数据是原始数据，去除特殊字符的特殊含义
  //路由注册函数的第一个参数可以是一个正则表达式
  //正则表达式：用于匹配符合某种规则/特定格式的字符串
  //(\d+)到时候就可以捕捉括起来的数据
  server.Post(R"(/user)",InsertUser);
 // user/1 /user/2  /user/11
 //server.post(请求路径，回调函数) 
  server.Delete(R"(/user/(\d+))",DeleteUser);
  server.Put(R"(/user/(\d+))",UpdateUser);
  server.Get(R"(/user)",GetAllUser);
  server.Get(R"(/user/(\d+))",GetOneUser);
 
  server.Post(R"(/tag)",InsertTag);
  server.Delete(R"(/tag/(\d+))",DeleteTag);
  server.Put(R"(/tag/(\d+))",UpdateTag);
  server.Get(R"(/tag)",GetAllTag);
  server.Get(R"(/tag/(\d+))",GetOneTag);

  server.Post("/blog",InsertBlog);
  server.Delete(R"(/blog/(\d+))",DeleteBlog);
  server.Put(R"(/blog/(\d+))",UpdateBlog);
  server.Get(R"(/blog)",GetAllBlog);
  server.Get(R"(/blog/(\d+))",GetOneBlog);

  server.listen("0.0.0.0", 9000);
   return 0;
   }
