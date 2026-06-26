#include "PhaseSpaceGeneratorAction.hh"
#include "Parameters.hh"

#include "G4ParticleTable.hh"
#include "G4SystemOfUnits.hh"
#include "G4RootAnalysisReader.hh"
#include "G4PrimaryVertex.hh"
#include "G4PrimaryParticle.hh"
#include "G4ParticleDefinition.hh"
#include "G4Event.hh"
#include "G4RunManager.hh"
#include "G4MTRunManager.hh"


PhaseSpaceGeneratorAction::PhaseSpaceGeneratorAction(const G4String &filename, const G4String &treename) : filename(filename), treename(treename) {}


PhaseSpaceGeneratorAction::~PhaseSpaceGeneratorAction() {}


void PhaseSpaceGeneratorAction::GeneratePrimaries(G4Event* event) {
    if(!initialised)
        InitialiseRootAnalysisReader();

    for(G4int i = 0; i < nThreads; ++i){
        if(!G4RootAnalysisReader::Instance()->GetNtupleRow()){
            G4RunManager::GetRunManager()->AbortRun();
            return;
        }
        if(i == threadID){
            G4ParticleDefinition *particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(pdg);
            auto vertex = new G4PrimaryVertex(G4ThreeVector(x, y, z), 0.);
            auto particle = new G4PrimaryParticle(particleDefinition, u, v, w);
            particle->SetKineticEnergy(kE);
            particle->SetWeight(weight);
            vertex->SetPrimary(particle);
            event->AddPrimaryVertex(vertex);
        }
    }
}


void PhaseSpaceGeneratorAction::InitialiseRootAnalysisReader(){ // per thread
    threadID = std::max(G4Threading::G4GetThreadId(), 0);
    G4MTRunManager *mtRunManager = G4MTRunManager::GetMasterRunManager();
    nThreads = mtRunManager ? std::max(mtRunManager->GetNumberOfThreads(), 1) : 1;

    SetNtupleColumns();

    initialised = true;
}


void PhaseSpaceGeneratorAction::SetNtupleColumns(){
    auto rootAnalysisReader = G4RootAnalysisReader::Instance();
    rootAnalysisReader->SetVerboseLevel(1);
    // rootAnalysisReader->SetFileName(filename); // This automatically adds a suffix to the filename in MT modes
    G4int ntuple = rootAnalysisReader->GetNtuple(treename, filename);
    if(ntuple < 0){
        G4ExceptionDescription msg;
        msg << "Failed to open ntuple: " << treename << " in file: " << filename;
        G4Exception("PhaseSpaceGeneratorAction::SetNtupleColumns", "PSGA001", FatalException, msg);
    }

    rootAnalysisReader->SetNtupleDColumn(ntuple, "x", x);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "y", y);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "z", z);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "u", u);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "v", v);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "w", w);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "kE", kE);
    rootAnalysisReader->SetNtupleDColumn(ntuple, "weight", weight);
    rootAnalysisReader->SetNtupleIColumn(ntuple, "pdg", pdg);
    rootAnalysisReader->SetNtupleIColumn(ntuple, "isNew", isNew);
    // rootAnalysisReader->SetNtupleIColumn(ntuple, "eventID", eventID);
}