#ifndef DrsDraw_h
#define DrsDraw_h

#include <string>
#include <memory>

class THttpServer;
struct dataDrs;

class DrsDraw 
{
public:
  DrsDraw(const char* name="drs0");
  DrsDraw(const DrsDraw&)            = delete;
  DrsDraw& operator=(const DrsDraw&) = delete;
  DrsDraw(DrsDraw&&)                 = delete;
  DrsDraw& operator=(DrsDraw&&)      = delete;
  ~DrsDraw()                         = default;

  void Fill(const dataDrs& cont);
  void Initialize(THttpServer* p);
  void Reset();


private:
  std::string fName;

};
#endif
