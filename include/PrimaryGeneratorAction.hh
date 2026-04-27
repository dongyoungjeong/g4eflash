#ifndef PrimaryGeneratorAction_h
#define PrimaryGeneratorAction_h

#include "globals.hh"
#include "G4VUserPrimaryGeneratorAction.hh"


class G4Event;
class G4GeneralParticleSource;
class G4SPSPosDistribution;
class G4SPSAngDistribution;
class G4SPSEneDistribution;

class PrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction {
public:
    PrimaryGeneratorAction();
    ~PrimaryGeneratorAction() override;

    void GeneratePrimaries(G4Event*) override; // method from the base class

private:
    void SetDefaultPrimaryParticle();

    G4GeneralParticleSource* particleSource;
    G4SPSPosDistribution *posDist;
    G4SPSAngDistribution *angDist;
    G4SPSEneDistribution *eneDist;
};

#endif
