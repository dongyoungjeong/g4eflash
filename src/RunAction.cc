#include "RunAction.hh"
#include "DetectorConstruction.hh"
#include "PrimaryGeneratorAction.hh"

#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"


RunAction::RunAction() : G4UserRunAction(){
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
  
  // Merging ntuples from multiple root files due to multithreading 
//   analysisManager->SetNtupleMerging(true);

  // Creation of ntuple
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


RunAction::~RunAction() {}


void RunAction::BeginOfRunAction(const G4Run* run) {
    G4cout << "### Run " << run->GetRunID() << " start." << G4endl;

    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();

    // Merging ntuples from multiple root files due to multithreading
    // analysisManager->SetNtupleMerging(true); // uncomment this only for root files

    // Open an output file
    G4int runID = run->GetRunID();
    std::stringstream ssRunID;
    ssRunID << runID;
    analysisManager->OpenFile("output_" + ssRunID.str() + ".root");

    G4RunManager::GetRunManager()->SetRandomNumberStore(false);
}


void RunAction::EndOfRunAction(const G4Run* run) {
    G4int nofEvents = run->GetNumberOfEvent();
    if(nofEvents == 0)
        return;

    // close the output file
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();

    // if (IsMaster()) {
    //     G4int runID = run->GetRunID();
    //     std::stringstream ss;
    //     ss << runID;
    //     G4String baseName = "output_" + ss.str();

    //     // Output merged file
    //     std::ofstream mergedFile(baseName + "_nt_phasespace_merged.csv");

    //     G4bool headerWritten = false;
    //     G4int nThreads = G4Threading::GetNumberOfRunningWorkerThreads();

    //     G4cout << "\n[RunAction] Merging " << nThreads << " thread CSV files..." << G4endl;

    //     for (G4int t = 0; t < nThreads; ++t) {
    //         std::stringstream ts;
    //         ts << t;
    //         // Geant4 CSV naming convention: output_0_nt_phasespace_t0.csv
    //         G4String threadFile = baseName + "_nt_phasespace_t" + ts.str() + ".csv";

    //         std::ifstream inFile(threadFile);
    //         if (!inFile.is_open()) {
    //             G4cerr << "[RunAction] WARNING: Cannot open "
    //                    << threadFile << G4endl;
    //             continue;
    //         }

    //         G4String line;
    //         G4bool firstLine = true;

    //         while (std::getline(inFile, line)) {
    //             if (firstLine) {
    //                 // Write header only once (from thread 0)
    //                 if (!headerWritten) {
    //                     mergedFile << line << "\n";
    //                     headerWritten = true;
    //                 }
    //                 firstLine = false;
    //                 continue; // skip header from other threads
    //             }
    //             mergedFile << line << "\n";
    //         }

    //         inFile.close();
    //         G4cout << "[RunAction] Merged: " << threadFile << G4endl;
    //     }

    //     mergedFile.close();
    //     G4cout << "[RunAction] Saved: " << baseName + "_nt_phasespace_merged.csv" << G4endl;

    //     G4cout << "\n--------------------End of Global Run-----------------------" << "\n  The run was " << nofEvents << " events." << G4endl;
    // }
    // else
    //     G4cout << "\n--------------------End of Local Run------------------------" << "\n  The run was " << nofEvents << " events." << G4endl;
    }