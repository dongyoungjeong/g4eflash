#ifndef PhantomSD_h
#define PhantomSD_h 1

#include "G4VSensitiveDetector.hh"

#include <vector>

class PhantomSD : public G4VSensitiveDetector{

public:
    PhantomSD(const G4String &name);
    ~PhantomSD() override;

    G4bool ProcessHits(G4Step *step, G4TouchableHistory *) override;
    void Initialize(G4HCofThisEvent *) override {}
    void EndOfEvent(G4HCofThisEvent *) override {}

private:
};

#endif
