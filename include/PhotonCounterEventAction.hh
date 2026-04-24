//PhotonCounterEventAction.hh
#ifndef PhotonCounterEventAction_h
#define PhotonCounterEventAction_h 1

#include "G4Types.hh"
#include "G4UserEventAction.hh"
#include <fstream> 
#include <set>          
#include "globals.hh"   

class ExN02RunAction;

class PhotonCounterEventAction : public G4UserEventAction
{
  public:
    PhotonCounterEventAction(ExN02RunAction* runAction); // Add a filename parameter to the constructor
    virtual ~PhotonCounterEventAction();

    virtual void BeginOfEventAction(const G4Event*);  
    virtual void EndOfEventAction(const G4Event*);
    
    
    // ...

    void AddTransmittedPhoton1(G4int trackID);
     
    void AddTransmittedPhoton2(G4int trackID); 
     
    void AddTransmittedPhoton3(G4int trackID); 
     
    void AddTransmittedPhoton4(G4int trackID);
    
    void AddEdep(G4double edep);
    
    //void AddCharge(G4double Charge);
    
    void AddGeneratedPhoton() { fGeneratedPhotonCount++; }  // 用于记录产生的闪烁光子


  private:
      
    ExN02RunAction * fRunAction;
    G4double fTotalEdep;
    G4int fGeneratedPhotonCount;  //记录产生的闪烁光子总数
    
    G4int fTransmittedPhotonCount1; 
    std::set<G4int> photonIDs1;
    G4int fTransmittedPhotonCount2; //  for transmitted photons count
    std::set<G4int> photonIDs2;
    G4int fTransmittedPhotonCount3; 
    std::set<G4int> photonIDs3;
    G4int fTransmittedPhotonCount4; 
    std::set<G4int> photonIDs4; // 使用集合来存储唯一的轨道ID
};

#endif


