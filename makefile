<<<<<<< HEAD
.
├── code           源代码
│   ├── skiplist_implement.h
│   ├── Node.h
│   ├── skiplist.h
│   └── main.cpp
├── test           接口测试
│   ├── stress
│   └── stress_test.cpp
├── bin            可执行文件
│   └── main
├── Makefile
├── test.sh
└── readme.md
=======
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
>>>>>>> d8031b743700db9b18b672d0aefcacb1f1f47657
