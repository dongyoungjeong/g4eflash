#ifndef PhysicsList_h
#define PhysicsList_h 1

#include "G4VModularPhysicsList.hh"
#include "G4EmConfigurator.hh"

class PhysicsListMessenger;

class PhysicsList : public G4VModularPhysicsList {
public:
    PhysicsList();
    ~PhysicsList() override;

    void AddPhysicsList(const G4String &name);
    void ConstructProcess();
    void ConstructParticle();

private:
    G4String emName;
    G4VPhysicsConstructor *emPhysicsList, *decPhysicsList, *radPhysicsList;
    // PhysicsListMessenger *pMessenger;
};

#endif