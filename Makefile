CXXFLAGS=-std=c++17 -Wall -Wextra -pedantic

make:
	g++ -o mini_se main.cpp  -lsqlite3 -lSQLiteCpp -lpthread #$@ $(CXXFLAGS)

server:
	g++ -o server server.cpp -lpthread -lsqlite3 -lSQLiteCpp

index:
	g++ -o index index.cpp -lsqlite3 -lSQLiteCpp

clean:
	rm server
