main:
	g++ -std=c++1y -o x_main main.cpp -lboost_system -lpthread
	g++ -std=c++1y -shared -o librequest.so request.cc -lboost_system -lpthread -fPIC
	g++ -std=c++1y -shared -o librequestparser.so request_parser.cc -lboost_system -lpthread -fPIC
	g++ -std=c++1y -shared -o libbinaryrequest.so binary_request.cc -lboost_system -lpthread -fPIC 
	g++ -std=c++1y -shared -o libbinaryrequestparser.so binary_request_parser.cc -lboost_system -lpthread -fPIC 

clean:
	rm -rf x_*
	rm -rf lib*.so
