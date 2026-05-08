#include "DetectorConstruction.hh"
#include "ActionInitialization.hh"
#include "QGSP_BIC_HP.hh"

#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4ScoringManager.hh"

int main(int argc, char** argv){
    G4int nThreads = 9;
    G4Random::setTheSeed(1); // 1 ≤ seed ≤ 900,000,000

    auto runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    runManager->SetNumberOfThreads(nThreads);
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BIC_HP());
    runManager->SetUserInitialization(new ActionInitialization());
    runManager->Initialize(); // declared here instead of .mac files
    // runManager->BeamOn(1000000); // normally declared in run.mac

    auto visManager = new G4VisExecutive(argc, argv);
    visManager->Initialize();

    // G4ScoringManager::GetScoringManager();

    auto UImanager = G4UImanager::GetUIpointer();
    if (argc == 1){ // interactive mode
        G4UIExecutive *ui = new G4UIExecutive(argc, argv);
        UImanager->ApplyCommand("/control/execute vis.mac");
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
