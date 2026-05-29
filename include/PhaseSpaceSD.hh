#ifndef PhaseSpaceSD_h
#define PhaseSpaceSD_h 1

#include "G4VSensitiveDetector.hh"

#include <vector>

class PhaseSpaceSD : public G4VSensitiveDetector
{

public:
    PhaseSpaceSD(const G4String &name);
    ~PhaseSpaceSD() override;

    G4bool ProcessHits(G4Step *step, G4TouchableHistory *) override;
    void Initialize(G4HCofThisEvent *) override {}
    void EndOfEvent(G4HCofThisEvent *) override {} 

private:
};

#endif
