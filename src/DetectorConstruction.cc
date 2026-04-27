#include "DetectorConstruction.hh"
#include "SensitiveDetector.hh"

#include "G4NistManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4PhysicalConstants.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4PVReplica.hh"
#include "G4Box.hh"
#include "G4Tubs.hh"
#include "G4Cons.hh"
#include "G4Trap.hh"
#include "G4VisAttributes.hh"
#include "G4SubtractionSolid.hh"
#include "G4SDManager.hh"

DetectorConstruction::DetectorConstruction() {}

DetectorConstruction::~DetectorConstruction() {}

G4VPhysicalVolume* DetectorConstruction::Construct() {
    DefineMaterials();

    G4bool checkOverlaps = true;

    // World
    G4double world_XYZ = m; // half length in x, y, z direction
    auto solidWorld = new G4Box("World", world_XYZ, world_XYZ, world_XYZ);
    auto logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, checkOverlaps);
    // no rotation, at (0,0,0), its logical volume, its name, its mother  volume, no boolean operation, copy number, overlaps checking
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Scattering Foils
    G4Material* SF1_material = Ta;
    G4double SF1_RMax = 12.7 * mm;
    G4double SF1_Dz = 10. * um; // half length
    G4double SF1_distance_from_source = 5.*cm;
    G4ThreeVector SF1_tlate(0., 0., -world_XYZ + SF1_distance_from_source + SF1_Dz);
    auto solidSF1 = new G4Tubs("Primary Scattering Foil", 0., SF1_RMax, SF1_Dz, 0., twopi);
    auto logicSF1 = new G4LogicalVolume(solidSF1, SF1_material, "Primary Scattering Foil");
    new G4PVPlacement(nullptr, SF1_tlate, logicSF1, "Primary Scattering Foil", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes SF1VisAtt(G4Colour::Red());
    SF1VisAtt.SetForceSolid();
    logicSF1->SetVisAttributes(SF1VisAtt);

    G4Material* SF2L_material = Ta;
    G4double SF2L_RMax = 12.7 * mm;
    G4double SF2L_Dz = 25. * um; // half length
    G4double SF2_distance_from_source = 10. * cm;
    G4ThreeVector SF2L_tlate(0., 0., -world_XYZ + SF2_distance_from_source + SF2L_Dz);
    auto solidSF2L = new G4Tubs("Secondary Scattering Foil Large", 0., SF2L_RMax, SF2L_Dz, 0., twopi);
    auto logicSF2L = new G4LogicalVolume(solidSF2L, SF2L_material, "Secondary Scattering Foil Large");
    new G4PVPlacement(nullptr, SF2L_tlate, logicSF2L, "Secondary Scattering Foil Large", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes SF2LVisAtt(G4Colour::Green());
    SF2LVisAtt.SetForceSolid();
    logicSF2L->SetVisAttributes(SF2LVisAtt);

    G4Material* SF2S_material = Ta;
    G4double SF2S_RMax = 6.35 * mm;
    G4double SF2S_Dz = 125. * um; // half length
    G4ThreeVector SF2S_tlate(0., 0., -world_XYZ + SF2_distance_from_source + 2.*SF2L_Dz + SF2S_Dz);
    auto solidSF2S = new G4Tubs("Secondary Scattering Foil Small", 0., SF2S_RMax, SF2S_Dz, 0., twopi);
    auto logicSF2S = new G4LogicalVolume(solidSF2S, SF2S_material, "Secondary Scattering Foil Small");
    new G4PVPlacement(nullptr, SF2S_tlate, logicSF2S, "Secondary Scattering Foil Small", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes SF2SVisAtt(G4Colour::Blue());
    SF2SVisAtt.SetForceSolid();
    logicSF2S->SetVisAttributes(SF2SVisAtt);

    // Scoring Plane
    G4Material *SP_material = air;
    G4double SP_RMax = 12.7 * mm;
    G4double SP_Dz = 10. * um; // half length
    G4double SP_distance_from_source = 11. * cm;
    G4ThreeVector SP_tlate(0., 0., -world_XYZ + SP_distance_from_source + SP_Dz);
    auto solidSP = new G4Tubs("Scoring Plane", 0., SP_RMax, SP_Dz, 0., twopi);
    auto logicSP = new G4LogicalVolume(solidSP, SP_material, "Scoring Plane");
    new G4PVPlacement(nullptr, SP_tlate, logicSP, "Scoring Plane", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes SPVisAtt(G4Colour::Yellow());
    SPVisAtt.SetForceSolid();
    logicSP->SetVisAttributes(SPVisAtt);
    scoringVolume = logicSP; // set the scoring volume for later use in SensitiveDetector

    // // Primary Collimator
    // G4double PC_RMin = 3.8*cm;
    // G4double PC_RMax = 37.72*cm;
    // G4double PC_Dz = 38.1*cm; // half length in z direction
    // G4double PC_SPhi = 0.*deg;
    // G4double PC_DPhi = 360.*deg;
    // G4ThreeVector PC_tlate(0., 0., 1.4*m);
    // auto PC_outer = new G4Tubs("PC outer", PC_RMin, PC_RMax, PC_Dz, PC_SPhi, PC_DPhi);
    // auto PC_cavity = new G4Cons("PC cavity", 0., 19.*cm, 0., PC_RMin, PC_Dz + 1.*mm, 0., 360. * deg);
    // auto solidPC = new G4SubtractionSolid("Primary Collimator", PC_outer, PC_cavity);
    // auto logicPC = new G4LogicalVolume(solidPC, W, "Primary Collimator");
    // new G4PVPlacement(nullptr, PC_tlate, logicPC, "Primary Collimator", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes PCVisAtt(G4Colour(1., 0., 1., 1.));
    // // PCVisAtt.SetForceCloud(false);
    // PCVisAtt.SetForceSolid();
    // logicPC->SetVisAttributes(PCVisAtt);

    // // Ion Chamber
    // G4double IC_RMin = 0.;
    // G4double IC_RMax = 35 * cm;
    // G4double IC_Dz = 4.5 * cm; // half length in z direction
    // G4double IC_SPhi = 0. * deg;
    // G4double IC_DPhi = 360. * deg;
    // G4ThreeVector IC_tlate(0., 0., .7 * m);
    // auto solidIC = new G4Tubs("Ion Chamber", IC_RMin, IC_RMax, IC_Dz, IC_SPhi, IC_DPhi);
    // auto logicIC = new G4LogicalVolume(solidIC, air, "Ion Chamber");
    // new G4PVPlacement(nullptr, IC_tlate, logicIC, "Ion Chamber", logicWorld, false, 0, checkOverlaps);
    // logicIC->SetVisAttributes(G4VisAttributes::GetInvisible());
    // G4double Kapton_thickness = 25. * um;
    // auto solidICRep = new G4Tubs("Ion Chamber Rep", IC_RMin, IC_RMax, .5 * Kapton_thickness, 0. * deg, 360. * deg);
    // G4LogicalVolume *logicICRep = new G4LogicalVolume(solidICRep, Kapton, "Ion Chamber Rep");
    // new G4PVReplica("Ion Chamber Rep", logicICRep, logicIC, kZAxis, 6, 1.5 * cm, 0.);
    // G4VisAttributes ICVisAtt(G4Colour::Yellow());
    // ICVisAtt.SetForceWireframe();
    // logicICRep->SetVisAttributes(ICVisAtt);

    // // Mirror
    // G4double mirror_XY = 22.098 * cm; // half length in x and y direction
    // G4double mirror_Z = .0508/2. * cm; // half length in z direction
    // G4ThreeVector mirror_tlate(0., 0., .5 * m);
    // auto mirrorRot = new G4RotationMatrix();
    // mirrorRot->rotateX(36. * deg);
    // auto solidMirror = new G4Box("Mirror", mirror_XY, mirror_XY, mirror_Z);
    // auto logicMirror = new G4LogicalVolume(solidMirror, Mylar, "Mirror");
    // new G4PVPlacement(mirrorRot, mirror_tlate, logicMirror, "Mirror", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes mirrorVisAtt(G4Colour::Grey());
    // mirrorVisAtt.SetForceSolid();
    // logicMirror->SetVisAttributes(mirrorVisAtt);

    // // Jaws
    // G4double jaw_Z = 60.*cm, jaw_Y = 30.*cm, jaw_X = 30.*cm, jaw_LTX = 20.*cm;
    // G4ThreeVector jaw_pos1(-20. * cm, 0., .1 * m);
    // auto jawRot1 = new G4RotationMatrix();
    // jawRot1->rotateX(pi/2.);
    // auto solidJaw1 = new G4Trap("Jaw1", jaw_Z, jaw_Y, jaw_X, jaw_LTX);
    // auto logicJaw1 = new G4LogicalVolume(solidJaw1, tungstenAlloy, "Jaw1");
    // new G4PVPlacement(jawRot1, jaw_pos1, logicJaw1, "Jaw1", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes jawVisAtt1(G4Colour::Red());
    // jawVisAtt1.SetForceSolid();
    // logicJaw1->SetVisAttributes(jawVisAtt1);

    // G4ThreeVector jaw_pos2(20. * cm, 0., .1 * m);
    // auto jawRot2 = new G4RotationMatrix();
    // jawRot2->rotateX(pi / 2.);
    // jawRot2->rotateY(pi);
    // auto solidJaw2 = new G4Trap("Jaw2", jaw_Z, jaw_Y, jaw_X, jaw_LTX);
    // auto logicJaw2 = new G4LogicalVolume(solidJaw2, tungstenAlloy, "Jaw2");
    // new G4PVPlacement(jawRot2, jaw_pos2, logicJaw2, "Jaw2", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes jawVisAtt2(G4Colour::Magenta());
    // jawVisAtt2.SetForceSolid();
    // logicJaw2->SetVisAttributes(jawVisAtt2);

    // G4ThreeVector jaw_pos3(0, -20. * cm, -.25 * m);
    // auto jawRot3 = new G4RotationMatrix();
    // jawRot3->rotateX(pi / 2.);
    // jawRot3->rotateY(pi/2.);
    // auto solidJaw3 = new G4Trap("Jaw3", jaw_Z, jaw_Y, jaw_X, jaw_LTX);
    // auto logicJaw3 = new G4LogicalVolume(solidJaw3, tungstenAlloy, "Jaw3");
    // new G4PVPlacement(jawRot3, jaw_pos3, logicJaw3, "Jaw3", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes jawVisAtt3(G4Colour::Blue());
    // jawVisAtt3.SetForceSolid();
    // logicJaw3->SetVisAttributes(jawVisAtt3);

    // G4ThreeVector jaw_pos4(0, 20. * cm, -.25 * m);
    // auto jawRot4 = new G4RotationMatrix();
    // jawRot4->rotateX(pi / 2.);
    // jawRot4->rotateY(-pi / 2.);
    // auto solidJaw4 = new G4Trap("Jaw4", jaw_Z, jaw_Y, jaw_X, jaw_LTX);
    // auto logicJaw4 = new G4LogicalVolume(solidJaw4, tungstenAlloy, "Jaw4");
    // new G4PVPlacement(jawRot4, jaw_pos4, logicJaw4, "Jaw4", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes jawVisAtt4(G4Colour::Cyan());
    // jawVisAtt4.SetForceSolid();
    // logicJaw4->SetVisAttributes(jawVisAtt4);

    // // Plastic Window
    // G4double PW_XY = 30. * cm; // half length in x and y direction
    // G4double PW_Z = .5 * cm; // half length in z direction
    // G4ThreeVector PW_tlate(0., 0., -.5 * m);
    // auto solidPW = new G4Box("Plastic Window", PW_XY, PW_XY, PW_Z);
    // auto logicPW = new G4LogicalVolume(solidPW, Mylar, "Plastic Window");
    // new G4PVPlacement(nullptr, PW_tlate, logicPW, "Plastic Window", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes PWVisAtt(G4Colour::Grey());
    // PWVisAtt.SetForceSolid();
    // logicPW->SetVisAttributes(PWVisAtt);

    // // Scrapers
    // G4double scraper1_XY = 30. * cm; // half length in x and y direction
    // G4double scraper1_Z = 5. * cm; // half length in z direction
    // G4double scraper1_inner_XY = scraper1_XY - 10.*cm; // half length in x and y direction of cavity
    // G4ThreeVector scraper1_tlate(0., 0., -.8*m);
    // auto scraper1_outer = new G4Box("Scraper 1 outer", scraper1_XY, scraper1_XY, scraper1_Z);
    // auto scraper1_cavity = new G4Box("Scraper 1 cavity", scraper1_inner_XY, scraper1_inner_XY, scraper1_Z + 1.*mm);
    // auto solidScraper1 = new G4SubtractionSolid("Scraper 1", scraper1_outer, scraper1_cavity);
    // auto logicScraper1 = new G4LogicalVolume(solidScraper1, Al, "Scraper 1");
    // new G4PVPlacement(nullptr, scraper1_tlate, logicScraper1, "Scraper 1", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes scraper1VisAtt(G4Colour::Green());
    // scraper1VisAtt.SetForceSolid();
    // logicScraper1->SetVisAttributes(scraper1VisAtt);

    // G4double scraper2_XY = 29. * cm;       // half length in x and y direction
    // G4double scraper2_Z = 5. * cm;         // half length in z direction
    // G4double scraper2_inner_XY = scraper2_XY - 10.*cm; // half length in x and y direction of cavity
    // G4ThreeVector scraper2_tlate(0., 0., -1.1 * m);
    // auto scraper2_outer = new G4Box("Scraper 2 outer", scraper2_XY, scraper2_XY, scraper2_Z);
    // auto scraper2_cavity = new G4Box("Scraper 2 cavity", scraper2_inner_XY, scraper2_inner_XY, scraper2_Z + 1. * mm);
    // auto solidScraper2 = new G4SubtractionSolid("Scraper 2", scraper2_outer, scraper2_cavity);
    // auto logicScraper2 = new G4LogicalVolume(solidScraper2, Al, "Scraper 2");
    // new G4PVPlacement(nullptr, scraper2_tlate, logicScraper2, "Scraper 2", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes scraper2VisAtt(G4Colour::Green());
    // scraper2VisAtt.SetForceSolid();
    // logicScraper2->SetVisAttributes(scraper2VisAtt);

    // G4double scraper3_XY = 28. * cm;       // half length in x and y direction
    // G4double scraper3_Z = 5. * cm;         // half length in z direction
    // G4double scraper3_inner_XY = scraper3_XY - 10.*cm; // half length in x and y direction of cavity
    // G4ThreeVector scraper3_tlate(0., 0., -1.4 * m);
    // auto scraper3_outer = new G4Box("Scraper 3 outer", scraper3_XY, scraper3_XY, scraper3_Z);
    // auto scraper3_cavity = new G4Box("Scraper 3 cavity", scraper3_inner_XY, scraper3_inner_XY, scraper3_Z + 1. * mm);
    // auto solidScraper3 = new G4SubtractionSolid("Scraper 3", scraper3_outer, scraper3_cavity);
    // auto logicScraper3 = new G4LogicalVolume(solidScraper3, Al, "Scraper 3");
    // new G4PVPlacement(nullptr, scraper3_tlate, logicScraper3, "Scraper 3", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes scraper3VisAtt(G4Colour::Green());
    // scraper3VisAtt.SetForceSolid();
    // logicScraper3->SetVisAttributes(scraper3VisAtt);

    // // Cutout
    // G4double cutout_XY = scraper3_inner_XY; // half length in x and y direction
    // G4double cutout_Z = 2.5 * cm; // half length in z direction
    // G4double cutout_inner_XY = cutout_XY - 10.*cm; // half length in x and y direction of cavity
    // G4ThreeVector cutout_tlate(0., 0., scraper3_tlate.z() - scraper3_Z + cutout_Z);
    // auto cutout_outer = new G4Box("Cutout outer", cutout_XY, cutout_XY, cutout_Z);
    // auto cutout_cavity = new G4Box("Cutout cavity", cutout_inner_XY, cutout_inner_XY, cutout_Z + 1. * mm);
    // auto solidCutout = new G4SubtractionSolid("Cutout", cutout_outer, cutout_cavity);
    // auto logicCutout = new G4LogicalVolume(solidCutout, Al, "Cutout");
    // new G4PVPlacement(nullptr, cutout_tlate, logicCutout, "Cutout", logicWorld, false, 0, checkOverlaps);
    // G4VisAttributes cutoutVisAtt(G4Colour::Yellow());
    // cutoutVisAtt.SetForceSolid();
    // logicCutout->SetVisAttributes(cutoutVisAtt);

    return physWorld;
}


void DetectorConstruction::ConstructSDandField() {
    G4SDManager *sdManager = G4SDManager::GetSDMpointer();
    SensitiveDetector *sd = new SensitiveDetector("phasespace");
    sdManager->AddNewDetector(sd);
    scoringVolume->SetSensitiveDetector(sd);
}


void DetectorConstruction::DefineMaterials(){
    G4NistManager *nist = G4NistManager::Instance();

    Al = nist->FindOrBuildMaterial("G4_Al"); // aluminium (13)
    Cu = nist->FindOrBuildMaterial("G4_Cu"); // copper (29)
    Ta = nist->FindOrBuildMaterial("G4_Ta"); // tantalum (73)
    W = nist->FindOrBuildMaterial("G4_W");   // tungsten (74)
    Au = nist->FindOrBuildMaterial("G4_Au"); // gold (79)
    air = nist->FindOrBuildMaterial("G4_AIR");
    mylar = nist->FindOrBuildMaterial("G4_MYLAR");
    kapton = nist->FindOrBuildMaterial("G4_KAPTON");

    // G4Element *elH = nist->FindOrBuildElement("H");
    // G4Element *elC = nist->FindOrBuildElement("C");
    // G4Element *elN = nist->FindOrBuildElement("N");
    // G4Element *elO = nist->FindOrBuildElement("O");
    // G4Element *elFe = nist->FindOrBuildElement("Fe");
    // G4Element *elNi = nist->FindOrBuildElement("Ni");
    // G4Element *elW = nist->FindOrBuildElement("W");

    // G4Material *tungstenAlloy = new G4Material("tungsten alloy", 18. * g / cm3, 3);
    // tungstenAlloy->AddElement(elW, 90);
    // tungstenAlloy->AddElement(elNi, 7);
    // tungstenAlloy->AddElement(elFe, 3);
}