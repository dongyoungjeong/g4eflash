#include "ActionInitialisation.hh"
#include "PrimaryGeneratorAction.hh"
#include "PhaseSpaceGeneratorAction.hh"
#include "RunAction.hh"
#include "EventAction.hh"
#include "SteppingAction.hh"
#include "Parameters.hh"


ActionInitialisation::ActionInitialisation() : G4VUserActionInitialization() {}


ActionInitialisation::~ActionInitialisation() {}


void ActionInitialisation::BuildForMaster() const {
    SetUserAction(new RunAction);
}


void ActionInitialisation::Build() const { // This is called once per thread, so we can set up the actions for each thread here
    if(PHSP_SOURCE)
        SetUserAction(new PhaseSpaceGeneratorAction(PHSP_FILENAME, PHSP_TREENAME));
    else
        SetUserAction(new PrimaryGeneratorAction);

    SetUserAction(new RunAction);
    EventAction *eventAction = new EventAction();
    SetUserAction(eventAction);
    SetUserAction(new SteppingAction(eventAction));
}
