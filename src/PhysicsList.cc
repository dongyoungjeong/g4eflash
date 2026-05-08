#include "PhysicsList.hh"

#include "G4DecayPhysics.hh"
#include "G4EmLivermorePhysics.hh"
#include "G4EmPenelopePhysics.hh"
#include "G4EmStandardPhysics.hh"
#include "G4EmStandardPhysics_option4.hh"
#include "G4EmStandardPhysics_option3.hh"
#include "G4PhysListFactory.hh"
#include "G4ProductionCuts.hh"
#include "G4RadioactiveDecayPhysics.hh"
#include "G4Region.hh"
#include "G4RegionStore.hh"
#include "G4SystemOfUnits.hh"

PhysicsList::PhysicsList() : G4VModularPhysicsList(){
    // pMessenger = new PhysicsListMessenger(this);
    SetVerboseLevel(1);

    // EM physics
    emPhysicsList = new G4EmStandardPhysics_option4(1);
    emName = G4String("emstandard_opt4");
    // Decay physics and all particles
    decPhysicsList = new G4DecayPhysics();
    // Radioactive physics and all particles
    radPhysicsList = new G4RadioactiveDecayPhysics();
}

PhysicsList::~PhysicsList(){
    delete emPhysicsList;
    delete decPhysicsList;
    delete radPhysicsList;
    // delete pMessenger;
}

void PhysicsList::ConstructParticle(){
    decPhysicsList->ConstructParticle();
}

void PhysicsList::ConstructProcess(){
    AddTransportation();

    emPhysicsList->ConstructProcess();
    decPhysicsList->ConstructProcess();
    radPhysicsList->ConstructProcess();
}

void PhysicsList::AddPhysicsList(const G4String &name)
{
    if (verboseLevel > 1)
    {
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">" << G4endl;
    }
    if (name == emName)
        return;

    //   Electromagnetic Models
    if (name == "standard_opt3")
    {
        emName = name;
        delete emPhysicsList;
        emPhysicsList = new G4EmStandardPhysics_option3();
        G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: "
               << "G4EmStandardPhysics_option3" << G4endl;
    }
    else if (name == "standard_opt4")
    {
        emName = name;
        delete emPhysicsList;
        emPhysicsList = new G4EmStandardPhysics_option4();
        G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: "
               << "G4EmStandardPhysics_option4" << G4endl;
    }
    else if (name == "LowE_Livermore")
    {
        emName = name;
        delete emPhysicsList;
        emPhysicsList = new G4EmLivermorePhysics();
        G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: "
               << "G4EmLivermorePhysics" << G4endl;
    }
    else if (name == "LowE_Penelope")
    {
        emName = name;
        delete emPhysicsList;
        emPhysicsList = new G4EmPenelopePhysics();
        G4cout << "THE FOLLOWING ELECTROMAGNETIC PHYSICS LIST HAS BEEN ACTIVATED: "
               << "G4EmLivermorePhysics" << G4endl;

        //   Hadronic Models
    }
    // else if (name == "elastic" && !helIsRegistered)
    // {

    //     hadronPhys.push_back(new G4HadronElasticPhysics());
    //     helIsRegistered = true;
    //     G4cout << "THE FOLLOWING HADRONIC ELASTIC PHYSICS LIST HAS BEEN ACTIVATED: "
    //            << "G4HadronElasticPhysics()" << G4endl;
    // }
    // else if (name == "binary" && !bicIsRegistered)
    // {
    //     hadronPhys.push_back(new G4HadronInelasticQBBC());
    //     bicIsRegistered = true;
    //     G4cout << "THE FOLLOWING HADRONIC INELASTIC PHYSICS LIST HAS BEEN ACTIVATED: "
    //            << "G4HadronInelasticQBBC()" << G4endl;
    // }
    // else if (name == "binary_ion" && !biciIsRegistered)
    // {
    //     hadronPhys.push_back(new G4IonBinaryCascadePhysics());
    //     biciIsRegistered = true;
    // }
    else
    {
        G4cout << "PhysicsList::AddPhysicsList: <" << name << ">"
               << " is not defined"
               << G4endl;
    }
}