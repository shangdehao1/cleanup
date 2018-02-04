main:
	g++ -std=c++11 main.cpp -o x_main -lboost_system -lpthread
	g++ -std=c++11 -shared ../client_impl.cc -o libclient_impl.so -lboost_system -lpthread -fPIC
clean:
	rm x_* -rf
