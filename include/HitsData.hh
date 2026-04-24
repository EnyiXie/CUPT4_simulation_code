//////////////////////////////////////////////////////////
// Convert binary data from Tsinghua's TPC to ASCII or slcio files
// Sun. Apr. 29 17:54:22 2007 by yangzw
//////////////////////////////////////////////////////////
#ifndef Hits_Data_H
#define Hits_Data_H

#include "Rtypes.h"

class HitsData {
 public:
  HitsData( ) { 
    PDGid     = 0   ;
    Mass      = 0.0 ;
    Deposit_E = 0.0 ;
    Length = 0.0;
    PrimaryE = 0.0;
    PrimaryM = 0.0;
    evtid    = 0   ;
    e_tot    = 0.0 ;

  }
  virtual ~HitsData( ){;}

  Int_t   PDGid  ; //PDG ID of particle
  Float_t Mass ; //intitial energy of particle
  Float_t Deposit_E ; //deposit energy
  Float_t Length; //
  Float_t PrimaryE;//初始能量；
  Float_t PrimaryM;//初始动量；
  Int_t   evtid  ; //PDG ID of particle
  Float_t e_tot ; //intitial energy of particle

 private:
};

#endif //Hits_Data_H

