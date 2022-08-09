CXXFLAGS = -std=c++0x
CFLAGS=-I

#最终目标名
target = main

target:main.o
	g++ main.o -o $@
main.o:main.cpp
	g++ main.cpp -c -o main.o

.PHONY:clean

clean:
	rm $(obj) &$(target) -f