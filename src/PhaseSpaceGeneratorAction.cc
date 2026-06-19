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


PhaseSpaceGeneratorAction::PhaseSpaceGeneratorAction(const G4String &filename, const G4String &treename) : filename(filename), treename(treename) {}


PhaseSpaceGeneratorAction::~PhaseSpaceGeneratorAction() {}


void PhaseSpaceGeneratorAction::GeneratePrimaries(G4Event* event) {
    if(!initialised)
        InitialiseReaderThisThread();

    if (!G4RootAnalysisReader::Instance()->GetNtupleRow()){
        G4ExceptionDescription msg;
        msg << "WT" << threadID << " exhausted its slice of the phase-space file. Aborting this thread's run early.";
        G4Exception("PhaseSpaceGeneratorAction::GeneratePrimaries", "PSGA003", JustWarning, msg);
        G4RunManager::GetRunManager()->AbortRun();
        return;
    }
    G4ParticleDefinition* particleDefinition = G4ParticleTable::GetParticleTable()->FindParticle(pdg);
    if(!particleDefinition){
        G4ExceptionDescription msg;
        msg << "Failed to find particle definition for PDG code: " << pdg;
        G4Exception("PhaseSpaceGeneratorAction::GeneratePrimaries", "PSGA004", FatalException, msg);
    }
    auto vertex = new G4PrimaryVertex(G4ThreeVector(x, y, z), 0.);
    auto particle = new G4PrimaryParticle(particleDefinition, u, v, w);
    particle->SetKineticEnergy(kE);
    particle->SetWeight(weight);
    vertex->SetPrimary(particle);
    event->AddPrimaryVertex(vertex);

    for(G4int i = 1; i < nThreads; ++i){
        if(!G4RootAnalysisReader::Instance()->GetNtupleRow()){
            G4RunManager::GetRunManager()->AbortRun();
            return;
        }
    }
}


void PhaseSpaceGeneratorAction::InitialiseReaderThisThread(){
    threadID = std::max(G4Threading::G4GetThreadId(), 0);
    G4RunManager *runManager = G4RunManager::GetRunManager();
    nThreads = runManager ? std::max(runManager->GetNumberOfThreads(), 1) : 1;
    SetNtupleColumns();
    // Skip this thread's offset so each thread starts on a different row:
    // thread 0 starts at row 0, thread 1 at row 1, etc. Discarded rows still populate the bound column variables, but we throw those values away.
    for(G4int i = 0; i < threadID; ++i){
        if(!G4RootAnalysisReader::Instance()->GetNtupleRow()){
            G4ExceptionDescription msg;
            msg << "Phase-space file has fewer rows (" << i << ") than needed to seek thread " << threadID
                << " to its starting offset. Reduce the number of threads or use a larger phase-space file.";
            G4Exception("PhaseSpaceGeneratorAction::InitialiseReaderThisThread", "PSGA001", FatalException, msg);
        }
    }
    initialised = true;
}


void PhaseSpaceGeneratorAction::SetNtupleColumns(){
    auto rootAnalysisReader = G4RootAnalysisReader::Instance();
    rootAnalysisReader->SetVerboseLevel(1);
    rootAnalysisReader->SetFileName(filename);
    G4int ntuple = rootAnalysisReader->GetNtuple(treename);
    if(ntuple < 0){
        G4ExceptionDescription msg;
        msg << "Failed to open ntuple: " << treename << " in file: " << filename;
        G4Exception("PhaseSpaceGeneratorAction::SetNtupleColumns", "PSGA002", FatalException, msg);
    }
    G4cout << "PhaseSpaceGeneratorAction: WT" << threadID << " opened " << filename << " :: " << treename << "  (ntupleID=" << ntuple << ")" << G4endl;

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