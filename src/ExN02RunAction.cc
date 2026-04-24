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
// $Id: ExN02RunAction.cc 69899 2013-05-17 10:05:33Z gcosmo $
// 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "ExN02RunAction.hh"
#include "Randomize.hh"
#include "TFile.h"
#include "TTree.h"
#include "HitsData.hh"
#include "G4Run.hh"
#include "G4ios.hh"


#include "G4Navigator.hh"
#include "G4TransportationManager.hh"
#include "ExN02PrimaryGeneratorAction.hh"
#include "ExN02DetectorConstruction.hh"

#include "B1Analysis.hh"
#include "G4RunManager.hh"
//#include "G4AccumulableManager.hh"
#include "G4LogicalVolumeStore.hh"
#include "G4LogicalVolume.hh"
#include "G4UnitsTable.hh"
#include "G4SystemOfUnits.hh"
//#include "G4GeneralParticleSource.hh"
#include <stdio.h>
#include <iomanip> 

double kenergy,denergy;
int evid;


 
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02RunAction::ExN02RunAction(): G4UserRunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02RunAction::~ExN02RunAction()
{}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::BeginOfRunAction(const G4Run*)
{
  
    
  // 使用 C++ ofstream 创建一个 CSV 文件
  fDataFile.open("simulation_data.csv", std::ios::app);// 将打开模式从 std::ios::out 改为 std::ios::app (append)

  if (!fDataFile.is_open()) {
    G4cout << "Error: Failed to open data file: simulation_data.csv" << G4endl;
    return;
  }
  
  // 写入CSV文件的标题行 (Header)
  /*fDataFile << "EventID,"
            << "DepositedEnergy_MeV,"
            << "GeneratedPhotons,"
            << "DetectedPMT1,"
            << "DetectedPMT2,"
            << "DetectedPMT3,"
            << "DetectedPMT4"
            << std::endl;*/
            
  G4cout << "CSV data file opened." << G4endl;

  // --- 调试代码：检查材料  ---
  G4cout << "--- Checking material at a specific point ---" << G4endl;
  G4Navigator* navigator = G4TransportationManager::GetTransportationManager()->GetNavigatorForTracking();
  G4ThreeVector point_inside(23.55*cm, 0.*cm, 23.55*cm);
  G4VPhysicalVolume* vol_inside = navigator->LocateGlobalPointAndSetup(point_inside);
  if (vol_inside) 
  {
    G4cout << "Material at " << point_inside/cm << " cm is: "
           << vol_inside->GetLogicalVolume()->GetMaterial()->GetName()
           << G4endl;
  } else 
  {
    G4cout << "Point " << point_inside/cm << " cm is outside the world volume." << G4endl;
  }
  G4cout << "-------------------------------------------" << G4endl;
  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::AddEventData(
    G4int eventID, G4double edep, G4int generated,
    G4int pmt1, G4int pmt2, G4int pmt3, G4int pmt4, G4double x_true, G4double y_true)
{
  
  if ( fDataFile.is_open() ) {
    fDataFile << std::fixed << std::setprecision(3) 
    	      << x_true << "," 
              << y_true << ","
    	      << eventID << ","
              << std::fixed << std::setprecision(6) // 保证能量的小数位数
              << edep << ","
              << generated << ","
              << pmt1 << ","
              << pmt2 << ","
              << pmt3 << ","
              << pmt4
              << std::endl;
  }
}




//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02RunAction::EndOfRunAction(const G4Run*run)
{
  // 运行结束时，关闭文件
  if (fDataFile.is_open()) {
    fDataFile.close();
    G4cout << "CSV data file closed." << G4endl;

  }
  
  
  G4int nofEvents = run->GetNumberOfEvent();
  
  if (nofEvents == 0) return;
  
  const ExN02PrimaryGeneratorAction* generatorAction
   = static_cast<const ExN02PrimaryGeneratorAction*>
     (G4RunManager::GetRunManager()->GetUserPrimaryGeneratorAction());
  G4String runCondition;
  if (generatorAction)
  {
    const G4ParticleGun* particleGun = generatorAction->GetParticleGun();
    runCondition += particleGun->GetParticleDefinition()->GetParticleName();
    runCondition += " of ";
    G4double particleEnergy = particleGun->GetParticleEnergy();
    runCondition += G4BestUnit(particleEnergy,"Energy");
  }

  //    
  // Print
  //  
  
  if (IsMaster()) {
    G4cout
     << G4endl
     << "--------------------End of Global Run-----------------------";
  }
  else {
    G4cout
     << G4endl
     << "--------------------End of Local Run------------------------";
  }
  
  G4cout
     << G4endl
     << " The run consists of " << nofEvents << " "<< runCondition
     << G4endl
     << "------------------------------------------------------------"
     << G4endl;

}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......



