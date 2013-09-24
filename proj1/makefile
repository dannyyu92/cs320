CPP=g++ -g

all: predictors

predictors: Project1.o Driver.o
	$(CPP) Project1.o Driver.o -o predictors

Project1.o: Project1.cpp
	$(CPP) -c Project1.cpp

Driver.o: Driver.cpp
	$(CPP) -c Driver.cpp

clean:
	rm -f *.o predictors
