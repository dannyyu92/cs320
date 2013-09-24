#include "Project1.h"
#include <fstream>
#include <iostream>
#include <sstream>
#include <math.h>
using namespace std;

Project1::Project1() {
  inputFileName = "";
  outputFileName = "";
}

Project1::Project1(string inputFile, string outputFile) {
  inputFileName = inputFile;
  outputFileName = outputFile;
}

Project1::~Project1() {
}

void Project1::parseInput() {
  unsigned long long addr;
  string behavior, line;

  double totalNum = 0;
  //counter for always taken
  int numTaken = 0;
  //counters for mispredictions
  int bimodalSingleBitCount8 = 0;
  int bimodalSingleBitCount16 = 0;
  int bimodalSingleBitCount32 = 0;
  int bimodalSingleBitCount128 = 0;
  int bimodalSingleBitCount256 = 0;
  int bimodalSingleBitCount512 = 0;
  int bimodalSingleBitCount1024 = 0;
  int bimodalTwoBitCount8 = 0;
  int bimodalTwoBitCount16 = 0;
  int bimodalTwoBitCount32 = 0;
  int bimodalTwoBitCount128 = 0;
  int bimodalTwoBitCount256 = 0;
  int bimodalTwoBitCount512 = 0;
  int bimodalTwoBitCount1024 = 0;
  int bimodalTournCount = 0;

  //single bit bimodal tables
  //table size 8
  int *bimodalSingleBit8 = new int[8];
  for (int i=0; i<8; i++) {
    bimodalSingleBit8[i] = 1;
  }
  //table size 16
  int *bimodalSingleBit16 = new int[16];
  for (int i=0; i<16; i++) {
    bimodalSingleBit16[i] = 1;
  }
  //table size 32
  int *bimodalSingleBit32 = new int[32];
  for (int i=0; i<32; i++) {
    bimodalSingleBit32[i] = 1;
  }
  //table size 128
  int *bimodalSingleBit128 = new int[128];
  for (int i=0; i<128; i++) {
    bimodalSingleBit128[i] = 1;
  }
  //table size 256
  int *bimodalSingleBit256 = new int[256];
  for (int i=0; i<256; i++) {
    bimodalSingleBit256[i] = 1;
  }
  //table size 512
  int *bimodalSingleBit512 = new int[512];
  for (int i=0; i<512; i++) {
    bimodalSingleBit512[i] = 1;
  }
  //table size 1024
  int *bimodalSingleBit1024 = new int[1024];
  for (int i=0; i<1024; i++) {
    bimodalSingleBit1024[i] = 1;
  }


  //two-bit bimodal tables
  // 3- TT, 2- T, 1- NT, 0- NN
  //table size 8
  int *bimodalTwoBit8 = new int[8];
  for (int i=0; i<8; i++) {
    bimodalTwoBit8[i] = 3;
  }
  //table size 16
  int *bimodalTwoBit16 = new int[16];
  for (int i=0; i<16; i++) {
    bimodalTwoBit16[i] = 3;
  }
  //table size 32
  int *bimodalTwoBit32 = new int[32];
  for (int i=0; i<32; i++) {
    bimodalTwoBit32[i] = 3;
  }
  //table size 128
  int *bimodalTwoBit128 = new int[128];
  for (int i=0; i<128; i++) {
    bimodalTwoBit128[i] = 3;
  }
  //table size 256
  int *bimodalTwoBit256 = new int[256];
  for (int i=0; i<256; i++) {
    bimodalTwoBit256[i] = 3;
  }
  //table size 512
  int *bimodalTwoBit512 = new int[512];
  for (int i=0; i<512; i++) {
    bimodalTwoBit512[i] = 3;
  }
  //table size 1024
  int *bimodalTwoBit1024 = new int[1024];
  // bimodal for tournament predictor
  int *bimodalTourn = new int[1024];
  for (int i=0; i<1024; i++) {
    bimodalTwoBit1024[i] = 3;
    bimodalTourn[i] = 3;
  }


  //gshare tables (fixed size 1024)
  int *gshare2 = new int[1024];
  int *gshare3 = new int[1024];
  int *gshare4 = new int[1024];
  int *gshare5 = new int[1024];
  int *gshare6 = new int[1024];
  int *gshare7 = new int[1024];
  int *gshare8 = new int[1024];
  int *gshare9 = new int[1024];
  int *gshare10 = new int[1024];
  //gshare table for tournament predictor
  int *gshareTourn = new int[1024];
  for (int i=0; i<1024; i++) {
    gshare2[i] = 3;
    gshare3[i] = 3;
    gshare4[i] = 3;
    gshare5[i] = 3;
    gshare6[i] = 3;
    gshare7[i] = 3;
    gshare8[i] = 3;
    gshare9[i] = 3;
    gshare10[i] = 3;
    gshareTourn[i] = 3;
  }


  // global history registers
  for (int i=0; i<9; i++) {
    globalHistReg[i] = 0;
  }
  // global history register for tournament predictor
  int globalHistRegT = 0;

  // masks for global history bits
  int mask[9] = {3,7,15,31,63,127,255,511,1023};


  int gshareCount2 = 0;
  int gshareCount3 = 0;
  int gshareCount4 = 0;
  int gshareCount5 = 0;
  int gshareCount6 = 0;
  int gshareCount7 = 0;
  int gshareCount8 = 0;
  int gshareCount9 = 0;
  int gshareCount10 = 0;
  //gshare counter for tournament predictor
  int gshareCountT = 0;

  int *selector = new int[1024];
  for (int i=0; i<1024; i++) {
    selector[i] = 0;
  }
  bool bimodalCorrect = 0;
  bool gshareCorrect = 0;
  int selectorCounter = 0;


  //read/write files
  ifstream input(inputFileName.c_str());
  if (input.is_open()) {
	    
    ofstream fout(outputFileName.c_str());

	
	while(getline(input, line)) {
            stringstream ss(line);
            ss>>std::hex>>addr>>behavior;
            //cout<<addr<< " ";
            
            totalNum +=1;
// IF TAKEN
            if (behavior == "T") {
              //counter for always taken
              numTaken +=1;
/*------------------------------------------------------------------------
/single bit bimodal (IF TAKEN)
/------------------------------------------------------------------------*/
              //table size 8
              if(bimodalSingleBit8[addr%8] == 0) {
                //single bit misprediction
                bimodalSingleBit8[addr%8] = 1;
                bimodalSingleBitCount8 += 1;

              }
              //table size 16
              if(bimodalSingleBit16[addr%16] == 0) {
                //single bit misprediction
                bimodalSingleBit16[addr%16] = 1;
                bimodalSingleBitCount16 += 1;
              }
              //table size 32
              if(bimodalSingleBit32[addr%32] == 0) {
                //single bit misprediction
                bimodalSingleBit32[addr%32] = 1;
                bimodalSingleBitCount32 += 1;
              }
              //table size 128
              if(bimodalSingleBit128[addr%128] == 0) {
                //single bit misprediction
                bimodalSingleBit128[addr%128] = 1;
                bimodalSingleBitCount128 += 1;
              }
              //table size 256
              if(bimodalSingleBit256[addr%256] == 0) {
                //single bit misprediction
                bimodalSingleBit256[addr%256] = 1;
                bimodalSingleBitCount256 += 1;
              }
              //table size 512
              if(bimodalSingleBit512[addr%512] == 0) {
                //single bit misprediction
                bimodalSingleBit512[addr%512] = 1;
                bimodalSingleBitCount512 += 1;
              }
              //table size 1024
              if(bimodalSingleBit1024[addr%1024] == 0) {
                //single bit misprediction
                bimodalSingleBit1024[addr%1024] = 1;
                bimodalSingleBitCount1024 += 1;
              }
/*------------------------------------------------------------------------
/2-bit bimodal (IF TAKEN)
/------------------------------------------------------------------------*/
              //table size 8
              if(bimodalTwoBit8[addr%8] == 2) {
                //two bit misprediction
                bimodalTwoBit8[addr%8] = 3;
              }
              else if(bimodalTwoBit8[addr%8] == 1) {
                //two bit misprediction
                bimodalTwoBit8[addr%8] = 2;
                bimodalTwoBitCount8 += 1;
              } 
              else if(bimodalTwoBit8[addr%8] == 0) {
                //two bit misprediction
                bimodalTwoBit8[addr%8] = 1;
                bimodalTwoBitCount8 +=1;
              }
              else {
              }
              //table size 16
              if(bimodalTwoBit16[addr%16] == 2) {
                //two bit misprediction
                bimodalTwoBit16[addr%16] = 3;
              }
              else if(bimodalTwoBit16[addr%16] == 1) {
                //two bit misprediction
                bimodalTwoBit16[addr%16] = 2;
                bimodalTwoBitCount16 += 1;
              } 
              else if(bimodalTwoBit16[addr%16] == 0) {
                //two bit misprediction
                bimodalTwoBit16[addr%16] = 1;
                bimodalTwoBitCount16 += 1;
              }
              else {
              }
              //table size 32
              if(bimodalTwoBit32[addr%32] == 2) {
                //two bit misprediction
                bimodalTwoBit32[addr%32] = 3;
              }
              else if(bimodalTwoBit32[addr%32] == 1) {
                //two bit misprediction
                bimodalTwoBit32[addr%32] = 2;
                bimodalTwoBitCount32 += 1;
              } 
              else if(bimodalTwoBit32[addr%32] == 0) {
                //two bit misprediction
                bimodalTwoBit32[addr%32] = 1;
                bimodalTwoBitCount32 += 1;
              }
              else {
              }
              //table size 128
              if(bimodalTwoBit128[addr%128] == 2) {
                //two bit misprediction
                bimodalTwoBit128[addr%128] = 3;
              }
              else if(bimodalTwoBit128[addr%128] == 1) {
                //two bit misprediction
                bimodalTwoBit128[addr%128] = 2;
                bimodalTwoBitCount128 += 1;
              } 
              else if(bimodalTwoBit128[addr%128] == 0) {
                //two bit misprediction
                bimodalTwoBit128[addr%128] = 1;
                bimodalTwoBitCount128 += 1;
              }
              else {
              }
              //table size 256
              if(bimodalTwoBit256[addr%256] == 2) {
                //two bit misprediction
                bimodalTwoBit256[addr%256] = 3;
              }
              else if(bimodalTwoBit256[addr%256] == 1) {
                //two bit misprediction
                bimodalTwoBit256[addr%256] = 2;
                bimodalTwoBitCount256 += 1;
              } 
              else if(bimodalTwoBit256[addr%256] == 0) {
                //two bit misprediction
                bimodalTwoBit256[addr%256] = 1;
                bimodalTwoBitCount256 += 1;
              }
              else {
              }
              //table size 512
              if(bimodalTwoBit512[addr%512] == 2) {
                //two bit misprediction
                bimodalTwoBit512[addr%512] = 3;
              }
              else if(bimodalTwoBit512[addr%512] == 1) {
                //two bit misprediction
                bimodalTwoBit512[addr%512] = 2;
                bimodalTwoBitCount512 += 1;
              } 
              else if(bimodalTwoBit512[addr%512] == 0) {
                //two bit misprediction
                bimodalTwoBit512[addr%512] = 1;
                bimodalTwoBitCount512 += 1;
              }
              else {
              }
              //table size 1024
              if(bimodalTwoBit1024[addr%1024] == 2) {
                //two bit misprediction
                bimodalTwoBit1024[addr%1024] = 3;
              }
              else if(bimodalTwoBit1024[addr%1024] == 1) {
                //two bit misprediction
                bimodalTwoBit1024[addr%1024] = 2;
                bimodalTwoBitCount1024 += 1;
              } 
              else if(bimodalTwoBit1024[addr%1024] == 0) {
                //two bit misprediction
                bimodalTwoBit1024[addr%1024] = 1;
                bimodalTwoBitCount1024 += 1;
              }
              else {
              }
/*------------------------------------------------------------------------
/gshare (IF TAKEN)
/------------------------------------------------------------------------*/
              //2-bit global history
              int index2 = ((addr%1024)^(mask[0] & globalHistReg[0]));
              //update global hist reg
              globalHistReg[0] <<= 1;
              globalHistReg[0] += 1;
              if(gshare2[index2] == 2) {
                gshare2[index2] = 3;
              }
              else if(gshare2[index2] == 1) {
                gshare2[index2] = 2;
                gshareCount2 += 1;
              } 
              else if(gshare2[index2] == 0) {
                gshare2[index2] = 1;
                gshareCount2 += 1;
              }
              else {
              }
              //3-bit global history
              int index3 = ((addr%1024)^(mask[1] & globalHistReg[1]));
              //update global hist reg
              globalHistReg[1] <<= 1;
              globalHistReg[1] += 1;
              if(gshare3[index3] == 2) {
                gshare3[index3] = 3;
              }
              else if(gshare3[index3] == 1) {
                gshare3[index3] = 2;
                gshareCount3 += 1;
              } 
              else if(gshare3[index3] == 0) {
                gshare3[index3] = 1;
                gshareCount3 += 1;
              }
              else {
              }
              //4-bit global history
              int index4 = ((addr%1024)^(mask[2] & globalHistReg[2]));
              //update global hist reg
              globalHistReg[2] <<= 1;
              globalHistReg[2] += 1;
              if(gshare4[index4] == 2) {
                gshare4[index4] = 3;
              }
              else if(gshare4[index4] == 1) {
                gshare4[index4] = 2;
                gshareCount4 += 1;
              } 
              else if(gshare4[index4] == 0) {
                gshare4[index4] = 1;
                gshareCount4 += 1;
              }
              else {
              }
              //5-bit global history
              int index5 = ((addr%1024)^(mask[3] & globalHistReg[3]));
              //update global hist reg
              globalHistReg[3] <<= 1;
              globalHistReg[3] += 1;
              if(gshare5[index5] == 2) {
                gshare5[index5] = 3;
              }
              else if(gshare5[index5] == 1) {
                gshare5[index5] = 2;
                gshareCount5 += 1;
              } 
              else if(gshare5[index5] == 0) {
                gshare5[index5] = 1;
                gshareCount5 += 1;
              }
              else {
              }
              //6-bit global history
              int index6 = ((addr%1024)^(mask[4] & globalHistReg[4]));
              //update global hist reg
              globalHistReg[4] <<= 1;
              globalHistReg[4] += 1;
              if(gshare6[index6] == 2) {
                gshare6[index6] = 3;
              }
              else if(gshare6[index6] == 1) {
                gshare6[index6] = 2;
                gshareCount6 += 1;
              } 
              else if(gshare6[index6] == 0) {
                gshare6[index6] = 1;
                gshareCount6 += 1;
              }
              else {
              }
              //7-bit global history
              int index7 = ((addr%1024)^(mask[5] & globalHistReg[5]));
              //update global hist reg
              globalHistReg[5] <<= 1;
              globalHistReg[5] += 1;
              if(gshare7[index7] == 2) {
                gshare7[index7] = 3;
              }
              else if(gshare7[index7] == 1) {
                gshare7[index7] = 2;
                gshareCount7 += 1;
              } 
              else if(gshare7[index7] == 0) {
                gshare7[index7] = 1;
                gshareCount7 += 1;
              }
              else {
              }
              //8-bit global history
              int index8 = ((addr%1024)^(mask[6] & globalHistReg[6]));
              //update global hist reg
              globalHistReg[6] <<= 1;
              globalHistReg[6] += 1;
              if(gshare8[index8] == 2) {
                gshare8[index8] = 3;
              }
              else if(gshare8[index8] == 1) {
                gshare8[index8] = 2;
                gshareCount8 += 1;
              } 
              else if(gshare8[index8] == 0) {
                gshare8[index8] = 1;
                gshareCount8 += 1;
              }
              else {
              }
              //9-bit global history
              int index9 = ((addr%1024)^(mask[7] & globalHistReg[7]));
              //update global hist reg
              globalHistReg[7] <<= 1;
              globalHistReg[7] += 1;
              if(gshare9[index9] == 2) {
                gshare9[index9] = 3;
              }
              else if(gshare9[index9] == 1) {
                gshare9[index9] = 2;
                gshareCount9 += 1;
              } 
              else if(gshare9[index9] == 0) {
                gshare9[index9] = 1;
                gshareCount9 += 1;
              }
              else {
              }
              //10-bit global history
              int index10 = ((addr%1024)^(mask[8] & globalHistReg[8]));
              //update global hist reg
              globalHistReg[8] <<= 1;
              globalHistReg[8] += 1;
              if(gshare10[index10] == 2) {
                gshare10[index10] = 3;
              }
              else if(gshare10[index10] == 1) {
                gshare10[index10] = 2;
                gshareCount10 += 1;
              } 
              else if(gshare10[index10] == 0) {
                gshare10[index10] = 1;
                gshareCount10 += 1;
              }
              else {
              }
/*------------------------------------------------------------------------
/tournament predictor (IF TAKEN)
/------------------------------------------------------------------------*/
              if (selector[addr%1024] == 0) { //strongly predicts gshare
                //GSHARE for tournament
                int indexT = ((addr%1024)^(mask[8] & globalHistRegT));
                //update global hist reg
                globalHistRegT <<= 1;
                globalHistRegT += 1;
                if(gshareTourn[indexT] == 2) {
                  gshareTourn[indexT] = 3;
                  selectorCounter += 1;
                  gshareCorrect = true;
                }
                else if(gshareTourn[indexT] == 1) {
                  gshareTourn[indexT] = 2;
                  gshareCountT += 1;
                  gshareCorrect = false;
                } 
                else if(gshareTourn[indexT] == 0) {
                  gshareTourn[indexT] = 1;
                  gshareCountT += 1;
                  gshareCorrect = false;
                }
                else {
                  selectorCounter += 1;
                  gshareCorrect = true;
                }
                //2-BIT BIMODAL for tournament
                if(bimodalTourn[addr%1024] == 2) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 3;
                  bimodalCorrect = true;
                }
                else if(bimodalTourn[addr%1024] == 1) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 2;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                } 
                else if(bimodalTourn[addr%1024] == 0) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 1;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                }
                else {
                  bimodalCorrect = true;
                }

                // check if state should change
                if (bimodalCorrect == true && gshareCorrect == false) {
                  selector[addr%1024] = 1;
                }



              }
              else if (selector[addr%1024] == 1) { //weakly predicts gshare
                //GSHARE for tournament
                int indexT = ((addr%1024)^(mask[8] & globalHistRegT));
                //update global hist reg
                globalHistRegT <<= 1;
                globalHistRegT += 1;
                if(gshareTourn[indexT] == 2) {
                  gshareTourn[indexT] = 3;
                  selectorCounter += 1;
                  gshareCorrect = true;
                }
                else if(gshareTourn[indexT] == 1) {
                  gshareTourn[indexT] = 2;
                  gshareCountT += 1;
                  gshareCorrect = false;
                } 
                else if(gshareTourn[indexT] == 0) {
                  gshareTourn[indexT] = 1;
                  gshareCountT += 1;
                  gshareCorrect = false;
                }
                else {
                  selectorCounter += 1;
                  gshareCorrect = true;
                }
                //2-BIT BIMODAL for tournament
                if(bimodalTourn[addr%1024] == 2) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 3;
                  bimodalCorrect = true;
                }
                else if(bimodalTourn[addr%1024] == 1) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 2;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                } 
                else if(bimodalTourn[addr%1024] == 0) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 1;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                }
                else {
                  bimodalCorrect = true;
                }

                // check if state should change
                if (bimodalCorrect == true && gshareCorrect == false) {
                  selector[addr%1024] = 2;
                }
                else if (gshareCorrect == true && bimodalCorrect == false) {
                  selector[addr%1024] = 0;
                }
              }




              else if (selector[addr%1024] == 2) {  //weakly predicts bimodal
                //GSHARE for tournament
                int indexT = ((addr%1024)^(mask[8] & globalHistRegT));
                //update global hist reg
                globalHistRegT <<= 1;
                globalHistRegT += 1;
                if(gshareTourn[indexT] == 2) {
                  gshareTourn[indexT] = 3;
                  gshareCorrect = true;
                }
                else if(gshareTourn[indexT] == 1) {
                  gshareTourn[indexT] = 2;
                  gshareCountT += 1;
                  gshareCorrect = false;
                } 
                else if(gshareTourn[indexT] == 0) {
                  gshareTourn[indexT] = 1;
                  gshareCountT += 1;
                  gshareCorrect = false;
                }
                else {
                  gshareCorrect = true;
                }
                //2-BIT BIMODAL for tournament
                if(bimodalTourn[addr%1024] == 2) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 3;
                  bimodalCorrect = true;
                  selectorCounter += 1;
                }
                else if(bimodalTourn[addr%1024] == 1) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 2;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                } 
                else if(bimodalTourn[addr%1024] == 0) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 1;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                }
                else {
                  bimodalCorrect = true;
                  selectorCounter += 1;
                }

                // check if state should change
                if (bimodalCorrect == true && gshareCorrect == false) {
                  selector[addr%1024] = 3;
                }
                else if (gshareCorrect == true && bimodalCorrect == false) {
                  selector[addr%1024] = 1;
                }
              }




              else { //selector[addr%1024] == 3     strongly predicts bimodal
                //GSHARE for tournament
                int indexT = ((addr%1024)^(mask[8] & globalHistRegT));
                //update global hist reg
                globalHistRegT <<= 1;
                globalHistRegT += 1;
                if(gshareTourn[indexT] == 2) {
                  gshareTourn[indexT] = 3;
                  gshareCorrect = true;
                }
                else if(gshareTourn[indexT] == 1) {
                  gshareTourn[indexT] = 2;
                  gshareCountT += 1;
                  gshareCorrect = false;
                } 
                else if(gshareTourn[indexT] == 0) {
                  gshareTourn[indexT] = 1;
                  gshareCountT += 1;
                  gshareCorrect = false;
                }
                else {
                  gshareCorrect = true;
                }
                //2-BIT BIMODAL for tournament
                if(bimodalTourn[addr%1024] == 2) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 3;
                  bimodalCorrect = true;
                  selectorCounter += 1;
                }
                else if(bimodalTourn[addr%1024] == 1) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 2;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                } 
                else if(bimodalTourn[addr%1024] == 0) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 1;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                }
                else {
                  bimodalCorrect = true;
                  selectorCounter += 1;
                }

                // check if state should change
                if (gshareCorrect == true && bimodalCorrect == false) {
                  selector[addr%1024] = 2;
                }
              }

              //cout <<"Taken" << endl;
            }


// NOT TAKEN //

            else { 
/*------------------------------------------------------------------------
/single bit bimodal (IF NOT TAKEN)
/------------------------------------------------------------------------*/
              //table size 8
              if(bimodalSingleBit8[addr%8] == 1) {
                //single bit misprediction
                bimodalSingleBit8[addr%8] = 0;
                bimodalSingleBitCount8 +=1;
              }
              //table size 16
              if(bimodalSingleBit16[addr%16] == 1) {
                //single bit misprediction
                bimodalSingleBit16[addr%16] = 0;
                bimodalSingleBitCount16 += 1;
              }
              //table size 32
              if(bimodalSingleBit32[addr%32] == 1) {
                //single bit misprediction
                bimodalSingleBit32[addr%32] = 0;
                bimodalSingleBitCount32 += 1;
              }
              //table size 128
              if(bimodalSingleBit128[addr%128] == 1) {
                //single bit misprediction
                bimodalSingleBit128[addr%128] = 0;
                bimodalSingleBitCount128 += 1;
              }
              //table size 256
              if(bimodalSingleBit256[addr%256] == 1) {
                //single bit misprediction
                bimodalSingleBit256[addr%256] = 0;
                bimodalSingleBitCount256 += 1;
              }
              //table size 512
              if(bimodalSingleBit512[addr%512] == 1) {
                //single bit misprediction
                bimodalSingleBit512[addr%512] = 0;
                bimodalSingleBitCount512 += 1;
              }
              //table size 1024
              if(bimodalSingleBit1024[addr%1024] == 1) {
                //single bit misprediction
                bimodalSingleBit1024[addr%1024] = 0;
                bimodalSingleBitCount1024 += 1;
              }
/*------------------------------------------------------------------------
/2-bit bimodal (IF NOT TAKEN)
/------------------------------------------------------------------------*/
              //2-bit bimodal
              //table size 8
              if(bimodalTwoBit8[addr%8] == 3) {
                //two bit misprediction
                bimodalTwoBit8[addr%8] = 2;
                bimodalTwoBitCount8 += 1;
              }
              else if(bimodalTwoBit8[addr%8] == 2) {
                //two bit misprediction
                bimodalTwoBit8[addr%8] = 1;
                bimodalTwoBitCount8 += 1;
              } 
              else if(bimodalTwoBit8[addr%8] == 1) {
                //two bit misprediction
                bimodalTwoBit8[addr%8] = 0;
              }
              else {
              }
              //table size 16
              if(bimodalTwoBit16[addr%16] == 3) {
                //two bit misprediction
                bimodalTwoBit16[addr%16] = 2;
                bimodalTwoBitCount16 += 1;
              }
              else if(bimodalTwoBit16[addr%16] == 2) {
                //two bit misprediction
                bimodalTwoBit16[addr%16] = 1;
                bimodalTwoBitCount16 += 1;
              } 
              else if(bimodalTwoBit16[addr%16] == 1) {
                //two bit misprediction
                bimodalTwoBit16[addr%16] = 0;
              }
              else {
              }
              //table size 32
              if(bimodalTwoBit32[addr%32] == 3) {
                //two bit misprediction
                bimodalTwoBit32[addr%32] = 2;
                bimodalTwoBitCount32 += 1;
              }
              else if(bimodalTwoBit32[addr%32] == 2) {
                //two bit misprediction
                bimodalTwoBit32[addr%32] = 1;
                bimodalTwoBitCount32 += 1;
              } 
              else if(bimodalTwoBit32[addr%32] == 1) {
                //two bit misprediction
                bimodalTwoBit32[addr%32] = 0;
              }
              else {
              }
              //table size 128
              if(bimodalTwoBit128[addr%128] == 3) {
                //two bit misprediction
                bimodalTwoBit128[addr%128] = 2;
                bimodalTwoBitCount128 += 1;
              }
              else if(bimodalTwoBit128[addr%128] == 2) {
                //two bit misprediction
                bimodalTwoBit128[addr%128] = 1;
                bimodalTwoBitCount128 += 1;
              } 
              else if(bimodalTwoBit128[addr%128] == 1) {
                //two bit misprediction
                bimodalTwoBit128[addr%128] = 0;
              }
              else {
              }
              //table size 256
              if(bimodalTwoBit256[addr%256] == 3) {
                //two bit misprediction
                bimodalTwoBit256[addr%256] = 2;
                bimodalTwoBitCount256 += 1;
              }
              else if(bimodalTwoBit256[addr%256] == 2) {
                //two bit misprediction
                bimodalTwoBit256[addr%256] = 1;
                bimodalTwoBitCount256 += 1;
              } 
              else if(bimodalTwoBit256[addr%256] == 1) {
                //two bit misprediction
                bimodalTwoBit256[addr%256] = 0;
              }
              else {
              }
              //table size 512
              if(bimodalTwoBit512[addr%512] == 3) {
                //two bit misprediction
                bimodalTwoBit512[addr%512] = 2;
                bimodalTwoBitCount512 += 1;
              }
              else if(bimodalTwoBit512[addr%512] == 2) {
                //two bit misprediction
                bimodalTwoBit512[addr%512] = 1;
                bimodalTwoBitCount512 += 1;
              } 
              else if(bimodalTwoBit512[addr%512] == 1) {
                //two bit misprediction
                bimodalTwoBit512[addr%512] = 0;
              }
              else {
              }
              //table size 1024
              if(bimodalTwoBit1024[addr%1024] == 3) {
                //two bit misprediction
                bimodalTwoBit1024[addr%1024] = 2;
                bimodalTwoBitCount1024 += 1;
              }
              else if(bimodalTwoBit1024[addr%1024] == 2) {
                //two bit misprediction
                bimodalTwoBit1024[addr%1024] = 1;
                bimodalTwoBitCount1024 += 1;
              } 
              else if(bimodalTwoBit1024[addr%1024] == 1) {
                //two bit misprediction
                bimodalTwoBit1024[addr%1024] = 0;
              }
              else {
              }
/*------------------------------------------------------------------------
/gshare (IF NOT TAKEN)
/------------------------------------------------------------------------*/
              //2-bit global history
              int index2 = (addr%1024)^(mask[0] & globalHistReg[0]);
              //update global hist reg
              globalHistReg[0] <<= 1;
              if(gshare2[index2] == 3) {
                gshare2[index2] = 2;
                gshareCount2 += 1;
              }
              else if(gshare2[index2] == 2) {
                gshare2[index2] = 1;
                gshareCount2 += 1;
              } 
              else if(gshare2[index2] == 1) {
                gshare2[index2] = 0;
              }
              else {
              }
              //3-bit global history
              int index3 = (addr%1024)^(mask[1] & globalHistReg[1]);
              //update global hist reg
              globalHistReg[1] <<= 1;
              if(gshare3[index3] == 3) {
                gshare3[index3] = 2;
                gshareCount3 += 1;
              }
              else if(gshare3[index3] == 2) {
                gshare3[index3] = 1;
                gshareCount3 += 1;
              } 
              else if(gshare3[index3] == 1) {
                gshare3[index3] = 0;
              }
              else {
              }
              //4-bit global history
              int index4 = (addr%1024)^(mask[2] & globalHistReg[2]);
              //update global hist reg
              globalHistReg[2] <<= 1;
              if(gshare4[index4] == 3) {
                gshare4[index4] = 2;
                gshareCount4 += 1;
              }
              else if(gshare4[index4] == 2) {
                gshare4[index4] = 1;
                gshareCount4 += 1;
              } 
              else if(gshare4[index4] == 1) {
                gshare4[index4] = 0;
              }
              else {
              }
              //5-bit global history
              int index5 = (addr%1024)^(mask[3] & globalHistReg[3]);
              //update global hist reg
              globalHistReg[3] <<= 1;
              if(gshare5[index5] == 3) {
                gshare5[index5] = 2;
                gshareCount5 += 1;
              }
              else if(gshare5[index5] == 2) {
                gshare5[index5] = 1;
                gshareCount5 += 1;
              } 
              else if(gshare5[index5] == 1) {
                gshare5[index5] = 0;
              }
              else {
              }
              //6-bit global history
              int index6 = (addr%1024)^(mask[4] & globalHistReg[4]);
              //update global hist reg
              globalHistReg[4] <<= 1;
              if(gshare6[index6] == 3) {
                gshare6[index6] = 2;
                gshareCount6 += 1;
              }
              else if(gshare6[index6] == 2) {
                gshare6[index6] = 1;
                gshareCount6 += 1;
              } 
              else if(gshare6[index6] == 1) {
                gshare6[index6] = 0;
              }
              else {
              }
              //7-bit global history
              int index7 = (addr%1024)^(mask[5] & globalHistReg[5]);
              //update global hist reg
              globalHistReg[5] <<= 1;
              if(gshare7[index7] == 3) {
                gshare7[index7] = 2;
                gshareCount7 += 1;
              }
              else if(gshare7[index7] == 2) {
                gshare7[index7] = 1;
                gshareCount7 += 1;
              } 
              else if(gshare7[index7] == 1) {
                gshare7[index7] = 0;
              }
              else {
              }
              //8-bit global history
              int index8 = (addr%1024)^(mask[6] & globalHistReg[6]);
              //update global hist reg
              globalHistReg[6] <<= 1;
              if(gshare8[index8] == 3) {
                gshare8[index8] = 2;
                gshareCount8 += 1;
              }
              else if(gshare8[index8] == 2) {
                gshare8[index8] = 1;
                gshareCount8 += 1;
              } 
              else if(gshare8[index8] == 1) {
                gshare8[index8] = 0;
              }
              else {
              }
              //9-bit global history
              int index9 = (addr%1024)^(mask[7] & globalHistReg[7]);
              //update global hist reg
              globalHistReg[7] <<= 1;
              if(gshare9[index9] == 3) {
                gshare9[index9] = 2;
                gshareCount9 += 1;
              }
              else if(gshare9[index9] == 2) {
                gshare9[index9] = 1;
                gshareCount9 += 1;
              } 
              else if(gshare9[index9] == 1) {
                gshare9[index9] = 0;
              }
              else {
              }
              //10-bit global history
              int index10 = (addr%1024)^(mask[8] & globalHistReg[8]);
              //update global hist reg
              globalHistReg[8] <<= 1;
              if(gshare10[index10] == 3) {
                gshare10[index10] = 2;
                gshareCount10 += 1;
              }
              else if(gshare10[index10] == 2) {
                gshare10[index10] = 1;
                gshareCount10 += 1;
              } 
              else if(gshare10[index10] == 1) {
                gshare10[index10] = 0;
              }
              else {
              }
/*------------------------------------------------------------------------
/tournament predictor (IF NOT TAKEN)
/------------------------------------------------------------------------*/
              if (selector[addr%1024] == 0) { //strongly predicts gshare
                //GSHARE for tournament
                int indexT = (addr%1024)^(mask[8] & globalHistRegT);
                //update global hist reg
                globalHistRegT <<= 1;
                if(gshareTourn[indexT] == 3) {
                  gshareTourn[indexT] = 2;
                  gshareCountT += 1;
                  gshareCorrect = false;
                }
                else if(gshareTourn[indexT] == 2) {
                  gshareTourn[indexT] = 1;
                  gshareCountT += 1;
                  gshareCorrect = false;
                } 
                else if(gshareTourn[indexT] == 1) {
                  gshareTourn[indexT] = 0;
                  gshareCorrect = true;
                  selectorCounter += 1;
                }
                else {
                  gshareCorrect = true;
                  selectorCounter += 1;
                }
                //2-bit BIMODAL for tournament
                if(bimodalTourn[addr%1024] == 3) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 2;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                }
                else if(bimodalTourn[addr%1024] == 2) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 1;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                } 
                else if(bimodalTourn[addr%1024] == 1) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 0;
                  bimodalCorrect = true;
                }
                else {
                  bimodalCorrect = true;
                }

                // check if state should change
                if (bimodalCorrect == true && gshareCorrect == false) {
                  selector[addr%1024] = 1;
                }




              }
              else if (selector[addr%1024] == 1) { //weakly predicts gshare
                //GSHARE for tournament
                int indexT = (addr%1024)^(mask[8] & globalHistRegT);
                //update global hist reg
                globalHistRegT <<= 1;
                if(gshareTourn[indexT] == 3) {
                  gshareTourn[indexT] = 2;
                  gshareCountT += 1;
                  gshareCorrect = false;
                }
                else if(gshareTourn[indexT] == 2) {
                  gshareTourn[indexT] = 1;
                  gshareCountT += 1;
                  gshareCorrect = false;
                } 
                else if(gshareTourn[indexT] == 1) {
                  gshareTourn[indexT] = 0;
                  gshareCorrect = true;
                  selectorCounter += 1;
                }
                else {
                  gshareCorrect = true;
                  selectorCounter += 1;
                }
                //2-bit BIMODAL for tournament
                if(bimodalTourn[addr%1024] == 3) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 2;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                }
                else if(bimodalTourn[addr%1024] == 2) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 1;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                } 
                else if(bimodalTourn[addr%1024] == 1) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 0;
                  bimodalCorrect = true;
                }
                else {
                  bimodalCorrect = true;
                }

                // check if state should change
                if (bimodalCorrect == true && gshareCorrect == false) {
                  selector[addr%1024] = 2;
                }
                else if (gshareCorrect == true && bimodalCorrect == false) {
                  selector[addr%1024] = 0;
                }
              }



              else if (selector[addr%1024] == 2) { // weakly predicts bimodal
                //GSHARE for tournament
                int indexT = (addr%1024)^(mask[8] & globalHistRegT);
                //update global hist reg
                globalHistRegT <<= 1;
                if(gshareTourn[indexT] == 3) {
                  gshareTourn[indexT] = 2;
                  gshareCountT += 1;
                  gshareCorrect = false;
                }
                else if(gshareTourn[indexT] == 2) {
                  gshareTourn[indexT] = 1;
                  gshareCountT += 1;
                  gshareCorrect = false;
                } 
                else if(gshareTourn[indexT] == 1) {
                  gshareTourn[indexT] = 0;
                  gshareCorrect = true;
                }
                else {
                  gshareCorrect = true;
                }
                //2-bit BIMODAL for tournament
                if(bimodalTourn[addr%1024] == 3) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 2;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                }
                else if(bimodalTourn[addr%1024] == 2) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 1;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                } 
                else if(bimodalTourn[addr%1024] == 1) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 0;
                  bimodalCorrect = true;
                  selectorCounter += 1;
                }
                else {
                  bimodalCorrect = true;
                  selectorCounter += 1;
                }

                // check if state should change
                if (bimodalCorrect == true && gshareCorrect == false) {
                  selector[addr%1024] = 3;
                }
                else if (gshareCorrect == true && bimodalCorrect == false) {
                  selector[addr%1024] = 1;
                }
              }



              else { //strongly predicts bimodal
                //GSHARE for tournament
                int indexT = (addr%1024)^(mask[8] & globalHistRegT);
                //update global hist reg
                globalHistRegT <<= 1;
                if(gshareTourn[indexT] == 3) {
                  gshareTourn[indexT] = 2;
                  gshareCountT += 1;
                  gshareCorrect = false;
                }
                else if(gshareTourn[indexT] == 2) {
                  gshareTourn[indexT] = 1;
                  gshareCountT += 1;
                  gshareCorrect = false;
                } 
                else if(gshareTourn[indexT] == 1) {
                  gshareTourn[indexT] = 0;
                  gshareCorrect = true;
                }
                else {
                  gshareCorrect = true;
                }
                //2-bit BIMODAL for tournament
                if(bimodalTourn[addr%1024] == 3) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 2;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                }
                else if(bimodalTourn[addr%1024] == 2) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 1;
                  bimodalTournCount += 1;
                  bimodalCorrect = false;
                } 
                else if(bimodalTourn[addr%1024] == 1) {
                  //two bit misprediction
                  bimodalTourn[addr%1024] = 0;
                  bimodalCorrect = true;
                  selectorCounter += 1;
                }
                else {
                  bimodalCorrect = true;
                  selectorCounter += 1;
                }

                // check if state should change
                if (gshareCorrect == true && bimodalCorrect == false) {
                  selector[addr%1024] = 2;
                }
              }

              //cout << "Not Taken" << endl;
            }
	}

            //WRITE OUTPUT TO FILE
            //always taken accuracy
	    fout << round(100*numTaken/totalNum) << endl;
            //always not taken accuracy
            fout << round(100 - (100*numTaken/totalNum)) << endl;
            //bimodal single bit accuracy
            fout << round(100 - (100*bimodalSingleBitCount8/totalNum)) << " " << round(100 -  (100*bimodalSingleBitCount16/totalNum)) << " " << round(100 - (100*bimodalSingleBitCount32/totalNum)) << " " << round(100 - (100*bimodalSingleBitCount128/totalNum)) << " " << round(100 - (100*bimodalSingleBitCount256/totalNum)) << " " << round(100 - (100*bimodalSingleBitCount512/totalNum)) << " " << round(100 - (100*bimodalSingleBitCount1024/totalNum)) <<  endl;
            //bimodal 2-bit saturating accuracy
            fout << round(100 - (100*bimodalTwoBitCount8/totalNum)) << " " << round(100 -  (100*bimodalTwoBitCount16/totalNum)) << " " << round(100 - (100*bimodalTwoBitCount32/totalNum)) << " " << round(100 - (100*bimodalTwoBitCount128/totalNum)) << " " << round(100 - (100*bimodalTwoBitCount256/totalNum)) << " " << round(100 - (100*bimodalTwoBitCount512/totalNum)) << " " << round(100 - (100*bimodalTwoBitCount1024/totalNum)) <<  endl;
            //gshare accuracy
            fout << round(100 - (100*gshareCount2/totalNum)) << " " << round(100 -  (100*gshareCount3/totalNum)) << " " << round(100 - (100*gshareCount4/totalNum)) << " " << round(100 - (100*gshareCount5/totalNum)) << " " << round(100 - (100*gshareCount6/totalNum)) << " " << round(100 - (100*gshareCount7/totalNum)) << " " << round(100 - (100*gshareCount8/totalNum)) << " " << round(100 - (100*gshareCount9/totalNum)) << " " << round(100 - (100*gshareCount10/totalNum)) << endl;    
	    //tournament accuracy
            fout << round(100*selectorCounter/totalNum) << endl;

	input.close();
	fout.close();
        delete[] bimodalSingleBit8;
        delete[] bimodalSingleBit16;
        delete[] bimodalSingleBit32;
        delete[] bimodalSingleBit128;
        delete[] bimodalSingleBit256;
        delete[] bimodalSingleBit512;
        delete[] bimodalSingleBit1024;

        delete[] bimodalTwoBit8;
        delete[] bimodalTwoBit16;
        delete[] bimodalTwoBit32;
        delete[] bimodalTwoBit128;
        delete[] bimodalTwoBit256;
        delete[] bimodalTwoBit512;
        delete[] bimodalTwoBit1024;
	delete[] bimodalTourn;

        delete[] gshare2;
        delete[] gshare3;
        delete[] gshare4;
        delete[] gshare5;
        delete[] gshare6;
        delete[] gshare7;
        delete[] gshare8;
        delete[] gshare9;
        delete[] gshare10;
        delete[] gshareTourn;

  }
  else {
	cout<< "Input file was not found." << endl;
  }
}




