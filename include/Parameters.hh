#ifndef Parameters_h
#define Parameters_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"

// Overall geometry
static const G4double WORLD_XYZ = m;
static const G4double HEAD_R = 12.7 * mm; // radius of the head components
static const G4double MIRROR_ANGLE = 45. * deg;

// Materials
static const G4String WORLD_MATERIAL = "G4_AIR";
static const G4String VEW_MATERIAL = "G4_Ti"; // vacuum exit window
static const G4String SF1_MATERIAL = "G4_Ta"; // primary scattering foil
static const G4String SF2L_MATERIAL = "G4_Ta"; // secondary scattering foil large
static const G4String SF2S_MATERIAL = "G4_Ta"; // secondary scattering foil small
static const G4String MIRROR1_MATERIAL = "G4_MYLAR"; // light-field mirror upper
static const G4String MIRROR2_MATERIAL = "G4_Al"; // light-field mirror lower
static const G4String SP_MATERIAL = "G4_AIR"; // scoring plane
static const G4String PHANTOM_MATERIAL = "G4_WATER";

// Thicknesses of head components
static const G4double VEW_Z = 50. * um; 
static const G4double SF1_Z = 20. * um; 
static const G4double SF2L_Z = 50. * um; 
static const G4double SF2S_Z = 250. * um; 
static const G4double MIRROR1_Z = 60. * um; 
static const G4double MIRROR2_Z = 12. * um;

// Distances of head components from the source
static const G4double VEW_D = um;
static const G4double SF1_D = cm;
static const G4double SF2_D = 4.*cm;
static const G4double MIRROR_D = 20. * cm;
static const G4double PHSP_D = 49.9 * cm;
static const G4double PHANTOM_D = 50. * cm;

// Phase space scoring plane parameters
static const G4double PHSP_R = 5. * cm;

// Phantom geometry and parameters (common)
static const G4bool PHANTOM_DOSE = true; 
static const G4bool PHANTOM_PHASE_SPACE = false; // effective only if PHANTOM_DOSE is false
static const G4double SCORING_UNIT = mm;
static const G4double PHANTOM_Z = 5. * cm;
// Phantom for dosimetry
static const G4double PHANTOM_XY = 10. * cm;
static const G4int PHANTOM_N_XY = lround(PHANTOM_XY/SCORING_UNIT);
static const G4int PHANTOM_N_Z = lround(PHANTOM_Z/SCORING_UNIT);
static const G4int PHANTOM_N_VOXELS = PHANTOM_N_XY * PHANTOM_N_XY * PHANTOM_N_Z;
// Phantom for Phase Space scoring
static const G4double PHANTOM_R = 5. * cm;
static const G4int PHANTOM_NTUPLE_OFFSET = 1; // ntuple 0 is reserved for head SD
static const G4int N_PLANES = lround(PHANTOM_Z/SCORING_UNIT); // number of planes in the phantom, should be consistent with the actual geometry

#endif