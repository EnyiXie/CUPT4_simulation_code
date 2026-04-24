//
// ********************************************************************
// * License and Disclaimer                                           *
// *                                                                  *
// * The  Geant4 software  is  copyright of the Copyright Holders  of *
// * the Geant4 Collaboration.  It is provided  under  the terms  and *
// * conditions of the Geant4 Software License,  included in the file *
// * LICENSE and available at  http://cern.ch/geant4/license .  These *
// * include a list of copyright holders.                             *
// *                                                                  *
// * Neither the authors of this software system, nor their employing *
// * institutes,nor the agencies providing financial support for this *
// * work  make  any representation or  warranty, express or implied, *
// * regarding  this  software system or assume any liability for its *
// * use.  Please see the license in the file  LICENSE  and URL above *
// * for the full disclaimer and the limitation of liability.         *
// *                                                                  *
// * This  code  implementation is the result of  the  scientific and *
// * technical work of the GEANT4 collaboration.                      *
// * By using,  copying,  modifying or  distributing the software (or *
// * any work based  on the software)  you  agree  to acknowledge its *
// * use  in  resulting  scientific  publications,  and indicate your *
// * acceptance of all terms of the Geant4 Software license.          *
// ********************************************************************
//
//
// $Id: ExN02EventAction.hh 69899 2013-05-17 10:05:33Z gcosmo $
//
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
 
#ifndef ExN02EventAction_h
#define ExN02EventAction_h 1

#include "G4UserEventAction.hh"
#include "globals.hh"
#include <fstream>
#include "G4Types.hh"
extern double SumEnergy_tot;


using namespace std;

class G4Event;
class ExN02RunAction;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

class ExN02EventAction : public G4UserEventAction
{
  public:
    ExN02EventAction(ExN02RunAction*);
   ~ExN02EventAction();

  public:
    void BeginOfEventAction(const G4Event*event);
    void EndOfEventAction(const G4Event*event);
  void    AddEnergy_tot(G4double de_tot) {SumEnergy_tot += de_tot;}

  void    Addrmax(G4double r)
  {
    if(rmax<r)
       rmax=r;
  }
   void    Addrmin(G4double r)
  {
    if(rmin>r)
       rmin=r;
  }
  G4int   geteventid(){return eventid;}  
  
  void AddEdep(G4double edep) { fEdep += edep; }
  
  private:
    ExN02RunAction* runAct;
   G4int     eventid;   
   G4double  rmax;
   G4double  rmin;
   G4double     fEdep;
   G4double     fEdep2;
};

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#endif

    
