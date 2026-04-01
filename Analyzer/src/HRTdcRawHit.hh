/*
  HRTdcRawHit.hh
  
  2019/12  K.Shirotori
*/
#define E16DAQ
#ifndef HRTdcRawHit_h 
#define HRTdcRawHit_h

#include <cstddef>
#include <vector>

typedef std::vector <double> DoubleVec;
typedef std::vector <unsigned int> uIntVec;

class HRTdcRawHit
{
private:
  int DetId_, LayerId_, SegId_, UorD_;
  int EvNum_;
  uIntVec lTdc_, tTdc_;
#ifdef E16DAQ
  int spillID_, eventID0_, eventID1_, eventID2_;
#endif
  
public:
  HRTdcRawHit( int detid, int layerid, int segid )
    : DetId_(detid), LayerId_(layerid), SegId_(segid),
      EvNum_(),
      UorD_(), 
#ifdef E16DAQ
      lTdc_(), tTdc_(),
      spillID_(0), eventID0_(0), eventID1_(0), eventID2_(0)
#else
      lTdc_(), tTdc_()
#endif
  {};
  ~HRTdcRawHit() {};
  
public:
  int DetectorId( void ) const { return DetId_; };
  int LayerId( void ) const { return LayerId_; };
  int SegmentId( void ) const { return SegId_; };

  void SetEvNum( int evnum ) { EvNum_=evnum; }
  int GetEvNum( void ) const { return EvNum_; }

  void SetUorD( int uord ) { UorD_=uord; }
  int GetUorD( void ) const { return UorD_; }

  void SetlTdc( uIntVec ltdc ) { lTdc_=ltdc; }
  void SettTdc( uIntVec ttdc ) { tTdc_=ttdc; }

#ifdef E16DAQ
  void SetSpill( int spill ) { spillID_=spill; }
  void SetEvent0( int eventId0 ) { eventID0_=eventId0; }
  void SetEvent1( int eventId1 ) { eventID1_=eventId1; }
  void SetEvent2( int eventId2 ) { eventID2_=eventId2; }

  int GetSpill( void ) const { return spillID_; };
  int GetEvent0( void ) const { return eventID0_; };
  int GetEvent1( void ) const { return eventID1_; };
  int GetEvent2( void ) const { return eventID2_; };
#endif

  double GetlTdc( int nh ) const { return lTdc_[nh]; };
  double GettTdc( int nh ) const { return tTdc_[nh]; };

  int  GetSize_lTdc( void ) const { return lTdc_.size(); };
  int  GetSize_tTdc( void ) const { return tTdc_.size(); };
};
#endif
