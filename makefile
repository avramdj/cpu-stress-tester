compile: main.c stress.s libstress.cpp
	g++ main.cpp libstress.cpp stress.s -pthread -o stress
