main:
	g++ -std=c++11 -o x_main main.cpp -lpthread -lboost_system
	g++ -std=c++11 -fPIC -o x_controller -shared controller.cc -lpthread -lboost_system
clean:
	rm x_* -rf
