CXXFLAGS = -std=c++0x
CFLAGS=-I

#最终目标名
target = main

target:main.o
	g++ ./code/main.o -o ./bin/main
main.o:main.cpp
	g++ ./code/main.cpp -c -o ./code/main.o

.PHONY:clean

clean:
	rm $(obj) &$(target) -f

