#include "PrimaryGeneratorAction.hh"

// #include "G4LogicalVolume.hh"
// #include "G4LogicalVolumeStore.hh"
#include "G4GeneralParticleSource.hh"
#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "Randomize.hh"


PrimaryGeneratorAction::PrimaryGeneratorAction() {
    particleSource = new G4GeneralParticleSource();
    G4SingleParticleSource *currentSource = particleSource->GetCurrentSource();
    posDist = currentSource->GetPosDist();
    angDist = currentSource->GetAngDist();
    eneDist = currentSource->GetEneDist();

    SetDefaultPrimaryParticle();
}


PrimaryGeneratorAction::~PrimaryGeneratorAction() {
    delete particleSource;
}


void PrimaryGeneratorAction::SetDefaultPrimaryParticle(){
    G4ParticleTable *particleTable = G4ParticleTable::GetParticleTable();
    G4ParticleDefinition *electron = particleTable->FindParticle("e-");
    particleSource->SetParticleDefinition(electron);

    posDist->SetPosDisType("Beam");
    posDist->SetPosDisShape("Circle");
    posDist->SetCentreCoords(G4ThreeVector(0., 0., m));
    posDist->SetPosRot1(G4ThreeVector(1., 0., 0.)); // default value, but set it anyway
    posDist->SetPosRot2(G4ThreeVector(0., 1., 0.)); // default value, but set it anyway
    // posDist->SetBeamSigmaInX(mm);
    // posDist->SetBeamSigmaInY(mm);
    // posDist->SetRadius(mm);                         // Hard cutoff radius

    angDist->SetAngDistType("beam2d");
    angDist->DefineAngRefAxes("angref1", G4ThreeVector(1., 0., 0.)); // default value, but set it anyway
    angDist->DefineAngRefAxes("angref2", G4ThreeVector(0., 1., 0.)); // default value, but set it anyway
    // angDist->SetBeamSigmaInAngX(deg);                                // set planar divergence
    // angDist->SetBeamSigmaInAngY(deg);                                // set planar divergence

    // eneDist->SetEnergyDisType("Gauss");
    eneDist->SetMonoEnergy(6. * MeV);
    // eneDist->SetBeamSigmaInE(.2 * MeV);
}


void PrimaryGeneratorAction::GeneratePrimaries(G4Event* event) {
    // G4double x = G4RandGauss::shoot(x0, sigmaX);
    // G4double y = G4RandGauss::shoot(y0, sigmaY);
    // G4double z = G4RandGauss::shoot(z0, sigmaZ);
    // posDist->SetCentreCoords(G4ThreeVector(x, y, z));

    particleSource->GeneratePrimaryVertex(event);
}
