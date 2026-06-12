#ifndef ActionInitialisation_h
#define ActionInitialisation_h

#include "G4VUserActionInitialization.hh"

class ActionInitialisation : public G4VUserActionInitialization {
public:
    ActionInitialisation();
    ~ActionInitialisation();

    void BuildForMaster() const override;
    void Build() const override;
};

#endif
