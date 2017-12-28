main:
	g++ -std=c++11 buffer.cc -fPIC -shared -o libbuffer.so
	g++ -std=c++11 main.cpp -o x_main -L. -lbuffer -lprotobuf -I.
clean:
	rm -rf x_*
	rm -rf *.so
