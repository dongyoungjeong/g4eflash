#include "PhaseSpaceGeneratorAction.hh"
#include "Parameters.hh"

#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RootAnalysisReader.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"


PhaseSpaceGeneratorAction::PhaseSpaceGeneratorAction(const G4String &filename, const G4String &treename) {
     SetNtupleColumns(filename, treename);
}


PhaseSpaceGeneratorAction::~PhaseSpaceGeneratorAction() {}


void PhaseSpaceGeneratorAction::SetNtupleColumns(const G4String &filename, const G4String &treename){
    auto rootAnalysisReader = G4RootAnalysisReader::Instance();
    rootAnalysisReader->SetVerboseLevel(1);
    rootAnalysisReader->SetFileName(filename);
    G4int ntuple = rootAnalysisReader->GetNtuple(treename);
    if(ntuple < 0){
        G4ExceptionDescription msg;
        msg << "Failed to open ntuple: " << treename << " in file: " << filename;
        G4Exception("PhaseSpaceGeneratorAction::SetNtupleColumns", "PSGA001", FatalException, msg);
    }
    G4cout << "PhaseSpaceGeneratorAction: opened " << filename << " :: " << treename << "  (ntupleID=" << ntuple << ")" << G4endl;

    rootAnalysisReader->SetNtupleDColumn(ntuple, "x", x);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "y", y);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "z", z);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "u", u);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "v", v);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "w", w);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "kE", kE);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "weight", weight);
    rootAnalysisReader->SetNtupleIColumn(ntuple, "pdg", pdg);
}


void PhaseSpaceGeneratorAction::GeneratePrimaries(G4Event* event) {
    auto rootAnalysisReader = G4RootAnalysisReader::Instance();
    if(!rootAnalysisReader->GetNtupleRow()){
        G4ExceptionDescription msg;
        msg << "Failed to read ntuple row for primary generation";
        G4Exception("PhaseSpaceGeneratorAction::GeneratePrimaries", "PSGA002", FatalException, msg);
    }
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(pdg);
    if(!particleDefinition){
        G4ExceptionDescription msg;
        msg << "Failed to find particle definition for PDG code: " << pdg;
        G4Exception("PhaseSpaceGeneratorAction::GeneratePrimaries", "PSGA003", FatalException, msg);
    }

    auto vertex = new G4PrimaryVertex(G4ThreeVector(x, y, z), 0.);
    auto particle = new G4PrimaryParticle(particleDefinition, u, v, w);
    particle->SetKineticEnergy(kE);
    particle->SetWeight(weight);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);
}
