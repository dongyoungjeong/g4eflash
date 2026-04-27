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
    G4StepPoint *preStep = step->GetPreStepPoint();
    if (preStep->GetStepStatus() != fGeomBoundary)
        return false;
    // --- Only record forward-going particles (travelling along +Z) ---
    if (preStep->GetMomentumDirection().z() <= 0.)
        return false;

    G4Track *track = step->GetTrack();
    G4int pdg = track->GetDynamicParticle()->GetPDGcode();
    
    G4ThreeVector pos = preStep->GetPosition();
    G4ThreeVector dir = preStep->GetMomentumDirection();
    // G4ThreeVector mom = preStep->GetMomentum();
    G4double kE = preStep->GetKineticEnergy();

    G4double weight = track->GetWeight();
    G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4int parentID = track->GetParentID();
    G4int trackID = track->GetTrackID();
    G4int isNew = (trackID == 1) ? 1 : 0;

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(0, 0, pos.x() / mm);
    analysisManager->FillNtupleDColumn(0, 1, pos.y() / mm);
    analysisManager->FillNtupleDColumn(0, 2, pos.z() / mm);
    analysisManager->FillNtupleDColumn(0, 3, dir.x());
    analysisManager->FillNtupleDColumn(0, 4, dir.y());
    analysisManager->FillNtupleDColumn(0, 5, dir.z());
    analysisManager->FillNtupleDColumn(0, 6, kE / MeV);
    analysisManager->FillNtupleDColumn(0, 7, weight);
    analysisManager->FillNtupleIColumn(0, 8, pdg);
    analysisManager->FillNtupleIColumn(0, 9, isNew);
    analysisManager->FillNtupleIColumn(0, 10, eventID);
    analysisManager->FillNtupleIColumn(0, 11, parentID);
    analysisManager->FillNtupleIColumn(0, 12, trackID);
    analysisManager->AddNtupleRow(0);

    return true;
}