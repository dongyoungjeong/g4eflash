#include "DetectorConstruction.hh"
#include "ActionInitialisation.hh"
#include "Parameters.hh"

#include "QGSP_BIC_HP.hh"
#include "G4RunManagerFactory.hh"
#include "G4SteppingVerbose.hh"
#include "G4UIExecutive.hh"
#include "G4UImanager.hh"
#include "G4VisExecutive.hh"
#include "G4ScoringManager.hh"

int main(int argc, char** argv){
    G4int nThreads = lround(ceil(G4float(G4Threading::G4GetNumberOfCores())/2.));
    G4Random::setTheSeed(1); // 1 ≤ seed ≤ 900,000,000

    G4RunManager* runManager;
    if(PHSP_SOURCE)
        runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Serial);
    else
        runManager = G4RunManagerFactory::CreateRunManager(G4RunManagerType::Default);
    runManager->SetNumberOfThreads(nThreads);
    runManager->SetUserInitialization(new DetectorConstruction());
    runManager->SetUserInitialization(new QGSP_BIC_HP());
    runManager->SetUserInitialization(new ActionInitialisation());
    runManager->Initialize(); // declared here instead of .mac files
    // runManager->BeamOn(1000000); // normally declared in run.mac

    G4String argv1 = argc>1? argv[1]: "";
    if (argv1.size()>0 && std::all_of(argv1.begin(), argv1.end(), ::isdigit))
        runManager->BeamOn(std::stoi(argv1));
    else{ // use of visExecutive and UIpointer
        auto visExecutive = new G4VisExecutive(argc, argv);
        visExecutive->Initialize();
        // G4ScoringManager::GetScoringManager();
        auto UIpointer = G4UImanager::GetUIpointer();
        if (argc == 1){ // interactive mode
            auto uiExecutive = new G4UIExecutive(argc, argv);
            UIpointer->ApplyCommand("/control/execute vis.mac");
            uiExecutive->SessionStart();
            delete uiExecutive;
        }
        else
            UIpointer->ApplyCommand("/control/execute " + argv1);
        delete visExecutive;
    }
    delete runManager;
    return 0;
}

