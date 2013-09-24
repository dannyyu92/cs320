#include "Project2.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
#include <cmath>
#include <cstdlib>
using namespace std;

Project2::Project2() {
  inputFileName = "";
  outputFileName = "";
}

Project2::Project2(string inputFile, string outputFile) {
  inputFileName = inputFile;
  outputFileName = outputFile;
}

Project2::~Project2() {
}

void Project2::parseInput() {
  unsigned long long addr;
  string instr, line;

  double totalNum = 0;
  //int miss = 0;

/** Initialization for direct mapped **/
  // hit rates for direct mapped
  int hitDirect1KB = 0;
  int hitDirect4KB = 0;
  int hitDirect16KB = 0;
  int hitDirect32KB = 0;

  // initialization for direct mapped cache 1KB
  int cacheLineDirect1KB = 32;
  int indexDirect1KB = 0;
  unsigned long long tagDirect1KB = 0;
  unsigned long long *directCache1KB = new unsigned long long[32];
  for (int i=0; i<32; i++) {
    directCache1KB[i] = 0;
  }
  // initialization for direct mapped cache 4KB
  int cacheLineDirect4KB = 128;
  int indexDirect4KB = 0;
  unsigned long long tagDirect4KB = 0;
  unsigned long long *directCache4KB = new unsigned long long[128];
  for (int i=0; i<128; i++) {
    directCache4KB[i] = 0;
  } 
  // initialization for direct mapped cache 16KB
  int cacheLineDirect16KB = 512;
  int indexDirect16KB = 0;
  unsigned long long tagDirect16KB = 0;
  unsigned long long *directCache16KB = new unsigned long long[512];
  for (int i=0; i<512; i++) {
    directCache16KB[i] = 0;
  } 
  // initialization for direct mapped cache 32KB
  int cacheLineDirect32KB = 1024;
  int indexDirect32KB = 0;
  unsigned long long tagDirect32KB = 0;
  unsigned long long *directCache32KB = new unsigned long long[1024];
  for (int i=0; i<1024; i++) {
    directCache32KB[i] = 0;
  } 

/** Initialization for set associative **/

  // hit rates for 2-Way set associative
  int hitSetAssoc2W = 0; 
  // initialization for set-associative cache 16KB
  int assoc2W = 2;
  int cacheLineSetAssoc2W = 256;
  int setSetAssoc2W = 0;
  unsigned long long tagSetAssoc2W = 0;
  // cache
  unsigned long long **setAssoc2W = new unsigned long long *[256];
  // LRU
  unsigned long long **LRUSetAssoc2W = new unsigned long long *[256];
  for (int i=0; i<256; i++) {
    LRUSetAssoc2W[i] = new unsigned long long [2];
    setAssoc2W[i] = new unsigned long long [2];
  }
  // initialize
  for (int i=0; i<256; i++) {
    for (int j=0; j<2; j++) {
      LRUSetAssoc2W[i][j] = j; 
      setAssoc2W[i][j] = -1; 
    }
  }

  // hit rates for 4-Way set associative
  int hitSetAssoc4W = 0; 
  // initialization for set-associative cache 16KB
  int assoc4W = 4;
  int cacheLineSetAssoc4W = 128;
  int setSetAssoc4W = 0;
  unsigned long long tagSetAssoc4W = 0;
  // cache
  unsigned long long **setAssoc4W = new unsigned long long *[128];
  // LRU
  unsigned long long **LRUSetAssoc4W = new unsigned long long *[128];
  for (int i=0; i<128; i++) {
    LRUSetAssoc4W[i] = new unsigned long long [4];
    setAssoc4W[i] = new unsigned long long [4];
  }
  // initialize
  for (int i=0; i<128; i++) {
    for (int j=0; j<4; j++) {
      LRUSetAssoc4W[i][j] = j; 
      setAssoc4W[i][j] = -1; 
    }
  }

  // hit rates for 8-Way set associative
  int hitSetAssoc8W = 0; 
  // initialization for set-associative cache 16KB
  int assoc8W = 8;
  int cacheLineSetAssoc8W = 64;
  int setSetAssoc8W = 0;
  unsigned long long tagSetAssoc8W = 0;
  // cache
  unsigned long long **setAssoc8W = new unsigned long long *[64];
  // LRU
  unsigned long long **LRUSetAssoc8W = new unsigned long long *[64];
  for (int i=0; i<64; i++) {
    LRUSetAssoc8W[i] = new unsigned long long [8];
    setAssoc8W[i] = new unsigned long long [8];
  }
  // initialize
  for (int i=0; i<64; i++) {
    for (int j=0; j<8; j++) {
      LRUSetAssoc8W[i][j] = j; 
      setAssoc8W[i][j] = -1; 
    }
  }

  // hit rates for 16-Way set associative
  int hitSetAssoc16W = 0; 
  // initialization for set-associative cache 16KB
  int assoc16W = 16;
  int cacheLineSetAssoc16W = 32;
  int setSetAssoc16W = 0;
  unsigned long long tagSetAssoc16W = 0;
  // cache
  unsigned long long **setAssoc16W = new unsigned long long *[32];
  // LRU
  unsigned long long **LRUSetAssoc16W = new unsigned long long *[32];
  for (int i=0; i<32; i++) {
    LRUSetAssoc16W[i] = new unsigned long long [16];
    setAssoc16W[i] = new unsigned long long [16];
  }
  // initialize
  for (int i=0; i<32; i++) {
    for (int j=0; j<16; j++) {
      LRUSetAssoc16W[i][j] = j; 
      setAssoc16W[i][j] = -1; 
    }
  }

/** Initialization for fully associative **/

  // hit rate for fully associative with LRU
  int hitFullyAssoc = 0;
  int cacheLineFullyAssoc = 512;
  unsigned long long tagFullyAssoc = 0;
  // cache
  unsigned long long **fullyAssoc = new unsigned long long *[1];
  // LRU
  unsigned long long **LRUFullyAssoc = new unsigned long long *[1];
  for (int i=0; i<1; i++) {
    LRUFullyAssoc[i] = new unsigned long long [512];
    fullyAssoc[i] = new unsigned long long [512];
  }
  for (int i=0; i<1; i++) {
    for (int j=0; j<512; j++) {
      LRUFullyAssoc[i][j] = j;
      fullyAssoc[i][j] = -1;
    }
  }

  // hit rate for fully associative with random rep policy
  int hitFullyAssocRand = 0;
  int cacheLineFullyAssocRand = 512;
  unsigned long long tagFullyAssocRand = 0;
  // cache
  unsigned long long **fullyAssocRand = new unsigned long long *[1];
  // LRU
  unsigned long long **LRUFullyAssocRand = new unsigned long long *[1];
  for (int i=0; i<1; i++) {
    LRUFullyAssocRand[i] = new unsigned long long [512];
    fullyAssocRand[i] = new unsigned long long [512];
  }
  for (int i=0; i<1; i++) {
    for (int j=0; j<512; j++) {
      LRUFullyAssocRand[i][j] = j;
      fullyAssocRand[i][j] = -1;
    }
  }


/** Initialization for set associative with no allocation on a write miss **/

  // SET ASSOCIATIVE CACHE WITH NO ALLOCATION ON A WRITE MISS
  // hit rates for 2-Way set associative
  int hitSetAssoc2W2 = 0; 
  // initialization for set-associative cache 16KB
  int assoc2W2 = 2;
  int cacheLineSetAssoc2W2 = 256;
  int setSetAssoc2W2 = 0;
  unsigned long long tagSetAssoc2W2 = 0;
  // cache
  unsigned long long **setAssoc2W2 = new unsigned long long *[256];
  // LRU
  unsigned long long **LRUSetAssoc2W2 = new unsigned long long *[256];
  for (int i=0; i<256; i++) {
    LRUSetAssoc2W2[i] = new unsigned long long [2];
    setAssoc2W2[i] = new unsigned long long [2];
  }
  // initialize
  for (int i=0; i<256; i++) {
    for (int j=0; j<2; j++) {
      LRUSetAssoc2W2[i][j] = j; 
      setAssoc2W2[i][j] = -1; 
    }
  }

  // hit rates for 4-Way set associative
  int hitSetAssoc4W2 = 0; 
  // initialization for set-associative cache 16KB
  int assoc4W2 = 4;
  int cacheLineSetAssoc4W2 = 128;
  int setSetAssoc4W2 = 0;
  unsigned long long tagSetAssoc4W2 = 0;
  // cache
  unsigned long long **setAssoc4W2 = new unsigned long long *[128];
  // LRU
  unsigned long long **LRUSetAssoc4W2 = new unsigned long long *[128];
  for (int i=0; i<128; i++) {
    LRUSetAssoc4W2[i] = new unsigned long long [4];
    setAssoc4W2[i] = new unsigned long long [4];
  }
  // initialize
  for (int i=0; i<128; i++) {
    for (int j=0; j<4; j++) {
      LRUSetAssoc4W2[i][j] = j; 
      setAssoc4W2[i][j] = -1; 
    }
  }

  // hit rates for 8-Way set associative
  int hitSetAssoc8W2 = 0; 
  // initialization for set-associative cache 16KB
  int assoc8W2 = 8;
  int cacheLineSetAssoc8W2 = 64;
  int setSetAssoc8W2 = 0;
  unsigned long long tagSetAssoc8W2 = 0;
  // cache
  unsigned long long **setAssoc8W2 = new unsigned long long *[64];
  // LRU
  unsigned long long **LRUSetAssoc8W2 = new unsigned long long *[64];
  for (int i=0; i<64; i++) {
    LRUSetAssoc8W2[i] = new unsigned long long [8];
    setAssoc8W2[i] = new unsigned long long [8];
  }
  // initialize
  for (int i=0; i<64; i++) {
    for (int j=0; j<8; j++) {
      LRUSetAssoc8W2[i][j] = j; 
      setAssoc8W2[i][j] = -1; 
    }
  }

  // hit rates for 16-Way set associative
  int hitSetAssoc16W2 = 0; 
  // initialization for set-associative cache 16KB
  int assoc16W2 = 16;
  int cacheLineSetAssoc16W2 = 32;
  int setSetAssoc16W2 = 0;
  unsigned long long tagSetAssoc16W2 = 0;
  // cache
  unsigned long long **setAssoc16W2 = new unsigned long long *[32];
  // LRU
  unsigned long long **LRUSetAssoc16W2 = new unsigned long long *[32];
  for (int i=0; i<32; i++) {
    LRUSetAssoc16W2[i] = new unsigned long long [16];
    setAssoc16W2[i] = new unsigned long long [16];
  }
  // initialize
  for (int i=0; i<32; i++) {
    for (int j=0; j<16; j++) {
      LRUSetAssoc16W2[i][j] = j; 
      setAssoc16W2[i][j] = -1; 
    }
  }






/** File reading starts here **/

  //read/write files
  ifstream input(inputFileName.c_str());
  if (input.is_open()) {
	    
    ofstream fout(outputFileName.c_str());

	
	while(getline(input, line)) {
            stringstream ss(line);
            ss>>instr>>std::hex>>addr;
            totalNum++;
            //cout<<instr<< "  "  << addr<< " "<<endl;

/* DIRECT MAPPED CACHE */
            // Get indices for direct mapped caches
            // 1KB
            indexDirect1KB = (addr>>5)%cacheLineDirect1KB;
            tagDirect1KB = addr>>((unsigned long long)(log2(cacheLineDirect1KB))+5);
            // 4KB
	    indexDirect4KB = (addr>>5)%cacheLineDirect4KB;
            tagDirect4KB = addr>>((unsigned long long)(log2(cacheLineDirect4KB))+5);
            // 16KB
	    indexDirect16KB = (addr>>5)%cacheLineDirect16KB;
            tagDirect16KB = addr>>((unsigned long long)(log2(cacheLineDirect16KB))+5);
            // 32KB
	    indexDirect32KB = (addr>>5)%cacheLineDirect32KB;
            tagDirect32KB = addr>>((unsigned long long)(log2(cacheLineDirect32KB))+5);

	    // direct mapped cache 1KB
            if (directCache1KB[indexDirect1KB] == tagDirect1KB) {
              hitDirect1KB++;
            }
            else {
              //miss++;
              directCache1KB[indexDirect1KB] = tagDirect1KB;
            }

	    // direct mapped cache 4KB
            if (directCache4KB[indexDirect4KB] == tagDirect4KB) {
              hitDirect4KB++;
            }
            else {
	      //miss++;
              directCache4KB[indexDirect4KB] = tagDirect4KB;
            }

	    // direct mapped cache 16KB
            if (directCache16KB[indexDirect16KB] == tagDirect16KB) {
              hitDirect16KB++;
            }
            else {
	      //miss++;
              directCache16KB[indexDirect16KB] = tagDirect16KB;
            }

	    // direct mapped cache 32KB
            if (directCache32KB[indexDirect32KB] == tagDirect32KB) {
              hitDirect32KB++;
            }
            else {
              //miss++;
              directCache32KB[indexDirect32KB] = tagDirect32KB;
            }

/** 2-WAY SET ASSOCIATIVE **/

            setSetAssoc2W = (addr>>5)%cacheLineSetAssoc2W;
            tagSetAssoc2W = addr>>((unsigned long long)log2(cacheLineSetAssoc2W)+5);
	    int way2W = 0;
            bool found2W = 0;
            for (int i=0; i<assoc2W; i++) {
              if (setAssoc2W[setSetAssoc2W][i] == tagSetAssoc2W) {
                found2W = 1;
                way2W = i;
              }
            }   
            // if hit     
            if (found2W) {
              int LRUIndex2W = -1;
              // check where in LRU it is
              for (int i=0; i<assoc2W; i++) {
                if(LRUSetAssoc2W[setSetAssoc2W][i] == way2W) {
                  LRUIndex2W = i;
                }
              }
              // shift LRU over, update most recently used
              for (int i=0; i<LRUIndex2W; i++) {
                LRUSetAssoc2W[setSetAssoc2W][LRUIndex2W-i] = LRUSetAssoc2W[setSetAssoc2W][(LRUIndex2W-1)-i];
              }
              LRUSetAssoc2W[setSetAssoc2W][0] = way2W;
            
            hitSetAssoc2W++;
            } 
            // if miss
            else {
              unsigned long long temp = LRUSetAssoc2W[setSetAssoc2W][assoc2W-1];
              for (int i=0; i<assoc2W; i++) {
                LRUSetAssoc2W[setSetAssoc2W][assoc2W-i] = LRUSetAssoc2W[setSetAssoc2W][(assoc2W-1)-i];
              }
              LRUSetAssoc2W[setSetAssoc2W][0] = temp;
              setAssoc2W[setSetAssoc2W][LRUSetAssoc2W[setSetAssoc2W][0]] = tagSetAssoc2W;
            }

/** 4-WAY SET ASSOCIATIVE **/

            setSetAssoc4W = (addr>>5)%cacheLineSetAssoc4W;
            tagSetAssoc4W = addr>>((unsigned long long)log2(cacheLineSetAssoc4W)+5);
	    int way4W = 0;
            bool found4W = 0;
            for (int i=0; i<assoc4W; i++) {
              if (setAssoc4W[setSetAssoc4W][i] == tagSetAssoc4W) {
                found4W = 1;
                way4W = i;
              }
            }   
            // if hit     
            if (found4W) {
              int LRUIndex4W = -1;
              for (int i=0; i<assoc4W; i++) {
                if(LRUSetAssoc4W[setSetAssoc4W][i] == way4W) {
                  LRUIndex4W = i;
                }
              }
              for (int i=0; i<LRUIndex4W; i++) {
                LRUSetAssoc4W[setSetAssoc4W][LRUIndex4W-i] = LRUSetAssoc4W[setSetAssoc4W][(LRUIndex4W-1)-i];
              }
              LRUSetAssoc4W[setSetAssoc4W][0] = way4W;
            
            hitSetAssoc4W++;
            } 
            // if miss
            else {
              unsigned long long temp = LRUSetAssoc4W[setSetAssoc4W][assoc4W-1];
              for (int i=0; i<assoc4W; i++) {
                LRUSetAssoc4W[setSetAssoc4W][assoc4W-i] = LRUSetAssoc4W[setSetAssoc4W][(assoc4W-1)-i];
              }
              LRUSetAssoc4W[setSetAssoc4W][0] = temp;
              setAssoc4W[setSetAssoc4W][LRUSetAssoc4W[setSetAssoc4W][0]] = tagSetAssoc4W;
            }

/** 8-WAY SET ASSOCIATIVE **/

            setSetAssoc8W = (addr>>5)%cacheLineSetAssoc8W;
            tagSetAssoc8W = addr>>((unsigned long long)log2(cacheLineSetAssoc8W)+5);
	    int way8W = 0;
            bool found8W = 0;
            for (int i=0; i<assoc8W; i++) {
              if (setAssoc8W[setSetAssoc8W][i] == tagSetAssoc8W) {
                found8W = 1;
                way8W = i;
              }
            }   
            // if hit     
            if (found8W) {
              int LRUIndex8W = -1;
              for (int i=0; i<assoc8W; i++) {
                if(LRUSetAssoc8W[setSetAssoc8W][i] == way8W) {
                  LRUIndex8W = i;
                }
              }
              for (int i=0; i<LRUIndex8W; i++) {
                LRUSetAssoc8W[setSetAssoc8W][LRUIndex8W-i] = LRUSetAssoc8W[setSetAssoc8W][(LRUIndex8W-1)-i];
              }
              LRUSetAssoc8W[setSetAssoc8W][0] = way8W;
            
            hitSetAssoc8W++;
            } 
            // if miss
            else {
              unsigned long long temp = LRUSetAssoc8W[setSetAssoc8W][assoc8W-1];
              for (int i=0; i<assoc8W; i++) {
                LRUSetAssoc8W[setSetAssoc8W][assoc8W-i] = LRUSetAssoc8W[setSetAssoc8W][(assoc8W-1)-i];
              }
              LRUSetAssoc8W[setSetAssoc8W][0] = temp;
              setAssoc8W[setSetAssoc8W][LRUSetAssoc8W[setSetAssoc8W][0]] = tagSetAssoc8W;
            }


/** 16-WAY SET ASSOCIATIVE **/

            setSetAssoc16W = (addr>>5)%cacheLineSetAssoc16W;
            tagSetAssoc16W = addr>>((unsigned long long)log2(cacheLineSetAssoc16W)+5);
	    int way16W = 0;
            bool found16W = 0;
            for (int i=0; i<assoc16W; i++) {
              if (setAssoc16W[setSetAssoc16W][i] == tagSetAssoc16W) {
                found16W = 1;
                way16W = i;
              }
            }   
            // if hit     
            if (found16W) {
              int LRUIndex16W = -1;
              for (int i=0; i<assoc16W; i++) {
                if(LRUSetAssoc16W[setSetAssoc16W][i] == way16W) {
                  LRUIndex16W = i;
                }
              }
              for (int i=0; i<LRUIndex16W; i++) {
                LRUSetAssoc16W[setSetAssoc16W][LRUIndex16W-i] = LRUSetAssoc16W[setSetAssoc16W][(LRUIndex16W-1)-i];
              }
              LRUSetAssoc16W[setSetAssoc16W][0] = way16W;
            
            hitSetAssoc16W++;
            } 
            // if miss
            else {
              unsigned long long temp = LRUSetAssoc16W[setSetAssoc16W][assoc16W-1];
              for (int i=0; i<assoc16W; i++) {
                LRUSetAssoc16W[setSetAssoc16W][assoc16W-i] = LRUSetAssoc16W[setSetAssoc16W][(assoc16W-1)-i];
              }
              LRUSetAssoc16W[setSetAssoc16W][0] = temp;
              setAssoc16W[setSetAssoc16W][LRUSetAssoc16W[setSetAssoc16W][0]] = tagSetAssoc16W;
            }

/** Fully Associative Cache with LRU **/

            tagFullyAssoc = addr>>5;
            int indexFullyAssoc = 0;
            bool foundFullyAssoc = 0;
            for (int i=0; i<cacheLineFullyAssoc; i++) {
              if (fullyAssoc[0][i] == tagFullyAssoc) {
                foundFullyAssoc = 1;
                indexFullyAssoc = i;
              }
            }
            // if hit
            if (foundFullyAssoc) {
              int LRUIndexFullyAssoc = -1;
              for (int i=0; i<cacheLineFullyAssoc; i++) {
                if(LRUFullyAssoc[0][i] == indexFullyAssoc) {
                  LRUIndexFullyAssoc = i;
                }
              }
              for (int i=0; i<LRUIndexFullyAssoc; i++) {
                LRUFullyAssoc[0][LRUIndexFullyAssoc-i] = LRUFullyAssoc[0][(LRUIndexFullyAssoc-1)-i];
              }
              LRUFullyAssoc[0][0] = indexFullyAssoc;
              hitFullyAssoc++;
            }
            // if miss
            else {
              unsigned long long temp = LRUFullyAssoc[0][cacheLineFullyAssoc-1];
              for (int i=0; i<cacheLineFullyAssoc; i++) {
                LRUFullyAssoc[0][cacheLineFullyAssoc-i] = LRUFullyAssoc[0][(cacheLineFullyAssoc-1)-i];
              }
              LRUFullyAssoc[0][0] = temp;
              fullyAssoc[0][LRUFullyAssoc[0][0]] = tagFullyAssoc;
            }

/** Fully Associative Cache with random replacement policy **/

            tagFullyAssocRand = addr>>5;
            int indexFullyAssocRand = 0;
            bool foundFullyAssocRand = 0;
            for (int i=0; i<cacheLineFullyAssocRand; i++) {
              if (fullyAssocRand[0][i] == tagFullyAssocRand) {
                foundFullyAssocRand = 1;
                indexFullyAssocRand = i;
              }
            }
            // if hit
            if (foundFullyAssocRand) {
              hitFullyAssocRand++;
            }
            // if miss
            else {
              int emptyLineIndex = -1;
              for (int i=0; i<cacheLineFullyAssocRand; i++) {
                if (fullyAssocRand[0][i] == -1) {
                  emptyLineIndex = i;
                }
              }
              if (emptyLineIndex != -1) {
                fullyAssocRand[0][emptyLineIndex] = tagFullyAssocRand;
              }
              else {
                int randomInt;
                randomInt = (rand()%511);
                fullyAssocRand[0][randomInt] = tagFullyAssocRand;
              }
              //fullyAssocRand[0][LRUFullyAssocRand[0][0]] = tagFullyAssocRand;
            }

/** SET ASSOCIATIVE WITH NO ALLOCATION ON A WRITE MISS **/

/** 2-WAY SET ASSOCIATIVE **/
            setSetAssoc2W2 = (addr>>5)%cacheLineSetAssoc2W2;
            tagSetAssoc2W2 = addr>>((unsigned long long)log2(cacheLineSetAssoc2W2)+5);
	    int way2W2 = 0;
            bool found2W2 = 0;
            for (int i=0; i<assoc2W2; i++) {
              if (setAssoc2W2[setSetAssoc2W2][i] == tagSetAssoc2W2) {
                found2W2 = 1;
                way2W2 = i;
              }
            }   
            // if hit     
            if (found2W2) {
              int LRUIndex2W2 = -1;
              for (int i=0; i<assoc2W2; i++) {
                if(LRUSetAssoc2W2[setSetAssoc2W2][i] == way2W2) {
                  LRUIndex2W2 = i;
                }
              }
              for (int i=0; i<LRUIndex2W2; i++) {
                LRUSetAssoc2W2[setSetAssoc2W2][LRUIndex2W2-i] = LRUSetAssoc2W2[setSetAssoc2W2][(LRUIndex2W2-1)-i];
              }
              LRUSetAssoc2W2[setSetAssoc2W2][0] = way2W2;
            
            hitSetAssoc2W2++;
            } 
            // if miss
            else {
              if (instr == "L") {
              unsigned long long temp = LRUSetAssoc2W2[setSetAssoc2W2][assoc2W2-1];
              for (int i=0; i<assoc2W2; i++) {
                LRUSetAssoc2W2[setSetAssoc2W2][assoc2W2-i] = LRUSetAssoc2W2[setSetAssoc2W2][(assoc2W2-1)-i];
              }
              LRUSetAssoc2W2[setSetAssoc2W2][0] = temp;
              setAssoc2W2[setSetAssoc2W2][LRUSetAssoc2W2[setSetAssoc2W2][0]] = tagSetAssoc2W2;
              }
            }
/** 4-WAY SET ASSOCIATIVE **/

            setSetAssoc4W2 = (addr>>5)%cacheLineSetAssoc4W2;
            tagSetAssoc4W2 = addr>>((unsigned long long)log2(cacheLineSetAssoc4W2)+5);
	    int way4W2 = 0;
            bool found4W2 = 0;
            for (int i=0; i<assoc4W2; i++) {
              if (setAssoc4W2[setSetAssoc4W2][i] == tagSetAssoc4W2) {
                found4W2 = 1;
                way4W2 = i;
              }
            }   
            // if hit     
            if (found4W2) {
              int LRUIndex4W2 = -1;
              for (int i=0; i<assoc4W2; i++) {
                if(LRUSetAssoc4W2[setSetAssoc4W2][i] == way4W2) {
                  LRUIndex4W2 = i;
                }
              }
              for (int i=0; i<LRUIndex4W2; i++) {
                LRUSetAssoc4W2[setSetAssoc4W2][LRUIndex4W2-i] = LRUSetAssoc4W2[setSetAssoc4W2][(LRUIndex4W2-1)-i];
              }
              LRUSetAssoc4W2[setSetAssoc4W2][0] = way4W2;
            
            hitSetAssoc4W2++;
            } 
            // if miss
            else {
              if (instr == "L") {
              unsigned long long temp = LRUSetAssoc4W2[setSetAssoc4W2][assoc4W2-1];
              for (int i=0; i<assoc4W2; i++) {
                LRUSetAssoc4W2[setSetAssoc4W2][assoc4W2-i] = LRUSetAssoc4W2[setSetAssoc4W2][(assoc4W2-1)-i];
              }
              LRUSetAssoc4W2[setSetAssoc4W2][0] = temp;
              setAssoc4W2[setSetAssoc4W2][LRUSetAssoc4W2[setSetAssoc4W2][0]] = tagSetAssoc4W2;
              }
            }

/** 8-WAY SET ASSOCIATIVE **/

            setSetAssoc8W2 = (addr>>5)%cacheLineSetAssoc8W2;
            tagSetAssoc8W2 = addr>>((unsigned long long)log2(cacheLineSetAssoc8W2)+5);
	    int way8W2 = 0;
            bool found8W2 = 0;
            for (int i=0; i<assoc8W2; i++) {
              if (setAssoc8W2[setSetAssoc8W2][i] == tagSetAssoc8W2) {
                found8W2 = 1;
                way8W2 = i;
              }
            }   
            // if hit     
            if (found8W2) {
              int LRUIndex8W2 = -1;
              for (int i=0; i<assoc8W2; i++) {
                if(LRUSetAssoc8W2[setSetAssoc8W2][i] == way8W2) {
                  LRUIndex8W2 = i;
                }
              }
              for (int i=0; i<LRUIndex8W2; i++) {
                LRUSetAssoc8W2[setSetAssoc8W2][LRUIndex8W2-i] = LRUSetAssoc8W2[setSetAssoc8W2][(LRUIndex8W2-1)-i];
              }
              LRUSetAssoc8W2[setSetAssoc8W2][0] = way8W2;
            
            hitSetAssoc8W2++;
            } 
            // if miss
            else {
              if (instr == "L") {
              unsigned long long temp = LRUSetAssoc8W2[setSetAssoc8W2][assoc8W2-1];
              for (int i=0; i<assoc8W2; i++) {
                LRUSetAssoc8W2[setSetAssoc8W2][assoc8W2-i] = LRUSetAssoc8W2[setSetAssoc8W2][(assoc8W2-1)-i];
              }
              LRUSetAssoc8W2[setSetAssoc8W2][0] = temp;
              setAssoc8W2[setSetAssoc8W2][LRUSetAssoc8W2[setSetAssoc8W2][0]] = tagSetAssoc8W2;
              }
            }


/** 16-WAY SET ASSOCIATIVE **/
            setSetAssoc16W2 = (addr>>5)%cacheLineSetAssoc16W2;
            tagSetAssoc16W2 = addr>>((unsigned long long)log2(cacheLineSetAssoc16W2)+5);
	    int way16W2 = 0;
            bool found16W2 = 0;
            for (int i=0; i<assoc16W2; i++) {
              if (setAssoc16W2[setSetAssoc16W2][i] == tagSetAssoc16W2) {
                found16W2 = 1;
                way16W2 = i;
              }
            }   
            // if hit     
            if (found16W2) {
              int LRUIndex16W2 = -1;
              for (int i=0; i<assoc16W2; i++) {
                if(LRUSetAssoc16W2[setSetAssoc16W2][i] == way16W2) {
                  LRUIndex16W2 = i;
                }
              }
              for (int i=0; i<LRUIndex16W2; i++) {
                LRUSetAssoc16W2[setSetAssoc16W2][LRUIndex16W2-i] = LRUSetAssoc16W2[setSetAssoc16W2][(LRUIndex16W2-1)-i];
              }
              LRUSetAssoc16W2[setSetAssoc16W2][0] = way16W2;
            
            hitSetAssoc16W2++;
            } 
            // if miss
            else {
              if (instr == "L") {
              unsigned long long temp = LRUSetAssoc16W2[setSetAssoc16W2][assoc16W2-1];
              for (int i=0; i<assoc16W2; i++) {
                LRUSetAssoc16W2[setSetAssoc16W2][assoc16W2-i] = LRUSetAssoc16W2[setSetAssoc16W2][(assoc16W2-1)-i];
              }
              LRUSetAssoc16W2[setSetAssoc16W2][0] = temp;
              setAssoc16W2[setSetAssoc16W2][LRUSetAssoc16W2[setSetAssoc16W2][0]] = tagSetAssoc16W2;
              }
            }







	}
        delete[] directCache1KB;
        delete[] directCache4KB;
        delete[] directCache16KB;
        delete[] directCache32KB;
        //WRITE OUTPUT TO FILE

	//fout << round(100*numTaken/totalNum) << endl;
        // direct mapped results
        cout << round(100*(hitDirect1KB/totalNum)) << " " << round(100*(hitDirect4KB/totalNum)) << " " << round(100*(hitDirect16KB/totalNum)) << " " << round(100*(hitDirect32KB/totalNum)) << endl;
        // set associative results
	cout << round(100*(hitSetAssoc2W/totalNum)) << " " << round(100*(hitSetAssoc4W/totalNum))<< " " << round(100*(hitSetAssoc8W/totalNum)) << " " << round(100*(hitSetAssoc16W/totalNum)) <<endl;
        // fully assoc with LRU
	cout << round(100*(hitFullyAssoc/totalNum)) << endl;
        // fully assoc with LRU
	cout << round(100*(hitFullyAssocRand/totalNum)) << endl;
        // set associative results with no allocation on write miss
	cout << round(100*(hitSetAssoc2W2/totalNum)) << " " << round(100*(hitSetAssoc4W2/totalNum))<< " " << round(100*(hitSetAssoc8W2/totalNum)) << " " << round(100*(hitSetAssoc16W2/totalNum)) <<endl;


        // direct mapped results
        fout << round(100*(hitDirect1KB/totalNum)) << " " << round(100*(hitDirect4KB/totalNum)) << " " << round(100*(hitDirect16KB/totalNum)) << " " << round(100*(hitDirect32KB/totalNum)) << endl;
        // set associative results
	fout << round(100*(hitSetAssoc2W/totalNum)) << " " << round(100*(hitSetAssoc4W/totalNum))<< " " << round(100*(hitSetAssoc8W/totalNum)) << " " << round(100*(hitSetAssoc16W/totalNum)) <<endl;
        // fully assoc with LRU
	fout << round(100*(hitFullyAssoc/totalNum)) << endl;
        // fully assoc with LRU
	fout << round(100*(hitFullyAssocRand/totalNum)) << endl;
        // set associative results with no allocation on write miss
	fout << round(100*(hitSetAssoc2W2/totalNum)) << " " << round(100*(hitSetAssoc4W2/totalNum))<< " " << round(100*(hitSetAssoc8W2/totalNum)) << " " << round(100*(hitSetAssoc16W2/totalNum)) <<endl;


        // close file streams
	input.close();
	fout.close();

  }
  else {
	cout<< "Input file was not found." << endl;
  }
}




