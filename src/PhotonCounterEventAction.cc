//PhotonCounterEventAction.cc
#include "PhotonCounterEventAction.hh"
#include "ExN02RunAction.hh"
#include "G4Event.hh"
#include "G4ios.hh"
#include "G4SystemOfUnits.hh"
#include "G4PrimaryVertex.hh"

PhotonCounterEventAction::PhotonCounterEventAction(ExN02RunAction* runAction)
: G4UserEventAction(), 
  fRunAction(runAction)
{}

PhotonCounterEventAction::~PhotonCounterEventAction()
{}

void PhotonCounterEventAction::BeginOfEventAction(const G4Event*)
{ 
  fGeneratedPhotonCount = 0;
  fTotalEdep = 0.;
  
  fTransmittedPhotonCount1 = 0;
  fTransmittedPhotonCount2 = 0;
  fTransmittedPhotonCount3 = 0;
  fTransmittedPhotonCount4 = 0;

  photonIDs1.clear();
  photonIDs2.clear();
  photonIDs3.clear();
  photonIDs4.clear();
}

void PhotonCounterEventAction::EndOfEventAction(const G4Event* event)
{
  // 获取主顶点的真实位置
  G4PrimaryVertex* primaryVertex = event->GetPrimaryVertex();
  G4ThreeVector position = primaryVertex->GetPosition();
  G4double x_true = position.z() / mm; // 除以单位，得到纯数值
  G4double y_true = position.x() / mm;

  // 将 x_true 和 y_true 传递给 AddEventData
  fRunAction->AddEventData(event->GetEventID(),
                           fTotalEdep / MeV,
                           fGeneratedPhotonCount,
                           fTransmittedPhotonCount1,
                           fTransmittedPhotonCount2,
                           fTransmittedPhotonCount3,
                           fTransmittedPhotonCount4,
                           x_true, // <--- 传递 x
                           y_true  // <--- 传递 y
                           );
}

void PhotonCounterEventAction::AddEdep(G4double edep) {
  fTotalEdep += edep;
}

/*void PhotonCounterEventAction::AddCharge(G4double Charge) {
  fTotalCharge += Charge;
}*/


void PhotonCounterEventAction::AddTransmittedPhoton1(G4int trackID) 
{
    auto inserted = photonIDs1.insert(trackID);
    if (inserted.second) {           // 如果插入成功 (ID是新的)
        fTransmittedPhotonCount1++;
    }
}

void PhotonCounterEventAction::AddTransmittedPhoton2(G4int trackID) 
{
    auto inserted = photonIDs2.insert(trackID);
    if (inserted.second) {           // 如果插入成功 (ID是新的)
        fTransmittedPhotonCount2++;
    }
}

void PhotonCounterEventAction::AddTransmittedPhoton3(G4int trackID) 
{
    auto inserted = photonIDs3.insert(trackID);
    if (inserted.second) {           // 如果插入成功 (ID是新的)
        fTransmittedPhotonCount3++;
    }
}

void PhotonCounterEventAction::AddTransmittedPhoton4(G4int trackID) 
{
    auto inserted = photonIDs4.insert(trackID);
    if (inserted.second) {           // 如果插入成功 (ID是新的)
        fTransmittedPhotonCount4++;
    }
}
     
     
     
     
