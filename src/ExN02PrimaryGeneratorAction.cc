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
// $Id: ExN02PrimaryGeneratorAction.cc,v 1.6 2006/06/29 17:47:23 gunter Exp $
// GEANT4 tag $Name: geant4-09-02 $
//

#include "ExN02PrimaryGeneratorAction.hh"
#include "ExN02DetectorConstruction.hh"
#include "G4GeneralParticleSource.hh"
#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "globals.hh"
#include "Randomize.hh"
#include "G4SystemOfUnits.hh"
#include "G4GeneralParticleSource.hh"
#include "G4LogicalVolumeStore.hh"

#include "G4Box.hh"
#include <iostream>
#include <fstream>
#define _USE_MATH_DEFINES
#include <math.h>

#include "G4IonTable.hh"
#include "G4Geantino.hh"
#define PI 3.1415926
using namespace std;

G4double theta,phi;
std::ifstream in;

//ExN02PrimaryGeneratorAction::ExN02PrimaryGeneratorAction(ExN02DetectorConstruction* myDC):myDetector(myDC)
ExN02PrimaryGeneratorAction::ExN02PrimaryGeneratorAction()
: G4VUserPrimaryGeneratorAction(),
  fParticleGun(0), 
  fEnvelopeBox(0)
{

 
  //particleGun = new G4GeneralParticleSource();
  G4int n_particle = 1;
  fParticleGun  = new G4ParticleGun(n_particle);
 
  //default kinematic
  //
  G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
  G4ParticleDefinition* particle = particleTable->FindParticle("mu-");

  fParticleGun->SetParticleDefinition(particle);
  fParticleGun->SetParticleTime(0.0*ns);
  fParticleGun->SetParticlePosition(G4ThreeVector(0.0*cm,10.0*cm,0.0*cm));
  fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
  fParticleGun->SetParticleEnergy(4*GeV);    //动能
}
  
ExN02PrimaryGeneratorAction::~ExN02PrimaryGeneratorAction()
{
  delete fParticleGun;
}

void ExN02PrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{

  if(fParticleGun->GetParticleDefinition()==G4Geantino::Geantino())
  {
    /*G4int Z = 11, A = 22;
    G4double ionCharge  = 0.*eplus;
    G4double excitEnergy = 0.*keV;
    
    
    G4ParticleDefinition* ion
       =G4IonTable::GetIonTable()->GetIon(Z,A,excitEnergy);
       
       fParticleGun->SetParticleDefinition(ion);
       fParticleGun->SetParticleCharge(ionCharge);*/
  }

 G4double envSizeXY = 0;
  G4double envSizeZ = 0;

if (!fEnvelopeBox)
  {
    G4LogicalVolume* envLV
      = G4LogicalVolumeStore::GetInstance()->GetVolume("Envelope");
    if ( envLV ) fEnvelopeBox = dynamic_cast<G4Box*>(envLV->GetSolid());
  }

  if ( fEnvelopeBox ) {
    envSizeXY = fEnvelopeBox->GetXHalfLength()*2.;
    envSizeZ = fEnvelopeBox->GetZHalfLength()*2.;
  }  
  else  {
    G4ExceptionDescription msg;
    msg << "Envelope volume of box shape not found.\n"; 
    msg << "Perhaps you have changed geometry.\n";
    msg << "The gun will be place at the center.";
    G4Exception("B1PrimaryGeneratorAction::GeneratePrimaries()",
     "MyCode0002",JustWarning,msg);
  }
  
/*
//各向同性
G4double Direction[3];
 
 G4double theta = acos(1.0-G4UniformRand());
 G4double phi = G4UniformRand()*2.0*CLHEP::pi;
 
 Direction[0] = sin(theta)*cos(phi);
 Direction[1] = sin(theta)*sin(phi);
 Direction[2] = cos(theta);
 
 G4ThreeVector directPri = G4ThreeVector(Direction[0],Direction[1],Direction[2]);
 fParticleGun->SetParticleMomentumDirection(directPri);
*/ 
  //fParticleGun->SetParticlePosition(G4ThreeVector(x0,y0,z0));
  //fParticleGun->SetParticlePosition(G4ThreeVector(0.*cm,20.*cm,0.*cm));
   //define particle position
 // fParticleGun->SetParticleMomentumDirection(G4ThreeVector(0.,-1.,0.));
  fParticleGun->GeneratePrimaryVertex(anEvent);
  //define particle position
 


}
/*
void ExN02PrimaryGeneratorAction::SetOptPhotonPolar()//意思是闪烁光的方向是随机的吗?难道闪烁光是在这里发射出来的吗?
{
 G4double angle = G4UniformRand() * 360.0*deg;
 SetOptPhotonPolar(angle);
}
*/


//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
/*
void ExN02PrimaryGeneratorAction::SetOptPhotonPolar(G4double angle)
{
 if (fParticleGun->GetParticleDefinition()->GetParticleName()!="opticalphoton")
   {
     G4cout << "--> warning from PrimaryGeneratorAction::SetOptPhotonPolar() :"
               "the particleGun is not an opticalphoton" << G4endl;
     return;
   }

 G4ThreeVector normal (1., 0., 0.);
 G4ThreeVector kphoton = fParticleGun->GetParticleMomentumDirection();
 G4ThreeVector product = normal.cross(kphoton);
 G4double modul2       = product*product;
 
 G4ThreeVector e_perpend (0., 0., 1.);
 if (modul2 > 0.) e_perpend = (1./std::sqrt(modul2))*product;
 G4ThreeVector e_paralle    = e_perpend.cross(kphoton);
 
 G4ThreeVector polar = std::cos(angle)*e_paralle + std::sin(angle)*e_perpend;
 fParticleGun->SetParticlePolarization(polar);
}
*/

