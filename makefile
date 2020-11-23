Ex6.out : lunaE6.o Real.o
	g++ -g -o Ex6.out lunaE6.o Real.o

lunaE6.o : lunaE6.cpp Real.h
	g++ -g -c lunaE6.cpp

Real.o : Real.cpp Real.h
	g++ -g -c Real.cpp

clean :
	rm *.o Ex6.out *.gch

submit : lunaE6.cpp Real.h Real.cpp makefile README.txt input01.txt expOutput01.txt
	rm -rf lunaE6
	mkdir lunaE6
	cp lunaE6.cpp lunaE6
	cp Real.h lunaE6
	cp Real.cpp lunaE6
	cp input01.txt lunaE6
	cp expOutput01.txt lunaE6
	cp makefile lunaE6
	tar cfvz lunaE6.tgz lunaE6
	cp lunaE6.tgz ~tiawatts/cs460drop
