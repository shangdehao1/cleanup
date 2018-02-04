min:
	g++ -std=c++11 main.cpp -o x_main -lboost_system
	g++ -std=c++11 -c ../thread_group.cc -o libthread_group.so 
	g++ -std=c++11 -c ../io_service_pool.cc -o libx_io_service_pool.so
clean:
	rm -rf x_*
	rm -rf *.so
