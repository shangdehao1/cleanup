main:
	g++ -std=c++11 main.cpp -lboost_system -lpthread 
	g++ -std=c++11 -shared server_impl.cc -o libserverimpl.so -lboost_system -lpthread -fPIC
	g++ -std=c++11 -shared server.cc -o libserver.so -lboost_system -lpthread -fPIC
