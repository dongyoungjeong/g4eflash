#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "QBBC.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4ScoringManager.hh"

int main(int argc, char** argv){
    // G4int precision = 4;
    // G4int nThreads = 1;

    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    // G4SteppingVerbose::UseBestUnit(precision);
    // runManager->SetNumberOfThreads(nThreads);

    auto physicsList = new QBBC;
    physicsList->SetVerboseLevel(1);

    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(physicsList);
    runManager->SetUserInitialization(new ActionInitialization());
    
    auto visManager = new G4VisExecutive(argc, argv);
    visManager->Initialize();

    G4UIExecutive *ui = nullptr;
    auto UImanager = G4UImanager::GetUIpointer();
    G4ScoringManager::GetScoringManager();
    if (argc == 1){ // interactive mode
        ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute init_vis.mac");
        ui->SessionStart();
        delete ui;
    }
    else{ // batch mode
        G4String command = "/control/execute ";
        G4String fileName = argv[1];
        UImanager->ApplyCommand(command + fileName);
    }
    
    delete visManager;
    delete runManager;
    return 0;
}
