#pragma once
#include <ctemplate/template.h>
#include <jsoncpp/json/json.h>
#include <iostream>
#include <string>
//#include "OJ.cc"
class OjView
{
    public:
        static void RenderAllquestion(Json::Value all_questions,std::string* html)
        {
           /* *html="<html>";
            *html="<body>";
            *html="<div>";
          *html+=all_questions[0]["id"].asString()+all_questions[0]["name"].asString()+all_questions[0]["star"].asString(); 
            *html+="</div>";
            *html+="</body>";
            *html+="</html>";
           // std:: cout<<all_questions[0];
            std::cout<<"打印html"<<std::endl;            
            std::cout<<html<<std::endl;
        */     ctemplate::TemplateDictionary dict("all_questions");
             for(const auto &question: all_questions)
            {
                ctemplate::TemplateDictionary* table_dict=dict.AddSectionDictionary("question");
                table_dict->SetValue("id",question["id"].asString());
               table_dict->SetValue("name",question["name"].asString());
                table_dict->SetValue("star",question["star"].asString());
            }
            ctemplate::Template* tpl;
            tpl = ctemplate::Template::GetTemplate("./template/allquestion.html", ctemplate::DO_NOT_STRIP);
            tpl->Expand(html, &dict);
        }
        static void Renderquestion(Json::Value questions,std::string* html)
        {
            /**html="<html>";
            *html="<body>";
            *html="<div>";
          *html+=questions["id"].asString()+questions["title"].asString();
            *html+="</div>";
            *html+="</body>";
            *html+="</html>";
          // std:: cout<<questions["id"];
            std::cout<<"打印html"<<std::endl;            
            std::cout<<*html<<std::endl;
            */
            Json::StyledWriter writer;//有格式的序列化                     
                std:: cout<<writer.write(questions)<<std::endl;

            ctemplate::TemplateDictionary dict("questions");
                dict.SetValue("id",questions["id"].asString());
               dict.SetValue("name",questions["name"].asString());
               dict.SetValue("star",questions["star"].asString());
               dict.SetValue("des",questions["des"].asString());
           //table_dict->SetValue("star",question["star"].asString());
               ctemplate::Template* tpl;
            tpl = ctemplate::Template::GetTemplate("./template/question.html", ctemplate::DO_NOT_STRIP);
            tpl->Expand(html, &dict);
        }
        static void RenderResult(Json::Value Test,Json::Value resp,std::string* html)
        {
            /**html="<html>";
            *html="<body>";
            *html="<div>";
          *html+=questions["id"].asString()+questions["title"].asString();
            *html+="</div>";
            *html+="</body>";
            *html+="</html>";
          // std:: cout<<questions["id"];
            std::cout<<"打印html"<<std::endl;            
            std::cout<<*html<<std::endl;
            */
           /* Json::StyledWriter writer;//有格式的序列化                     
                std:: cout<<"Test的内容  "<<writer.write(Test)<<std::endl;
            Json::StyledWriter writer2;//有格式的序列化                     
                std:: cout<<"resp的内容"<<writer2.write(resp)<<std::endl;

                //如果响应里面有error则返回
                if(resp["stderr"]!="")
                { 
                ctemplate::TemplateDictionary dict("resp");
                dict.SetValue("stderr",resp["stderr"].asString());
                dict.SetValue("reason",resp["reason"].asString());
               ctemplate::Template* tpl;
               tpl = ctemplate::Template::GetTemplate("./template/result.html", ctemplate::DO_NOT_STRIP);
                tpl->Expand(html, &dict);
               
                }else{

               int flag=1; 
                //如果测试错误则返回错误的信息
                for(auto it=Test.begin();it!=Test.end();++it)
                {
                    if((*it)["error"].asString()!="")
                    {
                        flag=0;
                 ctemplate::TemplateDictionary dict("Test");
                dict.SetValue("input",Test["input"].asString());
               dict.SetValue("error",Test["error"].asString());
               dict.SetValue("output",Test["output"].asString());
               ctemplate::Template* tpl;
              tpl = ctemplate::Template::GetTemplate("./template/test.html", ctemplate::DO_NOT_STRIP);
              tpl->Expand(html, &dict);
                      break;  
                    }
                }
                }*/
            *html+="<html>";
            *html+="<body>";
            *html+="<div>";
          *html+=Test["input"].asString()+Test["output"].asString();
            *html+="</div>";
            *html+="</body>";
            *html+="</html>";
          // std:: cout<<questions["id"];
            std::cout<<"打印html"<<std::endl;            
            std::cout<<*html<<std::endl;
                
}
};
