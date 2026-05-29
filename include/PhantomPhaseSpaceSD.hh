#ifndef PhantomPhaseSpaceSD_h
#define PhantomSD_h 1

#include "G4VSensitiveDetector.hh"

#include <vector>

class PhantomPhaseSpaceSD : public G4VSensitiveDetector{

public:
    PhantomPhaseSpaceSD(const G4String &name);
    ~PhantomPhaseSpaceSD() override;

    G4bool ProcessHits(G4Step *step, G4TouchableHistory *) override;
    void Initialize(G4HCofThisEvent *) override {}
    void EndOfEvent(G4HCofThisEvent *) override {}

private:
};

#endif
