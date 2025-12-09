graph_tests: graph_tests.cpp graph.cpp graph.h makefile
	g++ -o graph_tests -g -O0 -fsanitize=address graph_tests.cpp
