#ifndef PhaseSpaceGeneratorAction_h
#define PhaseSpaceGeneratorAction_h

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"


class G4Event;
class G4GeneralParticleSource;
class G4SPSPosDistribution;
class G4SPSAngDistribution;
class G4SPSEneDistribution;

class PhaseSpaceGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PhaseSpaceGeneratorAction(const G4String &filename, const G4String &treename);
    ~PhaseSpaceGeneratorAction() override;

    void GeneratePrimaries(G4Event*) override; // method from the base class

private:
    void InitialiseReaderThisThread();
    void SetNtupleColumns();
    void SetDefaultPrimaryParticle();

    G4String filename, treename;
    G4bool initialised = false;
    G4int threadID = 0;
    G4int nThreads = 1;

    G4double x, y, z, u, v, w, kE, weight;
    G4int pdg;
};

#endif
