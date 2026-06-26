#ifndef Parameters_h
#define Parameters_h 1

#include "globals.hh"
#include "G4SystemOfUnits.hh"
#include "G4Threading.hh"

// Run parameters
static const G4int SEED = 1; // 1 ≤ seed ≤ 900,000,000
static const G4int N_THREADS = lround(ceil(G4float(G4Threading::G4GetNumberOfCores())/2.));
static const G4int PRINT_EVENT_MODULUS = 1000000;
static const G4bool PHSP_SOURCE = true;
static const G4String PHSP_FILENAME = "phsp_1G.root";
static const G4String PHSP_TREENAME = "phsp_head";
static const G4bool PHSP_SCORING = false;
static const G4bool PHANTOM = true;

// Beam Source Parameters (primary generator)
static const G4double BEAM_ENERGY = 9.9 * MeV;
static const G4double BEAM_SIGMA_E = .2 * MeV;
static const G4double BEAM_SOURCE_RADIUS = 3. * mm;
static const G4double BEAM_SIGMA_X = mm;
static const G4double BEAM_SIGMA_Y = mm;
static const G4double BEAM_SIGMA_ANG_X = 0. * deg;
static const G4double BEAM_SIGMA_ANG_Y = 0. * deg;

// Overall geometry
static const G4double WORLD_XYZ = m;
static const G4double HEAD_R = 12.7 * mm; // radius of the head components
static const G4double SF2S_R = 12.7/2. * mm; // radius of the head components
static const G4double MIRROR_ANGLE = 45. * deg;

// Materials
static const G4String WORLD_MATERIAL = "G4_AIR";
static const G4String VEW_MATERIAL = "G4_Ti"; // vacuum exit window
static const G4String SF1_MATERIAL = "G4_Ta"; // primary scattering foil
static const G4String SF2L_MATERIAL = "G4_Al"; // secondary scattering foil large
static const G4String SF2S_MATERIAL = "G4_Al"; // secondary scattering foil small
static const G4String MIRROR1_MATERIAL = "G4_MYLAR"; // light-field mirror upper
static const G4String MIRROR2_MATERIAL = "G4_Al"; // light-field mirror lower
static const G4String SP_MATERIAL = "G4_AIR"; // scoring plane
static const G4String PHANTOM_MATERIAL = "G4_WATER";

// Thicknesses of head components
static const G4double VEW_Z = 50. * um; 
static const G4double SF1_Z = 25.4 * um; 
static const G4double SF2L_Z = 50.8 * um; 
static const G4double SF2S_Z = 354. * um; 
static const G4double MIRROR1_Z = 60. * um; 
static const G4double MIRROR2_Z = 12. * um;

// Distances of head components from the source
static const G4double VEW_D = um;
static const G4double SF1_D = .5 * cm;
static const G4double SF2_D = 5. * cm;
static const G4double MIRROR_D = 9. * cm;
static const G4double PHSP_D = 45.5 * cm;
static const G4double PHANTOM_D = 50. * cm;

// Phase space scoring plane parameters
static const G4double PHSP_R = 12.5 * cm;
static const G4double PHSP_Z = cm;

// Phantom parameters
static const G4double SCORING_UNIT = mm;
static const G4double PHANTOM_XY = 25 * cm;
static const G4double PHANTOM_Z = 8. * cm;
static const G4int PHANTOM_N_XY = lround(PHANTOM_XY/SCORING_UNIT);
static const G4int PHANTOM_N_Z = lround(PHANTOM_Z/SCORING_UNIT);
static const G4int PHANTOM_N_VOXELS = PHANTOM_N_XY * PHANTOM_N_XY * PHANTOM_N_Z;

#endif