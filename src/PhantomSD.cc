#include "PhantomSD.hh"
#include "Parameters.hh"

#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4NistManager.hh"


G4Mutex PhantomSD::fMutex = G4MUTEX_INITIALIZER;
std::vector<G4double> PhantomSD::runEnergyDeposits(PHANTOM_N_VOXELS, 0.);


PhantomSD::PhantomSD(const G4String &name) : G4VSensitiveDetector(name), eventEnergyDeposits(PHANTOM_N_VOXELS, 0.) {

    G4double density = G4NistManager::Instance()->FindOrBuildMaterial(PHANTOM_MATERIAL)->GetDensity(); // g/mm^3
    voxelMass = density*std::pow(SCORING_UNIT, 3); // g
}


PhantomSD::~PhantomSD() {}


void PhantomSD::Initialize(G4HCofThisEvent *){
    std::fill(eventEnergyDeposits.begin(), eventEnergyDeposits.end(), 0.); // Reset per-event buffer at start of each event
}


G4bool PhantomSD::ProcessHits(G4Step *step, G4TouchableHistory *){
    G4double totalEnergyDeposit = step->GetTotalEnergyDeposit();
    if(totalEnergyDeposit == 0.)
        return false;

    const G4VTouchable *touchable = step->GetPreStepPoint()->GetTouchable();
    G4int ix = touchable->GetCopyNumber(0);
    G4int iy = touchable->GetCopyNumber(1);
    G4int iz = touchable->GetCopyNumber(2);
    if (ix < 0 || ix >= PHANTOM_N_XY || iy < 0 || iy >= PHANTOM_N_XY || iz < 0 || iz >= PHANTOM_N_Z){
        G4ExceptionDescription msg;
        msg << "Voxel index out of range: " << "ix=" << ix << " iy=" << iy << " iz=" << iz;
        G4Exception("PhantomSD::ProcessHits", "PhantomSD001", JustWarning, msg);
        return false;
    }
    
    eventEnergyDeposits[ix + PHANTOM_N_XY * (iy + PHANTOM_N_XY * iz)] += totalEnergyDeposit;
    return true;
}


void PhantomSD::EndOfEvent(G4HCofThisEvent *){ // Merge per-event buffer into run total — mutex for MT safety
    G4AutoLock lock(&fMutex);
    for(G4int i=0; i<PHANTOM_N_VOXELS; ++i)
        runEnergyDeposits[i] += eventEnergyDeposits[i];
}


void PhantomSD::Write3DDose(const G4String &filename) const{
    G4double maxEnergyDeposit = *std::max_element(runEnergyDeposits.begin(), runEnergyDeposits.end());
    if (maxEnergyDeposit == 0.)
        return;

    std::ofstream out(filename);
    out << "x (mm),y (mm),Depth (mm),Relative Dose\n";
    out << std::fixed << std::setprecision(4);

    for (G4int iz = 0; iz < PHANTOM_N_Z; ++iz){
        G4double depth = (PHANTOM_N_Z - 1 - iz + .5) * SCORING_UNIT;
        for (G4int iy = 0; iy < PHANTOM_N_XY; ++iy){
            G4double y = (-PHANTOM_N_XY / 2. + iy + .5) * SCORING_UNIT;
            for (G4int ix = 0; ix < PHANTOM_N_XY; ++ix){
                G4double x = (-PHANTOM_N_XY / 2. + ix + .5) * SCORING_UNIT;
                G4double dose = runEnergyDeposits[ix + PHANTOM_N_XY * (iy + PHANTOM_N_XY * iz)] / maxEnergyDeposit;
                if (dose > 1e-9) // skip near-zero voxels
                    out << x << "," << y << "," << depth << "," << dose << "\n";
            }
        }
    }
    out.close();
    G4cout << "PhantomSD: 3D dose written to " << filename << G4endl;
}