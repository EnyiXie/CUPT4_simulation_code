//PhotonCounterSteppingAction.hh
#ifndef PhotonCounterSteppingAction_h
#define PhotonCounterSteppingAction_h 1

#include "G4UserSteppingAction.hh"

class PhotonCounterEventAction;

class PhotonCounterSteppingAction : public G4UserSteppingAction
{
  public:
    PhotonCounterSteppingAction(PhotonCounterEventAction*eventAction);
    virtual ~PhotonCounterSteppingAction();

    virtual void UserSteppingAction(const G4Step*);

  private:
    PhotonCounterEventAction* fEventAction;
};

#endif
