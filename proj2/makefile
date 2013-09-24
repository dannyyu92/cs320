CPP=g++ -g

all: cache-sim

cache-sim: Project2.o Driver.o
	$(CPP) Project2.o Driver.o -o cache-sim

Project2.o: Project2.cpp
	$(CPP) -c Project2.cpp

Driver.o: Driver.cpp
	$(CPP) -c Driver.cpp

clean:
	rm -f *.o cache-sim
