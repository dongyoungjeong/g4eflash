#include "SteppingAction.hh"
#include "EventAction.hh"

#include "G4Step.hh"
#include "G4Electron.hh"
#include "G4RunManager.hh"
#include "G4Track.hh"


SteppingAction::SteppingAction(EventAction *eventAction) : G4UserSteppingAction(), eventAction(eventAction) {}


SteppingAction::~SteppingAction() {}


void SteppingAction::UserSteppingAction(const G4Step *step){}