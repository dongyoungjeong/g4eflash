#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"
#include "Parameters.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"


RunAction::RunAction() : G4UserRunAction() {}


RunAction::~RunAction() {}


void RunAction::BeginOfRunAction(const G4Run* run) {
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetDefaultFileType("root");
    analysisManager->SetNtupleMerging(true); // required for multithreading

    G4String fileName = "run" + std::to_string(run->GetRunID());
    analysisManager->OpenFile(fileName);

    CreateHeadNtuple();
    CreatePhantomNtuples();
}


void RunAction::CreateHeadNtuple(){ // ntuple 0 — head scoring plane (filled by SensitiveDetector)
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    G4int ntupleID = analysisManager->CreateNtuple("phsp_head", "phase space at head");
    if (ntupleID != 0) // id must be 0; assert just in case
        G4cerr << "RunAction: unexpected ntuple ID for head plane: " << ntupleID << G4endl;
    AddPhaseSpaceColumns(analysisManager, ntupleID);
}


void RunAction::CreatePhantomNtuples(){ // ntuples 1..N_PLANES — phantom planes
    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    for (G4int i=0; i<N_PLANES; ++i){
        std::ostringstream oss;
        oss << "phsp_plane_" << std::setw(3) << std::setfill('0') << i;
        G4int ntupleID = analysisManager->CreateNtuple(oss.str(), "phase space at phantom plane " + std::to_string(i));
        G4int expectedID = i + PHANTOM_NTUPLE_OFFSET; // ntupleID starts from the offset, for instance, 0 is for the scoring plane before the phantom
        if(ntupleID != expectedID)
            G4cerr << "RunAction: ntuple ID mismatch for plane " << i << " — expected " << expectedID << ", got " << ntupleID << G4endl;
        AddPhaseSpaceColumns(analysisManager, ntupleID);
    }
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
    G4int nofEvents = run->GetNumberOfEvent();
    if(nofEvents == 0)
        return;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}