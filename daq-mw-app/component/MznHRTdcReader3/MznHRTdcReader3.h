// -*- C++ -*-
/*!
 * @file 
 * @brief
 * @date
 * @author
 *
 */

#ifndef MZNHRTDCREADER3_H
#define MZNHRTDCREADER3_H

#include <daqmw/Sock.h>
#include <cstdint>
#include <string>
#include <vector>
#include <memory>
#include <errno.h>

#include "UDPRBCP.hh"
#include "FPGAModule.hh"

#include "DaqComponentBase.h"

struct component_header_t {
  uint32_t m_magic;
  uint32_t m_id;
};

using namespace RTC;

class MznHRTdcReader3
  : public DAQMW::DaqComponentBase
{
public:
  static constexpr int kNofHead        = 3;
  static constexpr int kNofData        = kNofHead + 2048;
  static constexpr uint32_t kMagic     = 0x504d4f43; //  COMP ('PMOC')
  static constexpr int kCompHeaderSize = sizeof(component_header_t);


public:
  MznHRTdcReader3(RTC::Manager* manager);
  MznHRTdcReader3(const MznHRTdcReader3&)             = delete;
  MznHRTdcReader3& operator=(const MznHRTdcReader3&)  = delete;
  MznHRTdcReader3(MznHRTdcReader3&&)                  = delete;
  MznHRTdcReader3& operator=(MznHRTdcReader3&&)       = delete;
  virtual ~MznHRTdcReader3()                           = default;
  // The initialize action (on CREATED->ALIVE transition)
  // former rtc_init_entry()
  virtual RTC::ReturnCode_t onInitialize();

  // The execution action that is invoked periodically
  // former rtc_active_do()
  virtual RTC::ReturnCode_t onExecute(RTC::UniqueId ec_id);

private:
  TimedOctetSeq          m_out_data;
  OutPort<TimedOctetSeq> m_OutPort;

private:
  int daq_dummy();
  int daq_configure();
  int daq_unconfigure();
  int daq_start();
  int daq_run();
  int daq_stop();
  int daq_pause();
  int daq_resume();

  int parse_params(::NVList* list);
  int read_data_from_detectors();
  int set_data(unsigned int data_byte_size);
  int write_OutPort();

  std::vector<uint32_t> m_data;
  unsigned int m_recv_byte_size{0};

  BufferStatus m_out_status{BUF_SUCCESS};
  bool m_debug{false};

  std::string m_srcAddr;
  int m_srcPort{24};
  int m_udpPort{4660};
  //int m_sock{0};
  DAQMW::Sock* m_sock{0};               /// socket for data server
  unsigned int m_recvtimeout{10};

  bool m_enUp{true};
  bool m_enDown{true};
  unsigned int m_windowMax{2047};
  unsigned int m_windowMin{0};
  std::unique_ptr<RBCP::UDPRBCP> m_udpRbcp;
  std::unique_ptr<HUL::FPGAModule> m_fModule;

  component_header_t m_compHeader{ kMagic, 0 };

};


//Those functions are copied from DAQ-HIMAC-201807/misc
unsigned int getAddrIPv4(const char* hostname);
std::string getAddrIPv4_str(const char* hostname);

extern "C"
{
  void MznHRTdcReader3Init(RTC::Manager* manager);
};

#endif // MZN_HRTDC_READER_H
