create database if not exists blog_system;
use blog_system;
create table if not exists table_tag(
    id int primary key auto_increment,
      name varchar(32) unique comment "标签名称"
) character set utf8 engine MyISAM;

create table if not exists table_user(
    id int primary key auto_increment,
      name varchar(32)  comment "用户名称",
        unique key(name)
) character set utf8 engine MyISAM;

create table if not exists table_blog(
    id int primary key auto_increment comment "博客id",
      tag_id int comment "标签id",
        user_id int comment "用户id",
          title varchar(255) comment "博客标题",
            content text comment "博客正文",
              ctime datetime comment "博客最后更改时间",
                foreign key(tag_id) references table_tag(id),
                  foreign key(user_id) references table_user(id)
                    
) character set utf8 engine MyISAM;

