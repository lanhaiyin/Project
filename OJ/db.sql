create database if not exists oj;    
use oj;    
create table if not exists table_allquestion(                                                                                                                        
    id int primary key auto_increment,    
          name varchar(32) unique comment "题目名称",
            star varchar(32)  comment "题目难度"    
            ) character set utf8 engine MyISAM;    
                
            create table if not exists table_test(    
                    id int,    
                          input varchar(32)  comment "测试输入",    
                              output varchar(32)  comment "测试输出"
                                   
                            ) character set utf8 engine MyISAM;    
                                
                            create table if not exists table_question(    
                                    id int primary key auto_increment comment "题目id",    
                                              title varchar(255) comment "题目描述",    
                                                        content text comment "代码"             
                                                                                
                                                        ) character set utf8 engine MyISAM;    


