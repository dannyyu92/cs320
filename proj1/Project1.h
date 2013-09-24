#ifndef PROJECT1_H__
#define PROJECT1_H__
#include <iostream>
using namespace std; 

class Project1 {
   public:
      Project1();
      Project1(string inputFile, string outputFile);
      ~Project1();
      void parseInput();
   private:
      string inputFileName;
      string outputFileName;
      int globalHistReg[9];
};

#endif // PROJECT1_H__
