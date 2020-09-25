compile: main.cpp stress.s libstress.cpp
	g++ main.cpp libstress.cpp stress.s -std=c++11 -pthread -o stress
