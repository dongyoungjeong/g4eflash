#ifndef EventAction_h
#define EventAction_h 1

#include "globals.hh"
#include "G4UserEventAction.hh"


class RunAction;


class EventAction : public G4UserEventAction {
public:
    EventAction();
    ~EventAction();

    void BeginOfEventAction(const G4Event* event) override;
    void EndOfEventAction(const G4Event* event) override;

private:
    G4int printModulo;
};
#endif