#ifndef SensitiveDetector_h
#define SensitiveDetector_h 1

#include "G4VSensitiveDetector.hh"

#include <vector>

class SensitiveDetector : public G4VSensitiveDetector
{

public:
    SensitiveDetector(const G4String &name);
    ~SensitiveDetector() override;

    G4bool ProcessHits(G4Step *step, G4TouchableHistory *) override;
    void Initialize(G4HCofThisEvent *) override {}
    void EndOfEvent(G4HCofThisEvent *) override {} 

private:
};

#endif
