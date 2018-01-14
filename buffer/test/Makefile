main:
	g++ -std=c++11 ../buffer.cc -fPIC -shared -o libbuffer.so
	g++ -std=c++11 test.cpp -o main_temp -L. -lprotobuf -I.
clean:
	rm -rf *_temp
	rm -rf *.so
