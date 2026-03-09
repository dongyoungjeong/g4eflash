#include "DetectorConstruction.hh"

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

G4VPhysicalVolume* DetectorConstruction::Construct() {
    G4NistManager* nist = G4NistManager::Instance();
    G4bool checkOverlaps = true;

    // Materials
    G4Material *air = nist->FindOrBuildMaterial("G4_AIR");
    G4Material *Al = nist->FindOrBuildMaterial("G4_Al"); // aluminum (13)
    G4Material *Cu = nist->FindOrBuildMaterial("G4_Cu"); // copper (29)
    G4Material *Ta = nist->FindOrBuildMaterial("G4_Ta"); // tantalum (73)
    G4Material *W = nist->FindOrBuildMaterial("G4_W"); // tungsten (74)
    G4Material *Au = nist->FindOrBuildMaterial("G4_Au"); // gold (79)
    G4Element *elH = nist->FindOrBuildElement("H");
    G4Element *elC = nist->FindOrBuildElement("C");
    G4Element *elN = nist->FindOrBuildElement("N");
    G4Element *elO = nist->FindOrBuildElement("O");
    G4Material *Kapton = new G4Material("Kapton", 1.42 * g / cm3, 4);
    Kapton->AddElement(elH, 22);
    Kapton->AddElement(elC, 10);
    Kapton->AddElement(elN, 2);
    Kapton->AddElement(elO, 5);
    G4Material *Mylar = nist->FindOrBuildMaterial("G4_MYLAR");
    G4Element *elW = nist->FindOrBuildElement("W");
    G4Element *elNi = nist->FindOrBuildElement("Ni");
    G4Element *elFe = nist->FindOrBuildElement("Fe");
    G4Material *tungstenAlloy = new G4Material("tungsten alloy", 18. * g / cm3, 3);
    tungstenAlloy->AddElement(elW, 90);
    tungstenAlloy->AddElement(elNi, 7);
    tungstenAlloy->AddElement(elFe, 3);

    // World
    G4double world_XYZ = 2.*m; // half length in x, y, z direction
    auto solidWorld = new G4Box("World", world_XYZ, world_XYZ, world_XYZ);
    auto logicWorld = new G4LogicalVolume(solidWorld, air, "World");
    auto physWorld = new G4PVPlacement(nullptr, G4ThreeVector(), logicWorld, "World", nullptr, false, 0, checkOverlaps);
    // no rotation, at (0,0,0), its logical volume, its name, its mother  volume, no boolean operation, copy number, overlaps checking
    logicWorld->SetVisAttributes(G4VisAttributes::GetInvisible());

    // Scattering Foils
    G4double SF1_XY = 20. * cm; // half length
    G4double SF1_Z = 1. * mm;   // half length
    G4ThreeVector SF1_tlate(0., 0., 1.95 * m);
    auto solidSF1 = new G4Box("Scattering Foil 1", SF1_XY, SF1_XY, SF1_Z);
    auto logicSF1 = new G4LogicalVolume(solidSF1, Au, "Scattering Foil 1");
    new G4PVPlacement(nullptr, SF1_tlate, logicSF1, "Scattering Foil 1", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes SFVisAtt1(G4Colour::White());
    SFVisAtt1.SetForceSolid();
    logicSF1->SetVisAttributes(SFVisAtt1);

    G4double SF2_XY = 20. * cm; // half length
    G4double SF2_Z = 5. * mm;   // half length
    G4ThreeVector SF2_tlate(0., 0., 1.9 * m);
    auto solidSF2 = new G4Box("Scattering Foil 2", SF2_XY, SF2_XY, SF2_Z);
    auto logicSF2 = new G4LogicalVolume(solidSF2, Al, "Scattering Foil 2");
    new G4PVPlacement(nullptr, SF2_tlate, logicSF2, "Scattering Foil 2", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes SFVisAtt2(G4Colour::Cyan());
    SFVisAtt2.SetForceSolid();
    logicSF2->SetVisAttributes(SFVisAtt2);

    // Primary Collimator
    G4double PC_RMin = 3.8*cm;
    G4double PC_RMax = 37.72*cm;
    G4double PC_Dz = 38.1*cm; // half length in z direction
    G4double PC_SPhi = 0.*deg;
    G4double PC_DPhi = 360.*deg;
    G4ThreeVector PC_tlate(0., 0., 1.4*m);
    auto PC_outer = new G4Tubs("PC outer", PC_RMin, PC_RMax, PC_Dz, PC_SPhi, PC_DPhi);
    auto PC_cavity = new G4Cons("PC cavity", 0., 19.*cm, 0., PC_RMin, PC_Dz + 1.*mm, 0., 360. * deg);
    auto solidPC = new G4SubtractionSolid("Primary Collimator", PC_outer, PC_cavity);
    auto logicPC = new G4LogicalVolume(solidPC, W, "Primary Collimator");
    new G4PVPlacement(nullptr, PC_tlate, logicPC, "Primary Collimator", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes PCVisAtt(G4Colour(1., 0., 1., 1.));
    // PCVisAtt.SetForceCloud(false);
    PCVisAtt.SetForceSolid();
    logicPC->SetVisAttributes(PCVisAtt);

    // Ion Chamber
    G4double IC_RMin = 0.;
    G4double IC_RMax = 35 * cm;
    G4double IC_Dz = 4.5 * cm; // half length in z direction
    G4double IC_SPhi = 0. * deg;
    G4double IC_DPhi = 360. * deg;
    G4ThreeVector IC_tlate(0., 0., .7 * m);
    auto solidIC = new G4Tubs("Ion Chamber", IC_RMin, IC_RMax, IC_Dz, IC_SPhi, IC_DPhi);
    auto logicIC = new G4LogicalVolume(solidIC, air, "Ion Chamber");
    new G4PVPlacement(nullptr, IC_tlate, logicIC, "Ion Chamber", logicWorld, false, 0, checkOverlaps);
    logicIC->SetVisAttributes(G4VisAttributes::GetInvisible());
    G4double Kapton_thickness = 25. * um;
    auto solidICRep = new G4Tubs("Ion Chamber Rep", IC_RMin, IC_RMax, .5 * Kapton_thickness, 0. * deg, 360. * deg);
    G4LogicalVolume *logicICRep = new G4LogicalVolume(solidICRep, Kapton, "Ion Chamber Rep");
    new G4PVReplica("Ion Chamber Rep", logicICRep, logicIC, kZAxis, 6, 1.5 * cm, 0.);
    G4VisAttributes ICVisAtt(G4Colour::Yellow());
    ICVisAtt.SetForceWireframe();
    logicICRep->SetVisAttributes(ICVisAtt);

    // Mirror
    G4double mirror_XY = 22.098 * cm; // half length in x and y direction
    G4double mirror_Z = .0508/2. * cm; // half length in z direction
    G4ThreeVector mirror_tlate(0., 0., .5 * m);
    auto mirrorRot = new G4RotationMatrix();
    mirrorRot->rotateX(36. * deg);
    auto solidMirror = new G4Box("Mirror", mirror_XY, mirror_XY, mirror_Z);
    auto logicMirror = new G4LogicalVolume(solidMirror, Mylar, "Mirror");
    new G4PVPlacement(mirrorRot, mirror_tlate, logicMirror, "Mirror", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes mirrorVisAtt(G4Colour::Grey());
    mirrorVisAtt.SetForceSolid();
    logicMirror->SetVisAttributes(mirrorVisAtt);

    // Jaws
    G4double jaw_Z = 60.*cm, jaw_Y = 30.*cm, jaw_X = 30.*cm, jaw_LTX = 20.*cm;
    G4ThreeVector jaw_pos1(-20. * cm, 0., .1 * m);
    auto jawRot1 = new G4RotationMatrix();
    jawRot1->rotateX(pi/2.);
    auto solidJaw1 = new G4Trap("Jaw1", jaw_Z, jaw_Y, jaw_X, jaw_LTX);
    auto logicJaw1 = new G4LogicalVolume(solidJaw1, tungstenAlloy, "Jaw1");
    new G4PVPlacement(jawRot1, jaw_pos1, logicJaw1, "Jaw1", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes jawVisAtt1(G4Colour::Red());
    jawVisAtt1.SetForceSolid();
    logicJaw1->SetVisAttributes(jawVisAtt1);

    G4ThreeVector jaw_pos2(20. * cm, 0., .1 * m);
    auto jawRot2 = new G4RotationMatrix();
    jawRot2->rotateX(pi / 2.);
    jawRot2->rotateY(pi);
    auto solidJaw2 = new G4Trap("Jaw2", jaw_Z, jaw_Y, jaw_X, jaw_LTX);
    auto logicJaw2 = new G4LogicalVolume(solidJaw2, tungstenAlloy, "Jaw2");
    new G4PVPlacement(jawRot2, jaw_pos2, logicJaw2, "Jaw2", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes jawVisAtt2(G4Colour::Magenta());
    jawVisAtt2.SetForceSolid();
    logicJaw2->SetVisAttributes(jawVisAtt2);

    G4ThreeVector jaw_pos3(0, -20. * cm, -.25 * m);
    auto jawRot3 = new G4RotationMatrix();
    jawRot3->rotateX(pi / 2.);
    jawRot3->rotateY(pi/2.);
    auto solidJaw3 = new G4Trap("Jaw3", jaw_Z, jaw_Y, jaw_X, jaw_LTX);
    auto logicJaw3 = new G4LogicalVolume(solidJaw3, tungstenAlloy, "Jaw3");
    new G4PVPlacement(jawRot3, jaw_pos3, logicJaw3, "Jaw3", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes jawVisAtt3(G4Colour::Blue());
    jawVisAtt3.SetForceSolid();
    logicJaw3->SetVisAttributes(jawVisAtt3);

    G4ThreeVector jaw_pos4(0, 20. * cm, -.25 * m);
    auto jawRot4 = new G4RotationMatrix();
    jawRot4->rotateX(pi / 2.);
    jawRot4->rotateY(-pi / 2.);
    auto solidJaw4 = new G4Trap("Jaw4", jaw_Z, jaw_Y, jaw_X, jaw_LTX);
    auto logicJaw4 = new G4LogicalVolume(solidJaw4, tungstenAlloy, "Jaw4");
    new G4PVPlacement(jawRot4, jaw_pos4, logicJaw4, "Jaw4", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes jawVisAtt4(G4Colour::Cyan());
    jawVisAtt4.SetForceSolid();
    logicJaw4->SetVisAttributes(jawVisAtt4);

    // Plastic Window
    G4double PW_XY = 30. * cm; // half length in x and y direction
    G4double PW_Z = .5 * cm; // half length in z direction
    G4ThreeVector PW_tlate(0., 0., -.5 * m);
    auto solidPW = new G4Box("Plastic Window", PW_XY, PW_XY, PW_Z);
    auto logicPW = new G4LogicalVolume(solidPW, Mylar, "Plastic Window");
    new G4PVPlacement(nullptr, PW_tlate, logicPW, "Plastic Window", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes PWVisAtt(G4Colour::Grey());
    PWVisAtt.SetForceSolid();
    logicPW->SetVisAttributes(PWVisAtt);

    // Scrapers
    G4double scraper1_XY = 30. * cm; // half length in x and y direction
    G4double scraper1_Z = 5. * cm; // half length in z direction
    G4double scraper1_inner_XY = scraper1_XY - 10.*cm; // half length in x and y direction of cavity
    G4ThreeVector scraper1_tlate(0., 0., -.8*m);
    auto scraper1_outer = new G4Box("Scraper 1 outer", scraper1_XY, scraper1_XY, scraper1_Z);
    auto scraper1_cavity = new G4Box("Scraper 1 cavity", scraper1_inner_XY, scraper1_inner_XY, scraper1_Z + 1.*mm);
    auto solidScraper1 = new G4SubtractionSolid("Scraper 1", scraper1_outer, scraper1_cavity);
    auto logicScraper1 = new G4LogicalVolume(solidScraper1, Al, "Scraper 1");
    new G4PVPlacement(nullptr, scraper1_tlate, logicScraper1, "Scraper 1", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes scraper1VisAtt(G4Colour::Green());
    scraper1VisAtt.SetForceSolid();
    logicScraper1->SetVisAttributes(scraper1VisAtt);

    G4double scraper2_XY = 29. * cm;       // half length in x and y direction
    G4double scraper2_Z = 5. * cm;         // half length in z direction
    G4double scraper2_inner_XY = scraper2_XY - 10.*cm; // half length in x and y direction of cavity
    G4ThreeVector scraper2_tlate(0., 0., -1.1 * m);
    auto scraper2_outer = new G4Box("Scraper 2 outer", scraper2_XY, scraper2_XY, scraper2_Z);
    auto scraper2_cavity = new G4Box("Scraper 2 cavity", scraper2_inner_XY, scraper2_inner_XY, scraper2_Z + 1. * mm);
    auto solidScraper2 = new G4SubtractionSolid("Scraper 2", scraper2_outer, scraper2_cavity);
    auto logicScraper2 = new G4LogicalVolume(solidScraper2, Al, "Scraper 2");
    new G4PVPlacement(nullptr, scraper2_tlate, logicScraper2, "Scraper 2", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes scraper2VisAtt(G4Colour::Green());
    scraper2VisAtt.SetForceSolid();
    logicScraper2->SetVisAttributes(scraper2VisAtt);

    G4double scraper3_XY = 28. * cm;       // half length in x and y direction
    G4double scraper3_Z = 5. * cm;         // half length in z direction
    G4double scraper3_inner_XY = scraper3_XY - 10.*cm; // half length in x and y direction of cavity
    G4ThreeVector scraper3_tlate(0., 0., -1.4 * m);
    auto scraper3_outer = new G4Box("Scraper 3 outer", scraper3_XY, scraper3_XY, scraper3_Z);
    auto scraper3_cavity = new G4Box("Scraper 3 cavity", scraper3_inner_XY, scraper3_inner_XY, scraper3_Z + 1. * mm);
    auto solidScraper3 = new G4SubtractionSolid("Scraper 3", scraper3_outer, scraper3_cavity);
    auto logicScraper3 = new G4LogicalVolume(solidScraper3, Al, "Scraper 3");
    new G4PVPlacement(nullptr, scraper3_tlate, logicScraper3, "Scraper 3", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes scraper3VisAtt(G4Colour::Green());
    scraper3VisAtt.SetForceSolid();
    logicScraper3->SetVisAttributes(scraper3VisAtt);

    // Cutout
    G4double cutout_XY = scraper3_inner_XY; // half length in x and y direction
    G4double cutout_Z = 2.5 * cm; // half length in z direction
    G4double cutout_inner_XY = cutout_XY - 10.*cm; // half length in x and y direction of cavity
    G4ThreeVector cutout_tlate(0., 0., scraper3_tlate.z() - scraper3_Z + cutout_Z);
    auto cutout_outer = new G4Box("Cutout outer", cutout_XY, cutout_XY, cutout_Z);
    auto cutout_cavity = new G4Box("Cutout cavity", cutout_inner_XY, cutout_inner_XY, cutout_Z + 1. * mm);
    auto solidCutout = new G4SubtractionSolid("Cutout", cutout_outer, cutout_cavity);
    auto logicCutout = new G4LogicalVolume(solidCutout, Al, "Cutout");
    new G4PVPlacement(nullptr, cutout_tlate, logicCutout, "Cutout", logicWorld, false, 0, checkOverlaps);
    G4VisAttributes cutoutVisAtt(G4Colour::Yellow());
    cutoutVisAtt.SetForceSolid();
    logicCutout->SetVisAttributes(cutoutVisAtt);

    return physWorld;
}