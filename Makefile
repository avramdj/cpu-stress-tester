compile:
	g++ src/main.cpp src/libstress.cpp src/stress.s -std=c++11 -pthread -I . -o bin/stress

.PHONY: clean

clean:
	rm stress
