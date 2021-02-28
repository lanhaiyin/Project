#include "OJ_view.hpp"
#include "db.hpp"
#include "httplib.h"
#include "compile.hpp"
using namespace httplib;

int main()
{
   Server server; 
   server.Get("/allquestion",[](const Request& req,Response &resp)
    {
        //1.获取题目列表
		MYSQL* mysql=my_mysql::MysqlInit();
     Decribe de(mysql);
     Json::Value ans;
     de.get_allquestion(&ans);

     //2.讲Json格式转换成html格式
Json::StyledWriter writer;//有格式的序列化   
     std:: cout<<"dai"<<writer.write(ans)<<"ma"<<std::endl;                                                                                                               my_mysql::MysqlRelease(mysql);   
     std::string html;
    OjView::RenderAllquestion(ans,&html);
    resp.set_content(html,"text/html");	
    //my_mysql::MysqlRelease(mysql); 
    });
    server.Get(R"(/question/(\d+))",[](const Request& req,Response &resp)
    {
    //获取id
    int id=stoi(req.matches[1]);
    MYSQL * mysql=my_mysql::MysqlInit();
    Decribe de(mysql);
    Json::Value ans;
    de.get_question(id,&ans);
    std::string html;
    OjView::Renderquestion(ans,&html);
   resp.set_content(html,"text/html");
    
    
    });
    server.Post(R"(/compile/(\d+))",[](const Request& req,Response &resp)
	{
        int id=stoi(req.matches[1]);
    std::cout<<req.body;
		(void) req;    
        Json::Value req_json;    
        Json::Value resp_json;    
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
		//获取测试用例
		MYSQL* mysql=my_mysql::MysqlInit();
         Decribe de(mysql);
        Json::Value Test;
        de.get_Test(id,&Test);
        //对代码进行运行和编译   
        Compile::ComAndRun(req_json,&resp_json,&Test);   
    Json::StyledWriter writer;//有格式的序列化                         
                     std:: cout<<"Test的内容  "<<writer.write(Test)<<std::endl;    
                                 Json::StyledWriter writer2;//有格式的序列化                         
                                                 std:: cout<<"resp的内容"<<writer2.write(resp_json)<<std::endl;     
		//将Json::Value转换成html 
    	std::string html;
       html+="<html>";
                   html+="<body>";
                               html+="<div>";
                                         html+=Test["input"].asString()+Test["output"].asString();
                                                     html+="</div>";
                                                                html+="</body>";
                                                                             html+="</html>";

        // OjView::RenderResult(Test,resp_json,&html); 
        std::cout<<html<<std::endl;
         resp.set_content(html,"text/html");
	});
  //  server.set_base_dir("./www");
    server.listen("0.0.0.0",9092);
}
