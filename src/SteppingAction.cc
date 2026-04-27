#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Electron.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"


SteppingAction::SteppingAction(EventAction *eventAction) : G4UserSteppingAction(), eventAction(eventAction) {}


SteppingAction::~SteppingAction() {}


void SteppingAction::UserSteppingAction(const G4Step *step){
    // G4int eventid = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    // G4StepPoint *postStep = step->GetPostStepPoint();
    // G4StepPoint *preStep = step->GetPreStepPoint();
    // // G4int trackID = step->GetTrack()->GetTrackID();

    // if(postStep->GetStepStatus() == fGeomBoundary){
    //     G4String volumeName = postStep->GetPhysicalVolume()->GetLogicalVolume()->GetName();
    //     G4String prevolumeName = preStep->GetPhysicalVolume()->GetLogicalVolume()->GetName();

    //     G4int parentID = step->GetTrack()->GetParentID();
    //     // G4Track* theTrack = step->GetTrack();
    //     G4double kineticEnergy = step->GetTrack()->GetKineticEnergy();
    //     G4double pos_x = step->GetTrack()->GetPosition().x();
    //     G4double pos_y = step->GetTrack()->GetPosition().y();
    //     G4double pos_z = step->GetTrack()->GetPosition().z();
    //     G4double cos_x = step->GetTrack()->GetMomentum().x();
    //     G4double cos_y = step->GetTrack()->GetMomentum().y();
    //     G4double cos_z = step->GetTrack()->GetMomentum().z();
    //     G4double momentum = std::sqrt(cos_x * cos_x + cos_y * cos_y + cos_z * cos_z);

    //     if(step->GetTrack()->GetDefinition() == G4Electron::ElectronDefinition() && prevolumeName == "logicTreatmentRoom" && volumeName == "phantomLog"){
    //         std::ofstream WriteDataIn("PhantomEntrance.txt", std::ios::app);
    //         WriteDataIn << parentID << "\t" << eventid << "\t" << kineticEnergy << "\t" <<
    //         pos_x << "\t" << pos_y << "\t" << pos_z << "\t" <<
    //         momentum << "\t" << cos_x << "\t" << cos_y << "\t" << cos_z << "\t" << G4endl;
    //     }
    // }
}