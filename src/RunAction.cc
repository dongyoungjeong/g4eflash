#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"


RunAction::RunAction() : G4UserRunAction() {}


RunAction::~RunAction() {}


void RunAction::BeginOfRunAction(const G4Run* run) {
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetNtupleMerging(true); // Merging ntuples from multiple root files due to multithreading

    G4String fileName = "run" + std::to_string(run->GetRunID());
    analysisManager->OpenFile(fileName);

    analysisManager->CreateNtuple("phasespace", "phase space data");
    analysisManager->CreateNtupleDColumn("x");
    analysisManager->CreateNtupleDColumn("y");
    analysisManager->CreateNtupleDColumn("z");
    analysisManager->CreateNtupleDColumn("u");
    analysisManager->CreateNtupleDColumn("v");
    analysisManager->CreateNtupleDColumn("w");
    analysisManager->CreateNtupleDColumn("kE");
    analysisManager->CreateNtupleDColumn("weight");
    analysisManager->CreateNtupleIColumn("pdg");
    analysisManager->CreateNtupleIColumn("isNew");
    analysisManager->CreateNtupleIColumn("eventID");
    analysisManager->CreateNtupleIColumn("parentID");
    analysisManager->CreateNtupleIColumn("trackID");
    analysisManager->FinishNtuple(0);
}


void RunAction::EndOfRunAction(const G4Run* run) {
    G4int nofEvents = run->GetNumberOfEvent();
    if(nofEvents == 0)
        return;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    }