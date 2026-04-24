// from ExN02, add radioactiveDecay and new cut value.  by Lsk 12-03
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......
//关于波长位移光纤(wls)的物理过程,在G4OpWLS文件中有较详细的描述,且G4OpWLS.hh需要添加到physicslist的头文件里
#include "globals.hh"
#include "ExN02PhysicsList.hh"

#include "G4ProcessManager.hh"
#include "G4ParticleTypes.hh"

#include "G4BosonConstructor.hh"
#include "G4LeptonConstructor.hh"
#include "G4MesonConstructor.hh"
#include "G4BaryonConstructor.hh"
#include "G4IonConstructor.hh"
#include "G4ShortLivedConstructor.hh"

#include "G4EmStandardPhysics.hh"
#include "G4EmProcessOptions.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4UAtomicDeexcitation.hh"

#include "G4SystemOfUnits.hh"
#include "G4PhysicsListHelper.hh"
#include "G4LossTableManager.hh"

#include "G4Cerenkov.hh"
#include "G4Scintillation.hh"
#include "G4OpAbsorption.hh"
#include "G4OpRayleigh.hh"
#include "G4OpMieHG.hh"
#include "G4OpBoundaryProcess.hh"
#include "G4OpWLS.hh"

#include "G4LossTableManager.hh"
#include "G4EmSaturation.hh"

#include "G4RegionStore.hh"

G4ThreadLocal G4int ExN02PhysicsList::fVerboseLevel = 0;
G4ThreadLocal G4int ExN02PhysicsList::fMaxNumPhotonStep = 20;
G4ThreadLocal G4Cerenkov* ExN02PhysicsList::fCerenkovProcess = 0;
G4ThreadLocal G4Scintillation* ExN02PhysicsList::fScintillationProcess = 0;
G4ThreadLocal G4OpAbsorption* ExN02PhysicsList::fAbsorptionProcess = 0;
G4ThreadLocal G4OpRayleigh* ExN02PhysicsList::fRayleighScatteringProcess = 0;
G4ThreadLocal G4OpMieHG* ExN02PhysicsList::fMieHGScatteringProcess = 0;
G4ThreadLocal G4OpBoundaryProcess* ExN02PhysicsList::fBoundaryProcess = 0;
G4ThreadLocal G4OpWLS* ExN02PhysicsList::fWLSProcess = 0;

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PhysicsList::ExN02PhysicsList():  G4VUserPhysicsList()
{
   defaultCutValue = 0.01*mm;  // 如何合理设置cutValue
   SetVerboseLevel(0);
   emPhysicsList  = new G4EmLivermorePhysics;
//   emPhysicsList  = new G4EmStandardPhysics;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

ExN02PhysicsList::~ExN02PhysicsList()
{
  delete emPhysicsList;
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructParticle()
{
  // In this method, static member functions should be called
  // for all particles which you want to use.
  // This ensures that objects of these particle types will be
  // created in the program. 

  ConstructBosons();
  ConstructLeptons();
  ConstructMesons();
  ConstructBaryons();
  ConstructIons();
//  ConstructShortLived();

  // pseudo-particles
  G4Geantino::GeantinoDefinition();
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructBosons()
{
  // Construct all bosons
  G4BosonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

void ExN02PhysicsList::ConstructLeptons()
{
  // Construct all leptons
  G4LeptonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

void ExN02PhysicsList::ConstructMesons()
{
  //  Construct all mesons
  G4MesonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

void ExN02PhysicsList::ConstructBaryons()  // (Lsk 12-03-18) 如何定义alpha等粒子
{
  //  Construct all barions
  G4BaryonConstructor pConstructor;
  pConstructor.ConstructParticle();
}

void ExN02PhysicsList::ConstructIons()
{
  //  Construct light ions
  G4IonConstructor pConstructor;
  pConstructor.ConstructParticle();  
}

void ExN02PhysicsList::ConstructShortLived()
{
   //  Construct  resonaces and quarks
  G4ShortLivedConstructor pConstructor;
  pConstructor.ConstructParticle();  
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructProcess()
{
  AddTransportation();
  ConstructEM();
  ConstructDecay();    // 注册decay过程
  ConstructRadioactiveDecay();
  ConstructOp();
//  AddStepMax();    // 注册AllowedStepMax过程，用户可以在construction中定义G4UserLimits来给定StepMax
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"
#include "G4eplusAnnihilation.hh"

#include "G4MuMultipleScattering.hh"
#include "G4MuIonisation.hh"
#include "G4MuBremsstrahlung.hh"
#include "G4MuPairProduction.hh"

#include "G4hMultipleScattering.hh"
#include "G4hIonisation.hh"
#include "G4hBremsstrahlung.hh"
#include "G4hPairProduction.hh"

#include "G4ionIonisation.hh"

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::ConstructEM()
{

    // Em options
  //
  emPhysicsList->ConstructProcess();
  G4EmProcessOptions emOptions;
  emOptions.SetBuildCSDARange(true);
  emOptions.SetDEDXBinningForCSDARange(10*10);
  emOptions.SetDeexcitationActiveRegion(true); //TBC
  emOptions.SetFluo(true);
  emOptions.SetAuger(true);
  emOptions.SetPIXE(true);
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4Decay.hh"

void ExN02PhysicsList::ConstructDecay()
{
 // Add Decay Process
  G4Decay* theDecayProcess = new G4Decay();
  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    if (theDecayProcess->IsApplicable(*particle)) {
      pmanager ->AddProcess(theDecayProcess);
      // set ordering for PostStepDoIt and AtRestDoIt
      pmanager ->SetProcessOrdering(theDecayProcess, idxPostStep);
      pmanager ->SetProcessOrdering(theDecayProcess, idxAtRest);
    }
  }
}
  
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4RadioactiveDecay.hh"
#include "G4IonConstructor.hh"

void ExN02PhysicsList::ConstructRadioactiveDecay()
{
//  G4IonTable *IonTable = G4IonTable::GetIonTable();
//  G4ParticleDefinition* ion = IonTable->FindIon(Z,A,excitEnergy);
  G4RadioactiveDecay* radioactiveDecay = new G4RadioactiveDecay();
  radioactiveDecay->SetHLThreshold(nanosecond);  // half live threshold
  radioactiveDecay->SetICM(true);  // Internal Conversion
  radioactiveDecay->SetARM(true);  // Atomic Rearangement
  radioactiveDecay->SetVerboseLevel(0);   // added by lsk, 12-03-24
  
  G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();  
  ph->RegisterProcess(radioactiveDecay, G4GenericIon::GenericIon());

  G4ProcessManager* pmanager = G4GenericIon::GenericIon()->GetProcessManager();   // 研究G4GenericIon类，以及GenericIon函数的作用？？？
  pmanager->AddProcess(radioactiveDecay, 0, -1, 1);    

  // Deexcitation (in case of Atomic Rearangement)
  //
  G4UAtomicDeexcitation* de = new G4UAtomicDeexcitation();
  de->SetFluo(true);
  de->SetAuger(true);   
  de->SetPIXE(true);  
  G4LossTableManager::Instance()->SetAtomDeexcitation(de);  
}


//   const G4IonTable *theIonTable = G4ParticleTable::GetParticleTable()->GetIonTable();  // copy from WangLi
// 
//   G4RadioactiveDecay*  theRadioactiveDecay = new G4RadioactiveDecay();
//   for (G4int i=0; i<theIonTable->Entries(); i++)
//     {
//       G4String particleName = theIonTable->GetParticle(i)->GetParticleName();
//       if (particleName == "GenericIon")
// 	{
// 	  G4ProcessManager* pmanager = theIonTable->GetParticle(i)->GetProcessManager();
// 	  pmanager->SetVerboseLevel(0);
// 	  pmanager ->AddProcess(theRadioactiveDecay);
// 	  pmanager ->SetProcessOrdering(theRadioactiveDecay, idxPostStep);
// 	  pmanager ->SetProcessOrdering(theRadioactiveDecay, idxAtRest);
// 	}
//     } 
//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

#include "G4StepLimiter.hh"
#include "G4UserSpecialCuts.hh"
/*
void ExN02PhysicsList::AddStepMax()   // 添加step max的用途是什么？？？
{
  // Step limitation seen as a process
  G4StepLimiter* stepLimiter = new G4StepLimiter();
  ////G4UserSpecialCuts* userCuts = new G4UserSpecialCuts();
  
  auto theParticleIterator=GetParticleIterator();
  while ((*theParticleIterator)()){
      G4ParticleDefinition* particle = theParticleIterator->value();
      G4ProcessManager* pmanager = particle->GetProcessManager();

      if (particle->GetPDGCharge() != 0.0)
        {
	  pmanager ->AddDiscreteProcess(stepLimiter);
	  ////pmanager ->AddDiscreteProcess(userCuts);
        }
  }
}
*/

#include "G4Threading.hh"

void ExN02PhysicsList::ConstructOp()//光学过程的定义主要在这一部分
{

  fCerenkovProcess = new G4Cerenkov("Cerenkov");
  fCerenkovProcess->SetMaxNumPhotonsPerStep(fMaxNumPhotonStep);
  fCerenkovProcess->SetMaxBetaChangePerStep(10.0);
  fCerenkovProcess->SetTrackSecondariesFirst(true);
  fScintillationProcess = new G4Scintillation("Scintillation");
  fScintillationProcess->SetScintillationYieldFactor(1.);//光产额因子,好像还有一个ExcitationRatio()可以设置,这里没写
  fScintillationProcess->SetTrackSecondariesFirst(true);//悬置原粒子直到所有子代被追踪后才计算原粒子,这个设置是用来加快计算的
  fAbsorptionProcess = new G4OpAbsorption();
  fRayleighScatteringProcess = new G4OpRayleigh();
  fMieHGScatteringProcess = new G4OpMieHG();
  fBoundaryProcess = new G4OpBoundaryProcess();

  fWLSProcess = new G4OpWLS();
  fWLSProcess->UseTimeProfile("delta");

  fCerenkovProcess->SetVerboseLevel(fVerboseLevel);
  fScintillationProcess->SetVerboseLevel(fVerboseLevel);
  fAbsorptionProcess->SetVerboseLevel(fVerboseLevel);
  fRayleighScatteringProcess->SetVerboseLevel(fVerboseLevel);
  fMieHGScatteringProcess->SetVerboseLevel(fVerboseLevel);
  fBoundaryProcess->SetVerboseLevel(fVerboseLevel);
  fWLSProcess->SetVerboseLevel(fVerboseLevel);
  
  // Use Birks Correction in the Scintillation process
  if(G4Threading::IsMasterThread())
  {
    G4EmSaturation* emSaturation =
              G4LossTableManager::Instance()->EmSaturation();
      fScintillationProcess->AddSaturation(emSaturation);
  }

  auto particleIterator=GetParticleIterator();
  particleIterator->reset();
  while( (*particleIterator)() ){
    G4ParticleDefinition* particle = particleIterator->value();
    G4ProcessManager* pmanager = particle->GetProcessManager();
    G4String particleName = particle->GetParticleName();
    if (fCerenkovProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(fCerenkovProcess);
      pmanager->SetProcessOrdering(fCerenkovProcess,idxPostStep);
    }
    if (fScintillationProcess->IsApplicable(*particle)) {
      pmanager->AddProcess(fScintillationProcess);
      // 使用标准的 SetProcessOrdering，而不是 SetProcessOrderingToLast
      pmanager->SetProcessOrdering(fScintillationProcess, idxAtRest);
      pmanager->SetProcessOrdering(fScintillationProcess, idxPostStep);
    }
    if (particleName == "opticalphoton") {
      G4cout << " AddDiscreteProcess to OpticalPhoton " << G4endl;
      pmanager->AddDiscreteProcess(fAbsorptionProcess);
      pmanager->AddDiscreteProcess(fRayleighScatteringProcess);
      pmanager->AddDiscreteProcess(fMieHGScatteringProcess);
      pmanager->AddDiscreteProcess(fBoundaryProcess);
      pmanager->AddDiscreteProcess(fWLSProcess);
    }
  }
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

void ExN02PhysicsList::SetCuts()  //  
{
  //G4VUserPhysicsList::SetCutsWithDefault method sets 
  //the default cut value for all particle types 
  //
//  SetCutsWithDefault();  // only Ionisation and Bremsstrahlung processes are concerned by this cutValue
  G4double cutForGamma = 100*mm;
  G4double cutForElectron = 100*mm;
  G4double cutForProton = 100*mm;
 
  SetCutValue( cutForGamma, "gamma" );
  SetCutValue( cutForElectron, "e-" );
  SetCutValue( cutForElectron, "e+" );
  SetCutValue( cutForProton, "proton" );


/*
  G4Region* region;
  G4String regName;
  G4ProductionCuts* cuts;
  regName = "pyramid";
  region = G4RegionStore::GetInstance()->GetRegion(regName);
  cuts = new G4ProductionCuts;
  cuts->SetProductionCut(100*mm); // same cuts for gamma, proton, e- and e+
  region->SetProductionCuts(cuts);

  regName = "chamberngallery";
  region = G4RegionStore::GetInstance()->GetRegion(regName);
  cuts = new G4ProductionCuts;
  cuts->SetProductionCut(100*mm); // same cuts for gamma, proton, e- and e+
  region->SetProductionCuts(cuts);

  regName = "queens_chamber";
  region = G4RegionStore::GetInstance()->GetRegion(regName);
  cuts = new G4ProductionCuts;
  cuts->SetProductionCut(1*mm); // same cuts for gamma, proton, e- and e+
  region->SetProductionCuts(cuts);

  regName = "gallery";
  region = G4RegionStore::GetInstance()->GetRegion(regName);
  cuts = new G4ProductionCuts;
  cuts->SetProductionCut(1*mm); // same cuts for gamma, proton, e- and e+
  region->SetProductionCuts(cuts);

  regName = "chamber1";
  region = G4RegionStore::GetInstance()->GetRegion(regName);
  cuts = new G4ProductionCuts;
  cuts->SetProductionCut(100*mm); // same cuts for gamma, proton, e- and e+
  region->SetProductionCuts(cuts);
*/
  //   We strongly recommend that results generated with this new feature be compared with results using the same
// geometry and uniform production thresholds. Setting completely different cut values for individual regions may
// break the coherent and comprehensive accuracy of the simulation. Therefore cut values should be carefully
// optimized, based on a comparison with results obtained using uniform cuts.
  
  // Production thresholds for CsI detector
//   G4Region* region;
//   G4String regName;
//   G4ProductionCuts* cuts;
//   
//   regName = "CsIDetectors";
//   region = G4RegionStore::GetInstance()->GetRegion(regName);
//   cuts = new G4ProductionCuts;
//   cuts->SetProductionCut(0.01*mm, G4ProductionCuts::GetIndex("gamma"));
//   cuts->SetProductionCut(0.1*mm, G4ProductionCuts::GetIndex("gamma"));
//   cuts->SetProductionCut(0.1*mm, G4ProductionCuts::GetIndex("gamma"));

  if (verboseLevel>0) DumpCutValuesTable();  // 该verboseLevel由哪个控制？？？在PhysicsList的构造函数中
}

//....oooOO0OOooo........oooOO0OOooo........oooOO0OOooo........oooOO0OOooo......

