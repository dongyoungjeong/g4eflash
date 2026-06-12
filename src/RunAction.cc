#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Parameters.hh"
#include "PhantomSD.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4SDManager.hh"


RunAction::RunAction() : G4UserRunAction() {}


RunAction::~RunAction() {}


void RunAction::BeginOfRunAction(const G4Run* run) {
    if(PHSP_SCORING){
        G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
        analysisManager->SetDefaultFileType("root");
        analysisManager->SetNtupleMerging(true); // required for multithreading

        G4String fileName = "phsp_run" + std::to_string(run->GetRunID());
        analysisManager->OpenFile(fileName);

        CreatePhaseSpaceNtuple();
    }
}


void RunAction::CreatePhaseSpaceNtuple(){ // ntuple 0 — head scoring plane (filled by SensitiveDetector)
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    G4int ntupleID = analysisManager->CreateNtuple(PHSP_TREENAME, "phase space at head");
    if (ntupleID != 0) // id must be 0; assert just in case
        G4cerr << "RunAction: unexpected ntuple ID for head plane: " << ntupleID << G4endl;
    AddPhaseSpaceColumns(analysisManager, ntupleID);
}


void RunAction::AddPhaseSpaceColumns(G4AnalysisManager *analysisManager, G4int ntupleID){
    analysisManager->CreateNtupleDColumn(ntupleID, "x");        // col 0 — position X (mm)
    analysisManager->CreateNtupleDColumn(ntupleID, "y");        // col 1 — position Y (mm)
    analysisManager->CreateNtupleDColumn(ntupleID, "z");        // col 2 — position Z (mm)
    analysisManager->CreateNtupleDColumn(ntupleID, "u");        // col 3 — direction cosine X
    analysisManager->CreateNtupleDColumn(ntupleID, "v");        // col 4 — direction cosine Y
    analysisManager->CreateNtupleDColumn(ntupleID, "w");        // col 5 — direction cosine Z
    analysisManager->CreateNtupleDColumn(ntupleID, "kE");       // col 6 — kinetic energy (MeV)
    analysisManager->CreateNtupleDColumn(ntupleID, "weight");   // col 7 — particle weight
    analysisManager->CreateNtupleIColumn(ntupleID, "pdg");      // col 8 — PDG code
    // analysisManager->CreateNtupleIColumn(ntupleID, "isNew");    // col 9 — new track flag
    // analysisManager->CreateNtupleIColumn(ntupleID, "eventID");  // col 10
    // analysisManager->CreateNtupleIColumn(ntupleID, "parentID"); // col 11
    // analysisManager->CreateNtupleIColumn(ntupleID, "trackID");  // col 12
    analysisManager->FinishNtuple(ntupleID);
}

void RunAction::EndOfRunAction(const G4Run* run) {
    if(run->GetNumberOfEvent() == 0)
        return;

    if(PHSP_SCORING){ // Phase Space files
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
    }
    else if(PHANTOM){// Phantom dose file
        if(G4Threading::IsWorkerThread())
            return;
        G4SDManager *SDMpointer = G4SDManager::GetSDMpointer();
        auto phantomSD = static_cast<PhantomSD*>(SDMpointer->FindSensitiveDetector("PhantomSD"));
        phantomSD->Write3DDose("dose3d.csv");
    }
}