OJ_Server:OJ_Server.cc compile.hpp util.hpp
	g++ $^ -o $@ -std=c++11 -L/usr/lib64/mysql -ljsoncpp -lmysqlclient -lpthread -lctemplate
.PHONY: all
clean:
	rm OJ_Server
