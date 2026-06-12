#include "EventAction.hh"
#include "Parameters.hh"

#include "G4Event.hh"


EventAction::EventAction() : G4UserEventAction() {}


EventAction::~EventAction() {}


void EventAction::BeginOfEventAction(const G4Event* event) {
    G4int eventID = event->GetEventID();
    if (eventID % PRINT_EVENT_MODULUS == 0)
        G4cout << "#### Event " << eventID << " starts." << G4endl;
}


void EventAction::EndOfEventAction(const G4Event* event) {
}