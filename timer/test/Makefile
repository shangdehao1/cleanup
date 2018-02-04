INCS=-I../..

main:
	#g++ -std=c++11 -fPIC -lboost_system -lpthread -o x_main main.cpp ../../controller/x_controller.o ${INCS}
	g++ -std=c++11 -fPIC -lboost_system -lpthread -o x_main main.cpp ${INCS}
	g++ -std=c++11 -fPIC -lboost_system -lpthread -c ../timeout_manager_impl.cc -o x_manager_impl.o ${INCS}
	g++ -std=c++11 -fPIC -lboost_system -lpthread -c ../timeout_manager.cc -o x_manager.o ${INCS}

clean:
	rm -rf x_*
