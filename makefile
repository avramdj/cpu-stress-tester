compile: main.c stress.s stresslib.cpp
	g++ main.cpp stresslib.cpp stress.s -pthread -o stress
