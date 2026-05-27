#include "PhantomSD.hh"
#include "Parameters.hh"

// #include "G4HCofThisEvent.hh"
#include "G4AnalysisManager.hh"
#include "G4SystemOfUnits.hh"
#include "G4VSolid.hh"
#include "G4RunManager.hh"
// #include "G4HCofThisEvent.hh"
// #include "G4SDManager.hh"

PhantomSD::PhantomSD(const G4String &name) : G4VSensitiveDetector(name) {}

PhantomSD::~PhantomSD() {}

G4bool PhantomSD::ProcessHits(G4Step *step, G4TouchableHistory *){
    // For phase space: record particles entering the volume
    G4StepPoint *preStepPoint = step->GetPreStepPoint();
    if (preStepPoint->GetStepStatus() != fGeomBoundary)
        return false;

    const G4VTouchable *touchable = preStepPoint->GetTouchable();
    G4int planeIndex = touchable->GetCopyNumber(0);
    // if (planeIndex < 0 || planeIndex >= N_PLANES){
    //     G4cerr << "PhantomSD: copy number out of range: " << planeIndex << G4endl;
    //     return false;
    // }
    G4int ntupleID = planeIndex + PHANTOM_NTUPLE_OFFSET; // ntupleID starts from the offset, for instance, 0 is for the scoring plane before the phantom

    G4Track *track = step->GetTrack();
    G4int pdg = track->GetDynamicParticle()->GetPDGcode();
    G4ThreeVector position = preStepPoint->GetPosition();
    G4ThreeVector momentumDirection = preStepPoint->GetMomentumDirection();
    G4double kineticEnergy = preStepPoint->GetKineticEnergy();
    G4double weight = track->GetWeight();
    // G4int eventID = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    // G4int parentID = track->GetParentID();
    // G4int trackID = track->GetTrackID();
    // G4int isNew = -1; //(trackID == 1) ? 1 : 0; // should be checked later for the correctness

    // G4cout<< "position"<< position<<", direction"<< momentumDirection<<", kE("<< kineticEnergy<< ")" <<G4endl;
    // track->SetTrackStatus(fStopAndKill); // kill the particle after recording its phase space data

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    analysisManager->FillNtupleDColumn(ntupleID, 0, position.x() / mm);
    analysisManager->FillNtupleDColumn(ntupleID, 1, position.y() / mm);
    analysisManager->FillNtupleDColumn(ntupleID, 2, position.z() / mm);
    analysisManager->FillNtupleDColumn(ntupleID, 3, momentumDirection.x());
    analysisManager->FillNtupleDColumn(ntupleID, 4, momentumDirection.y());
    analysisManager->FillNtupleDColumn(ntupleID, 5, momentumDirection.z());
    analysisManager->FillNtupleDColumn(ntupleID, 6, kineticEnergy / MeV);
    analysisManager->FillNtupleDColumn(ntupleID, 7, weight);
    analysisManager->FillNtupleIColumn(ntupleID, 8, pdg);
    // analysisManager->FillNtupleIColumn(ntupleID, 9, isNew);
    // analysisManager->FillNtupleIColumn(ntupleID, 10, eventID);
    // analysisManager->FillNtupleIColumn(ntupleID, 11, parentID);
    // analysisManager->FillNtupleIColumn(ntupleID, 12, trackID);
    analysisManager->AddNtupleRow(ntupleID);

    return true;
}