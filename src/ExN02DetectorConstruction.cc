// $Id: ExN02DetectorConstruction.cc,v 1.9 2015/10/04 17:47:19 gunter Exp $
// GEANT4 tag $Name: geant4-09-06 $
//

#include "ExN02DetectorConstruction.hh"

#include "G4Material.hh"
#include "G4Element.hh"
#include "G4Trd.hh"
#include "G4TessellatedSolid.hh"
#include "G4VFacet.hh"
#include "G4TriangularFacet.hh"
#include "G4QuadrangularFacet.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Tubs.hh"
#include "G4Sphere.hh"
#include "G4LogicalVolume.hh"
#include "G4ThreeVector.hh"
#include "G4PVPlacement.hh"
#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4UImanager.hh"

#include "G4VisAttributes.hh"
#include "G4Colour.hh"

#include "G4AssemblyVolume.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#include "G4Transform3D.hh"

#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4Para.hh"

#include "G4LogicalBorderSurface.hh"
#include "G4LogicalSkinSurface.hh"
#include "G4OpticalSurface.hh"


#include "G4RunManager.hh"
#include "G4NistManager.hh"
#include "G4Box.hh"
#include "G4Cons.hh"
#include "G4Orb.hh"
#include "G4Sphere.hh"
#include "G4Trd.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"
#include "G4Transform3D.hh"
#include "G4UnionSolid.hh"
#include "G4IntersectionSolid.hh"
#include "G4SubtractionSolid.hh"

#include "G4SubtractionSolid.hh"
#include "G4RotationMatrix.hh"
#include "G4ThreeVector.hh"
#define _USE_MATH_DEFINES


#include<math.h>

ExN02DetectorConstruction::ExN02DetectorConstruction()
: G4VUserDetectorConstruction(),
  fScoringVolume(0)
{ }

ExN02DetectorConstruction::~ExN02DetectorConstruction()
{}


G4VPhysicalVolume* ExN02DetectorConstruction::Construct()

{
//--------- Material definition  ---------
    G4double z, a, density;
    G4String name, symbol;
    G4int ncomponents;
    
    G4Element* H = new G4Element("Hydrogen","H",z=1., a= 1.01*g/mole);
    G4Element* C = new G4Element("Carbon","C",z=6., a= 12.01*g/mole);
    G4Element* N = new G4Element("Nitrogen", "N", z=7 , a=14.01*g/mole);
    G4Element* O = new G4Element("Oxygen"  , "O", z=8 , a=16.00*g/mole);
    
    //EJ200
    G4Material* ej200 = new G4Material("EJ200", density= 1.05*g/cm3, ncomponents=2);
    	ej200->AddElement(C, 91.6*perCent);
    	ej200->AddElement(H, 8.4*perCent);
    	
    //Acrylics	
    G4Material *PMMA = new G4Material("PMMA", density=1190*kg/m3,ncomponents=3);
    	PMMA->AddElement(H,8);
    	PMMA->AddElement(C,5);
    	PMMA->AddElement(O,2);
    	
    //Air
    G4Material* Air = new G4Material("Air", density=1.29*mg/cm3, ncomponents=2);
    	Air->AddElement(N, 70.*perCent);
    	Air->AddElement(O, 30.*perCent);
    
    //Al	
    G4Material* Aluminium = new G4Material("Aluminium", z=13., a=26.98*g/mole, density=2.700*g/cm3);

 
  // Get nist material manager
  G4NistManager* nist = G4NistManager::Instance();
  
  G4cout << G4endl << "The materials defined are : " << G4endl << G4endl;
  G4cout << *(G4Material::GetMaterialTable()) << G4endl;
  
  // Envelope parameters
  //
  G4double env_sizeXY = 200*cm, env_sizeZ = 200*cm;
  G4Material* env_mat = nist->FindOrBuildMaterial("G4_AIR");

  // Option to switch on/off checking of volumes overlaps
  //
  G4bool checkOverlaps = true;

  //
  // World
  //
  G4double world_sizeXY = env_sizeXY;
  G4double world_sizeZ  = env_sizeZ;
  G4ThreeVector pos = G4ThreeVector(0*cm, 0*cm, 0*cm);
  G4Material* world_mat = nist->FindOrBuildMaterial("G4_AIR");

  G4Box* solidWorld =
    new G4Box("World",                       //its name
       world_sizeXY, world_sizeXY, world_sizeZ);     //its size

  G4LogicalVolume* logicWorld =
    new G4LogicalVolume(solidWorld,          //its solid
                        Air,           //its material
                        "World");            //its name

  G4VPhysicalVolume* physWorld =
    new G4PVPlacement(0,                     //no rotation
                      G4ThreeVector(),       //at (0,0,0)
                      logicWorld,            //its logical volume
                      "World",               //its name
                      0,                     //its mother  volume
                      false,                 //no boolean operation
                      0,                     //copy number
                      checkOverlaps);        //overlaps checking

  //
  // Envelope
  //
  G4Box* solidEnv =
    new G4Box("Envelope",                    //its name
        150*cm, 150*cm, 150*cm); //its size

  G4LogicalVolume* logicEnv =
    new G4LogicalVolume(solidEnv,            //its solid
                        env_mat,             //its material
                        "Envelope");         //its name

  G4VPhysicalVolume* physEnv =
  new G4PVPlacement(0,                       //no rotation
                    G4ThreeVector(),         //at (0,0,0)
                    logicEnv,                //its logical volume
                    "Envelope",              //its name
                    logicWorld,              //its mother  volume
                    false,                   //no boolean operation
                    0,                       //copy number
                    checkOverlaps);          //overlaps checking
                    
                
 

  //
  // Shape 
  //
  
   
    G4Box* solidShape =
    	new G4Box("solidShape",                      //its name
              		25*cm, 2*cm, 25*cm); //its size	   
    
    G4Box* solidCutter = new G4Box("CutterBox", 2*cm, 3*cm, 2*cm);
    
    G4ThreeVector tripos1(25.*cm, 0*cm, 25.*cm);
    G4ThreeVector tripos2(25.*cm, 0*cm, -25.*cm);
    G4ThreeVector tripos3(-25.*cm, 0*cm, -25.*cm);
    G4ThreeVector tripos4(-25.*cm, 0*cm, 25.*cm);
    G4RotationMatrix* rotm = new G4RotationMatrix(); rotm->rotateY(45.*deg);
    G4Transform3D transform1(*rotm, tripos1);
    G4Transform3D transform2(*rotm, tripos2);
    G4Transform3D transform3(*rotm, tripos3);
    G4Transform3D transform4(*rotm, tripos4);
    
    G4SubtractionSolid* solidShape_cut1 = new G4SubtractionSolid("Shape-Cut1", solidShape, solidCutter, transform1);
    G4SubtractionSolid* solidShape_cut2 = new G4SubtractionSolid("Shape-Cut2", solidShape_cut1, solidCutter, transform2);
    G4SubtractionSolid* solidShape_cut3 = new G4SubtractionSolid("Shape-Cut3", solidShape_cut2, solidCutter, transform3);
    G4SubtractionSolid* finalScintSolid = new G4SubtractionSolid("FinalScintSolid", solidShape_cut3, solidCutter, transform4);
    G4LogicalVolume* logicShape = new G4LogicalVolume(finalScintSolid, ej200, "logicShape");
    
  //
  // 反射膜
  //
  
  
  G4Box* solidref =
    new G4Box("reflectance",                    //its name
        25.1*cm, 2.1*cm, 25.1*cm); 		//its size
  
  G4SubtractionSolid* finalRef = new G4SubtractionSolid("FinalRef", solidref, finalScintSolid ,0, G4ThreeVector());
  
  G4LogicalVolume* logicref =
    new G4LogicalVolume(finalRef,               //its solid
                        Aluminium,              //its material
                        "reflectance");         //its name
  
  //
  // PMT
  //
  
  G4Tubs* solidPMT= new G4Tubs("PMT", 0, 18.5*mm, 1*mm, 0.*deg, 360.*deg);
  
    G4ThreeVector tripos5(23.58*cm, 0*cm, 23.58*cm);
    G4ThreeVector tripos6(23.58*cm, 0*cm, -23.58*cm);
    G4ThreeVector tripos7(-23.58*cm, 0*cm, -23.58*cm);
    G4ThreeVector tripos8(-23.58*cm, 0*cm, 23.58*cm);
    G4RotationMatrix* rotm1 = new G4RotationMatrix(); rotm1->rotateY(45.*deg);
    G4RotationMatrix* rotm2 = new G4RotationMatrix(); rotm2->rotateY(135.*deg);
    G4RotationMatrix* rotm3 = new G4RotationMatrix(); rotm3->rotateY(225.*deg);
    G4RotationMatrix* rotm4 = new G4RotationMatrix(); rotm4->rotateY(315.*deg);
    G4Transform3D transform5(*rotm1, tripos5);
    G4Transform3D transform6(*rotm2, tripos6);
    G4Transform3D transform7(*rotm3, tripos7);
    G4Transform3D transform8(*rotm4, tripos8);
    
  
  G4IntersectionSolid* solidCorner1 = new G4IntersectionSolid("PMT1", finalRef, solidPMT,  transform5);
  G4IntersectionSolid* solidCorner2 = new G4IntersectionSolid("PMT2", finalRef, solidPMT,  transform6);
  G4IntersectionSolid* solidCorner3 = new G4IntersectionSolid("PMT3", finalRef, solidPMT,  transform7);
  G4IntersectionSolid* solidCorner4 = new G4IntersectionSolid("PMT4", finalRef, solidPMT,  transform8);
  
  // 创建逻辑体，并将角块定义为PMT材质
    G4LogicalVolume* logicPMT1 = new G4LogicalVolume(solidCorner1, PMMA, "logicPMT1");
    G4LogicalVolume* logicPMT2 = new G4LogicalVolume(solidCorner2, PMMA, "logicPMT2");
    G4LogicalVolume* logicPMT3 = new G4LogicalVolume(solidCorner3, PMMA, "logicPMT3");
    G4LogicalVolume* logicPMT4 = new G4LogicalVolume(solidCorner4, PMMA, "logicPMT4");
  

 
 
  
  G4VPhysicalVolume* physref =
  new G4PVPlacement(0,                          //no rotation
                    G4ThreeVector(),            //at (0,0,0)
                    logicref,                   //its logical volume
                    "reflectance",              //its name
                    logicEnv,                   //its mother  volume
                    false,                      //no boolean operation
                    0,                          //copy number
                    checkOverlaps);             //overlaps checking
                    

    


  
    
        G4VPhysicalVolume* physShape =
		  new G4PVPlacement(0,                       //no rotation
				    pos,                    //at position
				    logicShape,             //its logical volume
				    "physShape",                //its name
				    logicEnv,                //its mother  volume
				    false,                   //no boolean operation
				    0,                       //copy number
				    checkOverlaps);          //overlaps checking       
   
   

    
    G4VPhysicalVolume* physCornerPMT1 = new G4PVPlacement(0, G4ThreeVector(), logicPMT1, "physCornerPMT1", logicref, false, 1, checkOverlaps);
    G4VPhysicalVolume* physCornerPMT2 = new G4PVPlacement(0, G4ThreeVector(), logicPMT2, "physCornerPMT2", logicref, false, 2, checkOverlaps);
    G4VPhysicalVolume* physCornerPMT3 = new G4PVPlacement(0, G4ThreeVector(), logicPMT3, "physCornerPMT3", logicref, false, 3, checkOverlaps);
    G4VPhysicalVolume* physCornerPMT4 = new G4PVPlacement(0, G4ThreeVector(), logicPMT4, "physCornerPMT4", logicref, false, 4, checkOverlaps);
          
  fScoringVolume = logicShape; 
          
  fScoringVolume = logicShape; 

  //================================================================================
  //===== 光学属性设置
  //================================================================================
      
     
    // ------------ Inner & Add Material Properties Table ------------
    
      G4double photonEnergy[] =
      {
            2.482*eV, 2.538*eV, 2.562*eV, 2.583*eV, 2.604*eV, 2.627*eV, 2.647*eV, 2.668*eV, 2.694*eV, 2.710*eV,
            2.731*eV, 2.749*eV, 2.769*eV, 2.782*eV, 2.795*eV, 2.809*eV, 2.826*eV, 2.847*eV, 2.863*eV, 2.878*eV,
            2.894*eV, 2.917*eV, 2.928*eV, 2.940*eV, 2.951*eV, 2.959*eV, 2.967*eV, 2.978*eV, 2.986*eV, 2.993*eV,
            3.002*eV, 3.010*eV, 3.020*eV, 3.031*eV, 3.043*eV, 3.055*eV, 3.065*eV, 3.085*eV, 3.105*eV, 3.128*eV
        };

      const G4int nEntries = sizeof(photonEnergy)/sizeof(G4double);

      const G4int num = 2;
      G4double ephoton[num] = {2.482 *eV, 3.128 *eV};

    //
    //EJ200
    //

      G4double refractiveIndex1[] =
    { 1.58, 1.58};

      assert(sizeof(refractiveIndex1) == sizeof(ephoton));

      G4double absorption[] =
    { 3.80*m, 3.80*m};

      assert(sizeof(absorption) == sizeof(ephoton));

    G4double scintilFast[] =
      {
            0.056, 0.107, 0.138, 0.167, 0.211, 0.269, 0.331, 0.387, 0.429, 0.456, 
            0.498, 0.529, 0.580, 0.622, 0.671, 0.713, 0.780, 0.858, 0.904, 0.940, 
            0.976, 1.000, 0.987, 0.951, 0.884, 0.811, 0.731, 0.627, 0.549, 0.496, 
            0.400, 0.338, 0.280, 0.229, 0.160, 0.109, 0.060, 0.020, 0.007, 0.000, 
      };
      assert(sizeof(scintilFast) == sizeof(photonEnergy));


      G4MaterialPropertiesTable* mptEJ200 = new G4MaterialPropertiesTable();

        mptEJ200->AddProperty("RINDEX",       ephoton, refractiveIndex1,num)//折射率
              ->SetSpline(true);
        mptEJ200->AddProperty("ABSLENGTH",    ephoton, absorption,     num)//abslength吸收长度
              ->SetSpline(true);
        mptEJ200->AddProperty("FASTCOMPONENT",  photonEnergy, scintilFast,     nEntries)//fastcomponent快成分
              ->SetSpline(true);


        mptEJ200->AddConstProperty("SCINTILLATIONYIELD",11500./MeV);//addconstproperty添加晶体的本征属性(不随波长变化发生改变),scintillationyield光产额
        mptEJ200->AddConstProperty("RESOLUTIONSCALE",1.0);//resolutionscale本征分辨
        mptEJ200->AddConstProperty("FASTTIMECONSTANT", 2.1*ns);//快时间常数

        mptEJ200->AddConstProperty("YIELDRATIO",1.0);//快慢响应之比



      G4cout << "EJ-200 G4MaterialPropertiesTable" << G4endl;
        mptEJ200->DumpTable();

      ej200->SetMaterialPropertiesTable(mptEJ200);

    //
    // Air
    //

      G4double refractiveIndex2[] =
                {1.0, 1.0};

     G4MaterialPropertiesTable* mptAir = new G4MaterialPropertiesTable();
        mptAir->AddProperty("RINDEX", ephoton, refractiveIndex2, num);

      G4cout << "Air G4MaterialPropertiesTable" << G4endl;
        mptAir->DumpTable();

      Air->SetMaterialPropertiesTable(mptAir);
      
      
        G4double refractivePMMAIndex[] =
		{ 1.49, 1.49};
		
	G4MaterialPropertiesTable* mptPMMA = new G4MaterialPropertiesTable();
 	 mptPMMA->AddProperty("RINDEX",  ephoton, refractivePMMAIndex, num);
 	 
  	PMMA->SetMaterialPropertiesTable(mptPMMA);



  //  
  // ------------- Surfaces & Add Optical Surface --------------
  //

    //scintillator-reflectance

    G4OpticalSurface* opReflectorSurface = new G4OpticalSurface("opReflectorSurface");
      opReflectorSurface->SetType(dielectric_metal);
      opReflectorSurface->SetFinish(ground);
      opReflectorSurface->SetModel(unified);
      opReflectorSurface->SetSigmaAlpha(0.6);

    G4double reflectivityAl[num] = {0.92, 0.92};
    G4double specularlobe[num] = {0.9, 0.9}; // 漫反射光晕 
    G4double specularspike[num] = {0.0, 0.0}; // 镜面尖峰
    G4double backscatter[num] = {0.05, 0.05}; // 后向散射

    G4MaterialPropertiesTable *stAl = new G4MaterialPropertiesTable();
      stAl->AddProperty("REFLECTIVITY", ephoton, reflectivityAl, num);
      stAl->AddProperty("SPECULARLOBECONSTANT",ephoton,specularlobe,num);
      stAl->AddProperty("SPECULARSPIKECONSTANT",ephoton,specularspike,num);
      stAl->AddProperty("BACKSCATTERCONSTANT",ephoton,backscatter,num);
    stAl->DumpTable();  

    opReflectorSurface->SetMaterialPropertiesTable(stAl);
    opReflectorSurface->DumpInfo();

    new G4LogicalBorderSurface("Scintillation-Reflector",physShape, physref, opReflectorSurface);

      
    //PMT
      G4OpticalSurface* opPMTSurface = new G4OpticalSurface("opPMTSurface");
      
	// 1. 类型改成介质，只有介质才有全反射逻辑
	opPMTSurface->SetType(dielectric_dielectric); 

	// 2. 模型
	opPMTSurface->SetModel(unified);
	opPMTSurface->SetFinish(polished); // 抛光表面

	// 3. 定义属性
	// 反射率设为0，意味着“除了菲涅尔反射外，没有额外的人工强行反射”
	G4double pmtIndex[num] = {1.5, 1.5}; // 假设 PMT 窗口是玻璃，n=1.5
	
	G4MaterialPropertiesTable *myPMT2 = new G4MaterialPropertiesTable();
		myPMT2->AddProperty("RINDEX", ephoton, pmtIndex, num);

	opPMTSurface->SetMaterialPropertiesTable(myPMT2);



    new G4LogicalBorderSurface("PMT-scintillation",physShape,physCornerPMT1, opPMTSurface);
    new G4LogicalBorderSurface("PMT-scintillation",physShape,physCornerPMT2, opPMTSurface);
    new G4LogicalBorderSurface("PMT-scintillation",physShape,physCornerPMT3, opPMTSurface);
    new G4LogicalBorderSurface("PMT-scintillation",physShape,physCornerPMT4, opPMTSurface);


  
 //--------- Visualization attributes -------------------------------//设置颜色

	G4Colour silver (0.75, 0.75, 0.75, 0.2);
	G4Colour blue   (0.00, 1.00, 1.00, 0.3); 
	G4Colour grey   (0.5, 0.5, 0.5, 0.5);
	G4Colour gold   (1, 0.85, 0, 0.7);
	

        G4VisAttributes* shapeVisAtt = new G4VisAttributes(blue);
        	shapeVisAtt->SetForceSolid(true);
        	logicShape->SetVisAttributes(shapeVisAtt);
        
        G4VisAttributes* pmtVisAtt = new G4VisAttributes(G4Colour::Red());
        	pmtVisAtt->SetForceSolid(true); // 强制显示为实心
		logicPMT1->SetVisAttributes(pmtVisAtt);
		logicPMT2->SetVisAttributes(pmtVisAtt);
		logicPMT3->SetVisAttributes(pmtVisAtt);
		logicPMT4->SetVisAttributes(pmtVisAtt);
        
        G4VisAttributes* refVisAtt = new G4VisAttributes(gold);
        	//refVisAtt->SetForceSolid(true); // 强制显示为实心
        	logicref->SetVisAttributes(refVisAtt);
      
        G4UImanager* UIman = G4UImanager::GetUIpointer();
// 设置为白灰色
UIman->ApplyCommand("/vis/viewer/set/background 0.9 0.1 0.9");
         
       
  return physWorld;
}


