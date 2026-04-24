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
// $Id: exampleN02.cc 69899 2013-05-17 10:05:33Z gcosmo $
//
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
#ifdef G4MULTITHREADED
//#include "G4MTRunManager.hh"
#else
#include "G4RunManager.hh"
#endif

#include "ExN02DetectorConstruction.hh"

#include "ExN02PhysicsList.hh"
#include "ExN02PrimaryGeneratorAction.hh"
#include "ExN02RunAction.hh"
#include "ExN02EventAction.hh"
#include "ExN02SteppingAction.hh"
#include "ExN02SteppingVerbose.hh"

#include "G4RunManager.hh"
#include "G4UImanager.hh"
#include "G4UIterminal.hh"
#include "G4UItcsh.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"
#include "time.h"
#include "Para.h"
#include "G4GeneralParticleSource.hh"
#ifdef G4VIS_USE
#include "G4VisExecutive.hh"
#endif

#ifdef G4UI_USE
#include "G4UIExecutive.hh"
#endif


#include "Shielding.hh"
#include "PhotonCounterEventAction.hh"
#include "PhotonCounterSteppingAction.hh"


#include "G4VisExecutive.hh"
#include "G4UIExecutive.hh"
#include "G4PhysListFactory.hh"
#include "G4LogicalVolume.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4SDManager.hh"

int myeventID;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

int main(int argc,char** argv)
{
  //Choose the random engine
  CLHEP::HepRandom::setTheEngine(new CLHEP::RanecuEngine());
  G4long seed = time(NULL);
  CLHEP::HepRandom::setTheSeed(seed);   

  // User Verbose output class
  //
  G4VSteppingVerbose* verbosity = new ExN02SteppingVerbose;
  G4VSteppingVerbose::SetInstance(verbosity);
  
  // Run manager
  //
  G4RunManager * runManager = new G4RunManager;
  

  // User Initialization classes (mandatory)
  //
  ExN02DetectorConstruction* detector = new ExN02DetectorConstruction;
  runManager->SetUserInitialization(detector);
  //
  G4VUserPhysicsList* physics = new ExN02PhysicsList;
  //G4VModularPhysicsList* physics = new Shielding;
  runManager->SetUserInitialization(physics);
   
  // User Action classes
  //
  G4VUserPrimaryGeneratorAction* gen_action = new ExN02PrimaryGeneratorAction();
  runManager->SetUserAction(gen_action);
  //
  ExN02RunAction* run_action = new ExN02RunAction;
  runManager->SetUserAction(run_action);
  //
  //ExN02EventAction* event_action = new ExN02EventAction(run_action);
  //runManager->SetUserAction(event_action);
  //
  //ExN02SteppingAction* stepping_action = new ExN02SteppingAction(event_action,run_action);
  //runManager->SetUserAction(stepping_action);




  
  PhotonCounterEventAction* eventAction = new PhotonCounterEventAction(run_action);
  runManager->SetUserAction(eventAction);

  PhotonCounterSteppingAction* steppingAction = new PhotonCounterSteppingAction(eventAction);
  runManager->SetUserAction(steppingAction);
  
  
  
  
  
  
  //创建粒子源
  G4GeneralParticleSource* particleSource = new G4GeneralParticleSource();
  
  
  
  // Initialize G4 kernel
  //
  runManager->Initialize();
      
     
      
      
#ifdef G4VIS_USE
  G4VisManager* visManager = new G4VisExecutive;
  visManager->Initialize();
#endif    
     
  // Get the pointer to the User Interface manager
  //
  G4UImanager * UImanager = G4UImanager::GetUIpointer();  

  if (argc!=1)   // batch mode  
    {
      G4String command = "/control/execute ";
      G4String fileName = argv[1];
      UImanager->ApplyCommand(command+fileName);
    }

  else           // interactive mode : define UI session
    { 
#ifdef G4UI_USE
      G4UIExecutive * ui = new G4UIExecutive(argc,argv);
#ifdef G4VIS_USE
      UImanager->ApplyCommand("/control/execute vis.mac");     
#endif
      ui->SessionStart();
      delete ui;
#endif
     
#ifdef G4VIS_USE
      delete visManager;
#endif     
    }

  // Free the store: user actions, physics_list and detector_description are
  //                 owned and deleted by the run manager, so they should not
  //                 be deleted in the main() program !

  // Start a run
  //
//  G4int numberOfEvent = 10;
//  runManager->BeamOn(numberOfEvent);

  delete runManager;
  delete verbosity;

  return 0;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......


