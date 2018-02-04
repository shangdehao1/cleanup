INCS=-I../..
LIB=-lboost_system -lpthread 

main:
	g++ -std=c++11 main.cpp -o x_main ${INCS} ${LIB}
	g++ -std=c++11 -c ../simple_channel_impl.cc -o simple_channel_impl.o ${INCS} ${LIB}
clean:
	rm -rf x_*
	rm -rf *.o

    

