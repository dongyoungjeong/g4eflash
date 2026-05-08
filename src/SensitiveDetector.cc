#include "SensitiveDetector.hh"

// #include "G4HCofThisEvent.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSolid.hh"
#include "G4RunManager.hh"
// #include "G4HCofThisEvent.hh"
// #include "G4SDManager.hh"


SensitiveDetector::SensitiveDetector(const G4String &name) : G4VSensitiveDetector(name) {}


SensitiveDetector::~SensitiveDetector(){}


G4bool SensitiveDetector::ProcessHits(G4Step *step, G4TouchableHistory *){
    // For phase space: record particles entering the volume
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    if (preStepPoint->GetStepStatus() != fGeomBoundary)
        return false;
    // --- Only record forward-going particles (travelling toward -z) ---
    if (preStepPoint->GetMomentumDirection().z() >= 0.)
        return false;

    G4Track *track = step->GetTrack();
    G4int pdg = track->GetDynamicParticle()->GetPDGcode();
    
    G4ThreeVector position = preStepPoint->GetPosition();
    G4ThreeVector momentumDirection = preStepPoint->GetMomentumDirection();
    // G4ThreeVector momentum = preStepPoint->GetMomentum();
    G4double kineticEnergy = preStepPoint->GetKineticEnergy();

    G4double weight = track->GetWeight();
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int parentID = track->GetParentID();
    G4int trackID = track->GetTrackID();
    G4int isNew = -1; //(trackID == 1) ? 1 : 0; // should be checked later for the correctness

    // G4cout<< "position"<< position<<", direction"<< momentumDirection<<", kE("<< kineticEnergy<< ")" <<G4endl;
    // track->SetTrackStatus(fStopAndKill); // kill the particle after recording its phase space data

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, 0, position.x() / mm);
    analysisManager->FillNtupleDColumn(0, 1, position.y() / mm);
    analysisManager->FillNtupleDColumn(0, 2, position.z() / mm);
    analysisManager->FillNtupleDColumn(0, 3, momentumDirection.x());
    analysisManager->FillNtupleDColumn(0, 4, momentumDirection.y());
    analysisManager->FillNtupleDColumn(0, 5, momentumDirection.z());
    analysisManager->FillNtupleDColumn(0, 6, kineticEnergy / MeV);
    analysisManager->FillNtupleDColumn(0, 7, weight);
    analysisManager->FillNtupleIColumn(0, 8, pdg);
    analysisManager->FillNtupleIColumn(0, 9, isNew);
    analysisManager->FillNtupleIColumn(0, 10, eventID);
    analysisManager->FillNtupleIColumn(0, 11, parentID);
    analysisManager->FillNtupleIColumn(0, 12, trackID);
    analysisManager->AddNtupleRow(0);

    return true;
}