#include "EventAction.hh"

#include "G4Event.hh"


EventAction::EventAction() : G4UserEventAction(), printModulo(100000) {}


EventAction::~EventAction() {}


void EventAction::BeginOfEventAction(const G4Event* event) {
    G4int eventID = event->GetEventID();
    if(eventID % printModulo == 0) // this prints every 100000 events
        G4cout << "#### Event " << eventID << " starts." << G4endl;
}


void EventAction::EndOfEventAction(const G4Event* event) {
}