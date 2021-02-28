#include "httplib.h"
#include "compile.hpp"
#include "jsoncpp/json/json.h"
#include <iostream>
#include <map>
int main()
{
    using namespace httplib;
    //创建server对象
    Server server;
    //调用get方法
    server.Post("/compile",[](const Request &req,Response &resp)            
     {
        (void) req;
        Json::Value req_json;
        Json::Value resp_json;
        std::cout<<"身体"<<req.body<<std::endl;
        //对body部分进行分割,body中键和键用&分割，键和值用=分割
        std::map<std::string,std::string> kv_body; 
        Urlutil::my_prase(req.body,&kv_body);
        //组织成json格式
        for(auto it:kv_body)
        {
        std::cout<<"分割成功了"<<it.second<<std::endl;
        //Json::value的使用方法与map类似
        req_json[it.first]=it.second;
        }
        Compile::ComAndRun(req_json,&resp_json);
        //把Json::Value序列化一个字符串才能返回
        Json::FastWriter writer;
 resp.set_content(writer.write(resp_json), "text/plain");
     });
    server.set_base_dir("./www");
    server.listen("0.0.0.0",9090);
}
