#include <jsoncpp/json/json.h>
#include "compile.hpp"
#include "util.hpp"
#include <mysql/mysql.h>
#include "OJ.cc"
#include <iostream>
int main()
{
/*  Json::Value req;
    req["code"]="#include <stdio.h>\nint main() {printf(\"hello\");return 0;}";
    req["stdin"]="";
    Json::Value resp;
    Compile::ComAndRun(req,&resp);
    Json::FastWriter writer;
*/
    MYSQL* mysql=my_mysql::MysqlInit();
   Decribe de(mysql);
    Json::Value ans;
    de.get_question(1,&ans);
    Json::StyledWriter writer;//有格式的序列化                     
     std:: cout<<writer.write(ans)<<std::endl;
     std::cout<<"-------"<<std::endl;
//cout<<ans[0]["id"]<<" "<<ans[0]["name"]<<"  "<<ans[0]["star"];
     my_mysql::MysqlRelease(mysql); 
    return 0;
}
