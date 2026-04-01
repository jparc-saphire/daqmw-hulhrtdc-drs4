/*
  DetectorID.hh
*/

#ifndef DetectorID_h
#define DetectorID_h 1

//T0 
const int NumOfSegT0 =  16;
//const int NumOfCell = 1024;
//def up to 2024/6/1
const int NumOfCell = 1022;
//does not work const int NumOfCell = 2048;

//sako 2024/4/6
const int NumOfModule = 3; // no. of HUL-HRTDC card

const int NumOfHRTDC = 64;

const int MaxHits = 16;

const double Tdc2Time = 0.002604;
const double Tdc2Time2 = 0.0009390024;

#endif
