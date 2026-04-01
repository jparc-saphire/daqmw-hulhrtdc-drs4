/*
  UserMonitor.cc

  2018/10 K.Shirotori 
*/
#define E16DAQ
#include "VEvent.hh"

#include <iostream>
#include <sstream>
#include <cmath>
#include <string>
#include <iomanip>
#include <vector>

#include "ConfMan.hh"
#include "Decoder.hh"
#include "RootHelper.hh"
#include "DetectorID.hh"
#include "RawData.hh"
#include "HodoRawHit.hh"
#include "HRTdcRawHit.hh"

#include "TFile.h"
#include "TTree.h"

#define check1 0

 VEvent::VEvent()
 {
 }

 VEvent::~VEvent()
 {
 }

class EventMonitor : public VEvent
{
public:
  EventMonitor();
  ~EventMonitor();

  bool ProcessingBegin();
  bool ProcessingEnd();
  bool ProcessingNormal( Decoder& gDec );
  void InitializeEvent();

private:
  RawData *rawData;

};

EventMonitor::EventMonitor()
  : VEvent(),
    rawData(0)
{
}

EventMonitor::~EventMonitor()
{
  if (rawData) delete rawData;
}

struct Event{
#ifdef E16DAQ
  int spillID, eventID[3], event_number, MZN_event_number[3];
  int nh_all;
#endif
  //T0
  std::vector<std::vector<double> > WaveForm;

  std::vector<std::vector<double> > adc;
  std::vector<std::vector<double> > amp;
  std::vector<std::vector<double> > bl;
  std::vector<std::vector<double> > peakx;

  std::vector<std::vector<double> > tdc;
  std::vector<std::vector<double> > dt;
  std::vector<std::vector<double> > tdc_2nd;
  std::vector<std::vector<double> > dt_2nd;
  std::vector<std::vector<double> > width;

  int l1_tdc, l1_tdc1;
  //sako 2024/4/6
  int nhltdc[NumOfModule][NumOfHRTDC], nhttdc[NumOfModule][NumOfHRTDC];
  std::vector<std::vector<double> > ltdc[NumOfModule];
  std::vector<std::vector<double> > ttdc[NumOfModule];
  std::vector<std::vector<double> > ltdc1[NumOfModule];
  std::vector<std::vector<double> > ttdc1[NumOfModule];
};
static Event event;

bool EventMonitor::ProcessingBegin()
{
 return true;
}

bool EventMonitor::ProcessingNormal( Decoder& gDec )
{
  const std::string funcname = "ProcessingNormal";

  rawData = new RawData;
  if( !rawData->DecodeRawHits( gDec ) ) return false;
  //std::cout << "***" << std::endl;

  ConfMan *confMan = ConfMan::GetConfManager();

  //**************************************************************************
  //******************RawData
  TTree *tree = static_cast<TTree *>(gFile->Get("tree"));
  InitializeEvent();
  
  //**************************************************************************
  //DRS4
  {
#if check1
    std::cout << "DRS4***********************" << std::endl;
#endif
    event.WaveForm.resize(NumOfCell);

    event.adc.resize(NumOfSegT0);
    event.amp.resize(NumOfSegT0);
    event.bl.resize(NumOfSegT0);
    event.peakx.resize(NumOfSegT0);
    
    event.tdc.resize(NumOfSegT0);
    event.dt.resize(NumOfSegT0);
    event.tdc_2nd.resize(NumOfSegT0);
    event.dt_2nd.resize(NumOfSegT0);
    event.width.resize(NumOfSegT0);

    const HodoRHitContainer &cont =rawData->GetT0RHC();
    int nh=cont.size();
#ifdef E16DAQ
    event.nh_all = nh;
    event.spillID = rawData->getSpillID();
    int *EventId = rawData->getEventID();
    event.eventID[0] = EventId[0];
    event.eventID[1] = EventId[1];
    event.eventID[2] = EventId[2];
    event.event_number = EventId[0] + EventId[1]*2 + EventId[2]*4;
    int *MZNEventNumber = rawData->getMZNEventNumber();
    event.MZN_event_number[0] = MZNEventNumber[0];
    event.MZN_event_number[1] = MZNEventNumber[1];
    event.MZN_event_number[2] = MZNEventNumber[2];
    //    std::cout << "UserMonitor, event.event_number = " << event.event_number << std::endl;
#endif
    for( int i=0; i<nh; ++i ){
      HodoRawHit *hit=cont[i];

      int detectorId = hit->DetectorId();
      int layerId = hit->LayerId()+1;
      int segId = hit->SegmentId()+1;
      int UorD = hit->GetUorD();
      int l1_tdc = hit -> GetL1_Tdc();
      int l1_tdc1 = hit -> GetL1_Tdc1();

#ifdef E16DAQ
    //---------------------------------------------------------------
      /*2024/4/26 Sako commented out
      int spillId = hit->GetSpill();
      event.spillID = spillId;
      int EventId0 = hit->GetEvent0();
      event.eventID[0] = EventId0;
      int EventId1 = hit->GetEvent1();
      event.eventID[1] = EventId1;
      int EventId2 = hit->GetEvent2();
      event.eventID[2] = EventId2;
      int EventNumber = EventId0 + EventId1*2 + EventId2*4;
      event.event_number = EventNumber;
      */
#endif

      if( confMan->T0SimpleAna() ){
	//ADC
	int sizeAdc = hit->GetSize_Amp();
	for( int ia=0; ia<sizeAdc; ++ia ){
	  double amp = hit -> GetAmp(ia);
	  event.amp[segId-1].push_back(amp);
	}
	//TDC
	int sizeDt = hit->GetSize_Dt();
	for( int it=0; it<sizeDt; ++it ){
	  int dt = hit -> GetDt(it);
	  event.dt[segId-1].push_back(double(dt)*Tdc2Time);
	}
	int sizeWidth = hit->GetSize_Width();
	for( int it=0; it<sizeWidth; ++it ){
	  int width = hit -> GetWidth(it);
	  event.width[segId-1].push_back(double(width)*Tdc2Time);
	}
	event.l1_tdc = l1_tdc*Tdc2Time;
	event.l1_tdc1 = l1_tdc1*Tdc2Time;
      }
      else{
	//Waveform
	int sizeWf = hit->GetSize_Wf();
	for( int iwf=0; iwf<sizeWf; ++iwf ){
	  double wf = hit -> GetWaveform(iwf);
	  //bug 2023/3/9	  if(segId-1==0) event.WaveForm[segId-1].push_back(wf);
	  //sako
	  if ((1<=segId)&&(segId<=NumOfSegT0)) {
	    event.WaveForm[segId-1].push_back(wf);
	  }
	}
	//ADC
	int sizeAdc = hit->GetSize_Amp();
	for( int ia=0; ia<sizeAdc; ++ia ){
	  double adc = hit -> GetAdc(ia);
	  double amp = hit -> GetAmp(ia);
	  double bl = hit -> GetBl(ia);
	  double peakx = hit -> GetPeakX(ia);
	  
	  event.adc[segId-1].push_back(adc);
	  event.amp[segId-1].push_back(amp);
	  event.bl[segId-1].push_back(bl);
	  event.peakx[segId-1].push_back(peakx);
	  //	  std::cout << "segId-1, adc, amp, bl = " << segId-1 << " " <<  adc << " " << amp << " " <<  bl << std::endl; 
	}
	
	//TDC
	int sizeTdc = hit->GetSize_Tdc();
	for( int it=0; it<sizeTdc; ++it ){
	  int tdc = (hit -> GetTdc(it));
	  event.tdc[segId-1].push_back(double(tdc)*Tdc2Time);
	}
	int sizeDt = hit->GetSize_Dt();
	for( int it=0; it<sizeDt; ++it ){
	  int dt = hit -> GetDt(it);
	  event.dt[segId-1].push_back(double(dt)*Tdc2Time);
	}
	int sizeTdc_2nd = hit->GetSize_Tdc_2nd();
	for( int it=0; it<sizeTdc_2nd; ++it ){
	  int tdc_2nd = hit -> GetTdc_2nd(it);
	  event.tdc_2nd[segId-1].push_back(double(tdc_2nd)*Tdc2Time);
	}
	int sizeDt_2nd = hit->GetSize_Dt_2nd();
	for( int it=0; it<sizeDt_2nd; ++it ){
	  int dt_2nd = hit -> GetDt_2nd(it);
	  event.dt_2nd[segId-1].push_back(double(dt_2nd)*Tdc2Time);
	}
	int sizeWidth = hit->GetSize_Width();
	for( int it=0; it<sizeWidth; ++it ){
	  int width = hit -> GetWidth(it);
	  event.width[segId-1].push_back(double(width)*Tdc2Time);
	}
	event.l1_tdc = l1_tdc*Tdc2Time;
	event.l1_tdc1 = l1_tdc1*Tdc2Time;
      }
      
#if check1
      std::cout << "Layer = " << layerId 
		<< " Seg = " << segId 
		<< std::endl;
#endif
      
    }
  }

  //**************************************************************************
  //HRTdc
  {
#if check1
    std::cout << "HRTdc***********************" << std::endl;
#endif
    const int t0tdclow = confMan->T0RangeLow();
    const int t0tdchigh = confMan->T0RangeHigh();
    
    //    event.ltdc.resize(NumOfHRTDC);
    //    event.ttdc.resize(NumOfHRTDC);
    //    event.ltdc1.resize(NumOfHRTDC);
    //    event.ttdc1.resize(NumOfHRTDC);
    for (int imod=0;imod<NumOfModule;imod++) {
      event.ltdc[imod].resize(NumOfHRTDC);
      event.ttdc[imod].resize(NumOfHRTDC);
      //sako 2024/4/15
      event.ltdc1[imod].resize(NumOfHRTDC);
      event.ttdc1[imod].resize(NumOfHRTDC);
    }
    
    const HRTdcRHitContainer &cont =rawData->GetHRTRHC();
    int nh=cont.size();
    for( int i=0; i<nh; ++i ){
      HRTdcRawHit *hit=cont[i];

      int detectorId = hit->DetectorId();
      int layerId = hit->LayerId()+1;
      int segId = hit->SegmentId()+1;
      int UorD = hit->GetUorD();
      
      if( confMan->T0SimpleAna() ){
	//HR-TDC
	double ltdc_1st = -1.0;
	int sizelTdc = hit->GetSize_lTdc();
	for( int it=0; it<sizelTdc; ++it ){
	  unsigned int ltdc0 = hit -> GetlTdc(it);
	  double ltdc = double(ltdc0)*Tdc2Time2;
	  //def	  event.ltdc[segId-1].push_back(ltdc);
	  //sako 2024/4/6
	  event.ltdc[layerId-1][segId-1].push_back(ltdc);
	  if( t0tdclow<ltdc && ltdc<t0tdchigh && ltdc>ltdc_1st ) ltdc_1st = ltdc;
	}
	//def	event.ltdc1[segId-1].push_back(ltdc_1st);
	//sako 2024/4/6
	event.ltdc1[layerId-1][segId-1].push_back(ltdc_1st);
	
	double ttdc_1st = -1.0;
	int sizetTdc = hit->GetSize_tTdc();
	for( int it=0; it<sizetTdc; ++it ){
	  unsigned int ttdc0 = hit -> GettTdc(it);
	  double ttdc = double(ttdc0)*Tdc2Time2;
	  //def	  event.ttdc[segId-1].push_back(ttdc);
	  //sako 2024/4/6
	  event.ttdc[layerId-1][segId-1].push_back(ttdc);
	  if( t0tdclow<ttdc && ttdc<t0tdchigh && ttdc>ttdc_1st) ttdc_1st = ttdc;
	}
	//def	event.ttdc1[segId-1].push_back(ttdc_1st);
	//2024/4/6
	event.ttdc1[layerId-1][segId-1].push_back(ttdc_1st);

	//def	event.nhltdc[segId-1]=sizelTdc;
	//def	event.nhttdc[segId-1]=sizetTdc;
	//sako 2024/4/6
	event.nhltdc[layerId-1][segId-1]=sizelTdc;
	event.nhttdc[layerId-1][segId-1]=sizetTdc;
      }
      else{
	//HR-TDC
	double ltdc_1st = -1.0;
	int sizelTdc = hit->GetSize_lTdc();
	for( int it=0; it<sizelTdc; ++it ){
	  unsigned int ltdc0 = hit -> GetlTdc(it);
	  double ltdc = double(ltdc0)*Tdc2Time2;
	  //def	  event.ltdc[segId-1].push_back(ltdc);
	  //sako 2024/4/6
	  event.ltdc[layerId-1][segId-1].push_back(ltdc);
	  if( t0tdclow<ltdc && ltdc<t0tdchigh && ltdc>ltdc_1st ) ltdc_1st = ltdc;
	}
	//def	event.ltdc1[segId-1].push_back(ltdc_1st);
	//	std::cout << "UserMonitor, layerId-1, segId-1, ltdc_1st = " << layerId-1 << " " << segId-1 << " " << ltdc_1st << std::endl;
	event.ltdc1[layerId-1][segId-1].push_back(ltdc_1st);
	//std::cout<< "ltdc= " << ltdc_1st << std::endl;	
	double ttdc_1st = -1.0;
	int sizetTdc = hit->GetSize_tTdc();
	for( int it=0; it<sizetTdc; ++it ){
	  unsigned int ttdc0 = hit -> GettTdc(it);
	  double ttdc = double(ttdc0)*Tdc2Time2;
	  //def	  event.ttdc[segId-1].push_back(ttdc);
	  //sako 2024/4/6
	  event.ttdc[layerId-1][segId-1].push_back(ttdc);
	  if( t0tdclow<ttdc && ttdc<t0tdchigh && ttdc>ttdc_1st) ttdc_1st = ttdc;
	}
	//def	event.ttdc1[segId-1].push_back(ttdc_1st);
	//sako 2024/4/6
	event.ttdc1[layerId-1][segId-1].push_back(ttdc_1st);

	//def	event.nhltdc[segId-1]=sizelTdc;
	//def	event.nhttdc[segId-1]=sizetTdc;
	//sako 2024/4/6
	//	std::cout << " ### 	event.nhltdc["<<layerId-1<<"]["<<segId-1<<"]="<<sizelTdc<<std::endl;
	//	std::cout << " ### 	event.nhttdc["<<layerId-1<<"]["<<segId-1<<"]="<<sizetTdc<<std::endl;
	event.nhltdc[layerId-1][segId-1]=sizelTdc;
	event.nhttdc[layerId-1][segId-1]=sizetTdc;
      }

#if check1
      std::cout << "Layer = " << layerId 
		<< " Seg = " << segId 
		<< std::endl;
#endif
    }
  }

  tree->Fill();

  return true;
}

void EventMonitor::InitializeEvent( void )
{
  ConfMan *confMan = ConfMan::GetConfManager();
#ifdef E16DAQ
  //---------------------------------------------------------------
  event.spillID = -1;
  event.eventID[0] = -1;
  event.eventID[1] = -1;
  event.eventID[2] = -1;
  event.event_number = -1;
  event.MZN_event_number[0] = -1;
  event.MZN_event_number[1] = -1;
  event.MZN_event_number[2] = -1;
  event.nh_all = 0;

#endif
  
  //DRS4
  if( confMan->T0SimpleAna() ){
    for(int it=0; it<event.adc.size(); it++){
      event.amp[it].clear();
    }
    for(int it=0; it<event.tdc.size(); it++){
      event.dt[it].clear();
      event.width[it].clear();
    }
    event.l1_tdc  =-1;
    event.l1_tdc1 =-1;
  }
  else{
    for(int it=0; it<event.WaveForm.size(); it++){
      event.WaveForm[it].clear();
    }
    for(int it=0; it<event.adc.size(); it++){
      event.adc[it].clear();
      event.amp[it].clear();
      event.bl[it].clear();
      event.peakx[it].clear();
    }
    for(int it=0; it<event.tdc.size(); it++){
      event.tdc[it].clear();
      event.dt[it].clear();
      event.tdc_2nd[it].clear();
      event.dt_2nd[it].clear();
      event.width[it].clear();
    }
    event.l1_tdc  =-1;
    event.l1_tdc1 =-1;
  }

  //HRTdc
  //def
  /*  
  if( confMan->T0SimpleAna() ){
    for(int it=0; it<event.ltdc.size(); it++){
      event.ltdc[it].clear();
    }
    for(int it=0; it<event.ttdc.size(); it++){
      event.ttdc[it].clear();
    }
    for(int it=0; it<event.ltdc1.size(); it++){
      event.ltdc1[it].clear();
    }
    for(int it=0; it<event.ttdc1.size(); it++){
      event.ttdc1[it].clear();
    }

    for(int it=0; it<NumOfHRTDC; it++){
      event.nhltdc[it]=-1;
      event.nhttdc[it]=-1;
    }
  }
  else{
    for(int it=0; it<event.ltdc.size(); it++){
      event.ltdc[it].clear();
    }
    for(int it=0; it<event.ttdc.size(); it++){
      event.ttdc[it].clear();
    }
    for(int it=0; it<event.ltdc1.size(); it++){
      event.ltdc1[it].clear();
    }
    for(int it=0; it<event.ttdc1.size(); it++){
      event.ttdc1[it].clear();
    }
    for(int it=0; it<NumOfHRTDC; it++){
      event.nhltdc[it]=-1;
      event.nhttdc[it]=-1;
    }
  }
  */

  //sako 2024/4/6
  for(int imod=0;imod<NumOfModule; imod++) {
    if( confMan->T0SimpleAna() ){
      for(int it=0; it<event.ltdc[imod].size(); it++){
	event.ltdc[imod][it].clear();
      }
      for(int it=0; it<event.ttdc[imod].size(); it++){
	event.ttdc[imod][it].clear();
      }
      for(int it=0; it<event.ltdc1[imod].size(); it++){
	event.ltdc1[imod][it].clear();
      }
      for(int it=0; it<event.ttdc1[imod].size(); it++){
	event.ttdc1[imod][it].clear();
      }

      for(int it=0; it<NumOfHRTDC; it++){
	event.nhltdc[imod][it]=-1;
	event.nhttdc[imod][it]=-1;
      }
    }
    else{
      for(int it=0; it<event.ltdc[imod].size(); it++){
	event.ltdc[imod][it].clear();
      }
      for(int it=0; it<event.ttdc[imod].size(); it++){
	event.ttdc[imod][it].clear();
      }
      for(int it=0; it<event.ltdc1[imod].size(); it++){
	//def	event.ltdc1[it][imod].clear();
	//2024/4/15
	event.ltdc1[imod][it].clear();
      }
      for(int it=0; it<event.ttdc1[imod].size(); it++){
	//def	event.ttdc1[it][imod].clear();
	//2024/4/15
	event.ttdc1[imod][it].clear();
      }
      for(int it=0; it<NumOfHRTDC; it++){
	event.nhltdc[imod][it]=-1;
	event.nhttdc[imod][it]=-1;
      }
    }
  }

}

bool EventMonitor::ProcessingEnd()
{
  // gFile->Write();
  // gFile->Close();

  return true;
}

VEvent *ConfMan::EventAllocator()
{
  return new EventMonitor;
}

bool ConfMan:: InitializeHistograms()
{  
  ConfMan *confMan = ConfMan::GetConfManager();

  HBTree("tree","tree of Spec");
  TTree *tree = dynamic_cast<TTree *>(gFile->Get("tree"));
#ifdef E16DAQ
  tree->Branch("spillID", &event.spillID, "spillID/I");
  tree->Branch("eventID", &event.eventID, "eventID[3]/I");
  tree->Branch("event_number", &event.event_number, "event_number/I");
  tree->Branch("MZN_event_number", &event.MZN_event_number, "MZN_event_number[3]/I");
  tree->Branch("nh_all", &event.nh_all, "nh_all/I");
#endif
  //DRS4
  if( confMan->T0SimpleAna() ){
    tree->Branch("amp", &event.amp);
    tree->Branch("dt", &event.dt);
    tree->Branch("width", &event.width);
    tree->Branch("l1_tdc", &event.l1_tdc, "l1_tdc/I");
    tree->Branch("l1_tdc1", &event.l1_tdc1, "l1_tdc1/I");
  }
  else{
    tree->Branch("WaveForm", &event.WaveForm );

    tree->Branch("adc", &event.adc);
    tree->Branch("amp", &event.amp);
    tree->Branch("bl", &event.bl);
    tree->Branch("peakx", &event.peakx);
    
    tree->Branch("tdc", &event.tdc);
    tree->Branch("dt", &event.dt);
    tree->Branch("tdc_2nd", &event.tdc_2nd);
    tree->Branch("dt_2nd", &event.dt_2nd);
    tree->Branch("width", &event.width);
    
    tree->Branch("l1_tdc", &event.l1_tdc, "l1_tdc/I");
    tree->Branch("l1_tdc1", &event.l1_tdc1, "l1_tdc1/I");
  }

  //HRTdc
  //def
  /*
  if( confMan->T0SimpleAna() ){
    tree->Branch("ltdc", &event.ltdc);
    tree->Branch("ttdc", &event.ttdc);
    tree->Branch("ltdc1", &event.ltdc1);
    tree->Branch("ttdc1", &event.ttdc1);

    tree->Branch("nhltdc", &event.nhltdc, "nhltdc[64]/I");
    tree->Branch("nhttdc", &event.nhttdc, "nhttdc[64]/I");
  }
  else{
    tree->Branch("ltdc", &event.ltdc);
    tree->Branch("ttdc", &event.ttdc);
    tree->Branch("ltdc1", &event.ltdc1);
    tree->Branch("ttdc1", &event.ttdc1);

    tree->Branch("nhltdc", &event.nhltdc, "nhltdc[64]/I");
    tree->Branch("nhttdc", &event.nhttdc, "nhttdc[64]/I");
  }
  */

  //sako 2024/4/6

    tree->Branch("ltdc0", &event.ltdc[0]);
    tree->Branch("ttdc0", &event.ttdc[0]);
    tree->Branch("ltdc1", &event.ltdc[1]);
    tree->Branch("ttdc1", &event.ttdc[1]);
    tree->Branch("ltdc2", &event.ltdc[2]);
    tree->Branch("ttdc2", &event.ttdc[2]);

    tree->Branch("nhltdc0", &event.nhltdc[0], "nhltdc0[64]/I");
    tree->Branch("nhttdc0", &event.nhttdc[0], "nhttdc0[64]/I");

    tree->Branch("nhltdc1", &event.nhltdc[1], "nhltdc1[64]/I");
    tree->Branch("nhttdc1", &event.nhttdc[1], "nhttdc1[64]/I");

    tree->Branch("nhltdc2", &event.nhltdc[2], "nhltdc2[64]/I");
    tree->Branch("nhttdc2", &event.nhttdc[2], "nhttdc2[64]/I");



  return true;
}
