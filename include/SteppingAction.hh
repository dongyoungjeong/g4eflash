#ifndef SteppingAction_h
#define SteppingAction_h 1

#include "G4UserSteppingAction.hh"


class EventAction;

class SteppingAction : public G4UserSteppingAction {
public:
    SteppingAction(EventAction*);
    ~SteppingAction();

    void UserSteppingAction(const G4Step*) override;

private:
    EventAction* eventAction = nullptr;
};
#endif