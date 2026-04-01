/*
  RawData.cc

  2018/10  K.Shirotori
*/
#define E16DAQ

#include "RawData.hh"
#include "GetNumberFromKernelEntropyPool.hh"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <iomanip>
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstdlib>
#include <string.h>

#include "Decoder.hh"
#include "Common_val_drs4.hh"
#include "Datadrs.hh"
#include "DetectorID.hh"
#include "HodoRawHit.hh"
#include "HRTdcRawHit.hh"
#include "TemplateLib.hh"

#include "ConfMan.hh"
#include "CMapMan.hh"

const double Deg2Rad = acos(-1.)/180.;
const double Rad2Deg = 180./acos(-1.);

RawData::RawData():
  T0RHC(0), HRTRHC(0)
{}

RawData::~RawData()
{
 clearAll();
}

bool RawData::AddHodoRHit( HodoRHitContainer& cont,
			   int DetId, int Layer, int Seg, int UorD, 
			   int EventNum,
			   DoubleVec Waveform, 
			   DoubleVec Adc, DoubleVec Amp, 
			   DoubleVec Bl, DoubleVec PeakX,
			   uIntVec Tdc, uIntVec Dt, 
			   uIntVec Tdc_2nd, uIntVec Dt_2nd,
			   uIntVec Width, 
			   unsigned int L1_Tdc, unsigned int L1_Tdc1 )
{
  static const std::string funcname = "[RawData::AddHodoRHit]";

  HodoRawHit *p=0;
  int nh=cont.size();
  for( int i=0; i<nh; ++i ){
    HodoRawHit *q=cont[i];
    if( q->DetectorId()==DetId &&
        q->LayerId()==Layer &&
        q->SegmentId()==Seg ){
      p=q; break;
    }
  }
  if(!p){
    p = new HodoRawHit( DetId, Layer, Seg );
    if(p) cont.push_back(p);
  }
  if(p){
    p->SetEvNum( EventNum );
    p->SetUorD( UorD );
    p->SetWaveform( Waveform );
    p->SetAdc( Adc );
    p->SetAmp( Amp );
    p->SetBl( Bl );
    p->SetPeakX( PeakX );
    p->SetTdc( Tdc );
    p->SetDt( Dt );
    p->SetTdc_2nd( Tdc_2nd );
    p->SetDt_2nd( Dt_2nd );
    p->SetWidth( Width );
    p->SetL1_Tdc( L1_Tdc );
    p->SetL1_Tdc1( L1_Tdc1 );

    //std::cout<< "Ch=" << Seg << " :TDC=" << Tdc.size() << std::endl;

    return true;
  }else{
    std::cerr << funcname << ": new fail." << std::endl;
    return false;
  }
}

#ifdef E16DAQ
bool RawData::AddHRTdcRHit( HRTdcRHitContainer& cont,
			    int DetId, int Layer, int Seg, int UorD, 
			    int EventNum,
			    uIntVec lTdc, uIntVec tTdc,
			    int spillId, int eventId0, int eventId1, int eventId2)
#else
bool RawData::AddHRTdcRHit( HRTdcRHitContainer& cont,
			    int DetId, int Layer, int Seg, int UorD, 
			    int EventNum,
			    uIntVec lTdc, uIntVec tTdc )
#endif
{
  static const std::string funcname = "[RawData::AddHRTdcRHit]";
  
  HRTdcRawHit *p=0;
  int nh=cont.size();
  for( int i=0; i<nh; ++i ){
    HRTdcRawHit *q=cont[i];
    if( q->DetectorId()==DetId &&
        q->LayerId()==Layer &&
        q->SegmentId()==Seg ){
      p=q; break;
    }
  }
  if(!p){
    p = new HRTdcRawHit( DetId, Layer, Seg );
    if(p) cont.push_back(p);
  }
  if(p){
    p->SetEvNum( EventNum );
    p->SetUorD( UorD );
    p->SetlTdc( lTdc );
    p->SettTdc( tTdc );

#ifdef E16DAQ
    p->SetSpill( spillId );
    p->SetEvent0( eventId0 );
    p->SetEvent1( eventId1 );
    p->SetEvent2( eventId2 );
#endif

    //std::cout<< "Ch=" << Seg << " :TDC=" << lTdc.size() << std::endl;

    return true;
  }else{
    std::cerr << funcname << ": new fail." << std::endl;
    return false;
  }
}

void RawData::clearAll()
{
  std::for_each(T0RHC.begin(), T0RHC.end(), DeleteObject());
  T0RHC.clear();

  std::for_each(HRTRHC.begin(), HRTRHC.end(), DeleteObject());
  HRTRHC.clear();

  return;
}

bool RawData::DecodeRawHits( Decoder& gDec )
{
  clearAll();

  ConfMan *confMan = ConfMan::GetConfManager();
  CMapMan *mapMan=ConfMan::GetConfManager()->GetCMapManager();

  /////////////////////////////////////////////////////////
  //T0 decoding (DRS4)
  DRS::dataDrs sDRSData;
  gDec.decode(sDRSData);
  gDec.decodeADC(sDRSData);
  gDec.decodeTDC(sDRSData);

  //std::cout<< "DRS4 Event Num = " << sDRSData.eventNum << std::endl;

  for(int ich = 0; ich<DRS::NofChModule*DRS::NofBoards; ++ich){
    int Caddr = 1, Naddr = 2, Aaddr = -1;
    Aaddr = ich;
    int DetId, PlId, SegId, UorD;
    bool status;
    
    status = mapMan->GetLogical(Caddr,Naddr,Aaddr,DetId,PlId,SegId,UorD);
    if(status){
      AddHodoRHit( T0RHC, DetId, PlId, SegId, UorD, 
		   sDRSData.eventNum,
		   sDRSData.data_wf[ich],
		   sDRSData.data_adc[ich], sDRSData.data_amp[ich], 
		   sDRSData.data_bl[ich], sDRSData.data_peakx[ich], 
		   sDRSData.data_tdc[ich], sDRSData.data_dt[ich], 
		   sDRSData.data_tdc_2nd[ich], sDRSData.data_dt_2nd[ich], 
		   sDRSData.data_width[ich], 
		   sDRSData.l1_tdc[0], sDRSData.l1_tdc1[0] );
    }
  
#if 0
      std::cout << "***********************" << std::endl;
      std::cout << "Caddr=" << Caddr
		<< " Naddr=" << Naddr
		<< " Aaddr=" << Aaddr
		<< " --> "
		<< "DetId=" << DetId 
		<< " PlId=" << PlId 
		<< " SegId=" << SegId
		<< " UorD=" << UorD 
		<< std::endl;
#endif
  }

  /////////////////////////////////////////////////////////
  //T0 decoding (HR-TDC)
  MZN_HRTDC::Data_t MZNData;
  MZNData = gDec.GetMznHRTdcData();
  //sako 2021/2/17
  //  const MZN_HRTDC::Data_t &MZNData = gDec.GetMznHRTdcData();

  std::vector<std::vector<unsigned int> > HRTdcLeading;
  std::vector<std::vector<unsigned int> > HRTdcTrailing;

  //2024/4/23 Sako
  //Look wrong. It is for 1 module
  /*
  HRTdcLeading.resize(NumOfHRTDC);
  HRTdcTrailing.resize(NumOfHRTDC);

  //std::cout<< "MZN Event Num = " << MZNData.EventNum << std::endl;

  if( MZNData.Nhit>0 || MZNData.tNhit>0 ){
    for( int ich=0; ich<NumOfHRTDC; ich++ ){
      int Caddr = 1, Naddr = 3, Aaddr = -1;
      Aaddr = ich;
      int DetId, PlId, SegId, UorD;
      bool status;

      for( int j=0; j<MZNData.Nhit; j++ ){
	if( ich==MZNData.Ch[j] ){
	  HRTdcLeading[ich].push_back(MZNData.tdc_leading[j]);
	}
      }
      for( int j=0; j<MZNData.tNhit; j++ ){
	if( ich==MZNData.tCh[j] ){
	  HRTdcTrailing[ich].push_back(MZNData.tdc_trailing[j]);
	}
      }
      
      status = mapMan->GetLogical(Caddr,Naddr,Aaddr,DetId,PlId,SegId,UorD);
      
      if(status){
	AddHRTdcRHit( HRTRHC, DetId, PlId, SegId, UorD, 
		      MZNData.EventNum,
		      HRTdcLeading[ich], HRTdcTrailing[ich]);
#if 0
      std::cout << "***********************" << std::endl;
      std::cout << "Caddr=" << Caddr
		<< " Naddr=" << Naddr
		<< " Aaddr=" << Aaddr
		<< " --> "
		<< "DetId=" << DetId 
		<< " PlId=" << PlId 
		<< " SegId=" << SegId
		<< " UorD=" << UorD 
		<< std::endl;
#endif
      }
    }
  }
  */

  //2024/4/23 copied from HRTDC_run0e_local
  const int ntdc=1;//don't change from 1
  HRTdcLeading.resize(NumOfHRTDC*NumOfModule);
  HRTdcTrailing.resize(NumOfHRTDC*NumOfModule);
  //  HRTdcWidth.resize(NumOfHRTDC*NumOfModule);
#ifdef E16DAQ
  int Spill = MZNData.spillID;
  int EventID0 = MZNData.eventID[0];
  int EventID1 = MZNData.eventID[1];
  int EventID2 = MZNData.eventID[2];
  spillID = MZNData.spillID;
  eventID[0] = EventID0;
  eventID[1] = EventID1;
  eventID[2] = EventID2;
  MZNEventNumber[0] = MZNData.MZN_event_number[0];
  MZNEventNumber[1] = MZNData.MZN_event_number[1];
  MZNEventNumber[2] = MZNData.MZN_event_number[2];
#endif
  for (int itdc=0;itdc<ntdc;itdc++) {
  
    if( MZNData.Nhit>0 ){
      for( int ich=0; ich<NumOfHRTDC*NumOfModule; ich++ ){
	//	int Caddr = 1, Naddr = 1, Aaddr = -1;
	int Caddr = 1;
	int Naddr = ich/NumOfHRTDC+1;
	int Aaddr = ich%NumOfHRTDC;
	int DetId, PlId, SegId, UorD;
	bool status;

	//	if (ich>=NumOfHRTDC) std::cout << "ich = " << ich << std::endl;
	for( int j=0; j<MZNData.Nhit; j++ ){
	  //	  if (ich>=NumOfModule) std::cout << "MZNData.Ch["<<j<<"] = " << MZNData.Ch[j] << std::endl;
	  if( ich==MZNData.Ch[j] ){
	    HRTdcLeading[ich].push_back(MZNData.tdc_leading[j]);
	    if (ich>=NumOfHRTDC) {
	      //	      std::cout << "########### FOUND ich, leading = " << ich << " " << MZNData.tdc_leading[j] << std::endl;
	    }
	    //std::cout << MZNData.Ch[j] << " = "  << MZNData.tdc_leading[j] << std::endl;

	    //2021/2/17 sako
	    
	    //	    if ((48<=MZNData.Ch[j])&&(MZNData.Ch[j]<=50)) {
	    //	      std::cout << "t["<< MZNData.Ch[j] << "] = " << MZNData.tdc_leading[j]*Tdc2Time << std::endl;
	    //	      std::cout << "MZNData.Nhit = " << MZNData.Nhit << std::endl;
	    //	      std::cout << "j = " << j << std::endl;
	    //	    }
	//	std::cout << "index_slot, imod, ch = " << index_slot << " " << imod << " " << sMZNData.Ch[sMZNData.Nhit] << std::endl;
	//	std::cout << "((rdata >> MZN_HRTDC::shift_ch)  & MZN_HRTDC::mask_ch)" << ((rdata >> MZN_HRTDC::shift_ch)  & MZN_HRTDC::mask_ch) << std::endl;

	  }
	}
	for( int j=0; j<MZNData.tNhit; j++ ){
	  if( ich==MZNData.tCh[j] ){
	    HRTdcTrailing[ich].push_back(MZNData.tdc_trailing[j]);
	    if (ich>=NumOfHRTDC) {
	      //	      	      std::cout << "########### FOUND ich, trailing = " << ich << " " << MZNData.tdc_trailing[j] << std::endl;
	    }
	  }
	}
	
	if( HRTdcLeading[ich].size()>0 ){
	  int size_ltdc = HRTdcLeading[ich].size();
	  int size_ttdc = HRTdcTrailing[ich].size();
	  for( int ilt=0; ilt<size_ltdc; ilt++ ){
	    int ltdc = HRTdcLeading[ich][ilt];
	    for( int itt=0; itt<size_ttdc; itt++ ){
	      int ttdc = HRTdcTrailing[ich][itt];
	      //	      HRTdcWidth[ich].push_back(ltdc-ttdc);
	    }
	  }
	}
      
	
      status = mapMan->GetLogical(Caddr,Naddr,Aaddr,DetId,PlId,SegId,UorD);
      //      std::cout << "**ich, Caddr,Naddr,Aaddr = " << ich << " " << Caddr << " " << Naddr << " " << Aaddr << std::endl;
      //      std::cout << "**DetId,PlId,SegId,pUorD =  " << DetId << " " << PlId << " " << SegId << " " << UorD << std::endl;
      //      if(status){
	//	AddHodoRHit( T0RHC, DetId, PlId, SegId, UorD, 
	//		     HRTdcLeading[ich], HRTdcTrailing[ich], HRTdcWidth[ich] );


	//to be checked
	     if(status){
#ifdef E16DAQ
		AddHRTdcRHit( HRTRHC, DetId, PlId, SegId, UorD, 
			      MZNData.EventNum,
			      HRTdcLeading[ich], HRTdcTrailing[ich],
			      Spill, EventID0, EventID1, EventID2);
#else
		AddHRTdcRHit( HRTRHC, DetId, PlId, SegId, UorD, 
			      MZNData.EventNum,
			      HRTdcLeading[ich], HRTdcTrailing[ich]);
#endif       
	
	
#if 0
      std::cout << "***********************" << std::endl;
      std::cout << "Caddr=" << Caddr
		<< " Naddr=" << Naddr
		<< " Aaddr=" << Aaddr
		<< " --> "
		<< "DetId=" << DetId 
		<< " PlId=" << PlId 
		<< " SegId=" << SegId
		<< " UorD=" << UorD
		<< " status=" << status
		<< std::endl;
#endif
      //      }
         }
      }
    }
  }

  return true;
}

const HodoRHitContainer& RawData::GetT0RHC() const
{
  return T0RHC;
}

const HRTdcRHitContainer& RawData::GetHRTRHC() const
{
  return HRTRHC;
}

