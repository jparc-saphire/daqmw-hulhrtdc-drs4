// -*- C++ -*-

#ifndef Drs4TdcDrawer_hh
#define Drs4TdcDrawer_hh

#include <string>

class THttpServer;

class Drs4TdcDrawer
{
public:
  static const unsigned int kTdcMask = 0xffffff;
  
  Drs4TdcDrawer();
  ~Drs4TdcDrawer();

  Drs4TdcDrawer(const Drs4TdcDrawer&);
  Drs4TdcDrawer& operator=(const Drs4TdcDrawer&);
  void fill(const dataDrs& cont,bool isupdate=true);
  void initialize(THttpServer* server, 
                  const std::string& name);
  void reset();
  void setT0Channel(unsigned int t0_ch)
  { m_t0_ch = t0_ch; }

  unsigned int m_t0_ch;
  unsigned int nobject_;

private:
  static int m_ncalled;
  int m_nIns;
  
};

#endif
