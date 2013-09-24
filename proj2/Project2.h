#ifndef Project2_H__
#define Project2_H__
#include <iostream>
using namespace std; 

class Project2 {
   public:
      Project2();
      Project2(string inputFile, string outputFile);
      ~Project2();
      void parseInput();
   private:
      string inputFileName;
      string outputFileName;
};

#endif // Project2_H__
