/*
  RawData.hh

  2018/10  K.Shirotori
*/

#define E16DAQ

#ifndef RawData_h
#define RawData_h

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>

#include "DetectorID.hh"
#include <vector>

class Decoder;

class HodoRawHit;
class HRTdcRawHit;

typedef std::vector<HodoRawHit*> HodoRHitContainer;
typedef std::vector<HRTdcRawHit*> HRTdcRHitContainer;

typedef std::vector <double> DoubleVec;
typedef std::vector <unsigned int> uIntVec;
typedef std::vector <int> IntVec;

class RawData
{

private:
  HodoRHitContainer T0RHC;
  HRTdcRHitContainer HRTRHC;

public:
  RawData();
  ~RawData();

  void clearAll();
  bool DecodeRawHits( Decoder& gDec );

private:
  RawData(const RawData&);
  RawData& operator=(const RawData&);

  bool AddHodoRHit( HodoRHitContainer& cont,
		    int DetId, int Layer, int Seg, int UorD,
		    int EventNum,
		    DoubleVec Waveform,
		    DoubleVec Adc, DoubleVec Amp, 
		    DoubleVec Bl, DoubleVec PeakX,
		    uIntVec Tdc, uIntVec Dt, 
		    uIntVec Tdc_2nd, uIntVec Dt_2nd,
		    uIntVec Width, 
		    unsigned int L1_Tdc, unsigned int L1_Tdc1 
		    );

#ifdef E16DAQ
  bool AddHRTdcRHit( HRTdcRHitContainer& cont,
		     int DetId, int Layer, int Seg, int UorD,
		     int EventNum,
		     uIntVec lTdc, uIntVec tTdc,
		     int spillId, int eventId0, int eventId1, int eventId2);
  int spillID;
  int eventID[3];
  //sako 2024.4.19
  //def  int MZNEventNumber[2];
  int MZNEventNumber[3];

#else
  bool AddHRTdcRHit( HRTdcRHitContainer& cont,
		     int DetId, int Layer, int Seg, int UorD,
		     int EventNum,
		     uIntVec lTdc, uIntVec tTdc
		     );
#endif

public:
  const HodoRHitContainer& GetT0RHC() const;
  const HRTdcRHitContainer& GetHRTRHC() const;
#ifdef E16DAQ
  int getSpillID() {
    return spillID;
  }
  int *getEventID() {
    return eventID;
  }
  int *getMZNEventNumber() {
    return MZNEventNumber;
  }
#endif
};

#endif

