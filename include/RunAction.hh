#ifndef RunAction_h
#define RunAction_h 1

#include "globals.hh"
#include "G4UserRunAction.hh"
#include "G4AnalysisManager.hh"

class G4Run;

class RunAction : public G4UserRunAction {
public:
    RunAction();
    ~RunAction() override;

    void BeginOfRunAction(const G4Run*) override;
    void EndOfRunAction(const G4Run*) override;

private:
    void CreateHeadNtuple();          // ntuple 0  — head scoring plane
    void CreatePhantomNtuples();      // ntuples 1..N_PLANES — phantom planes
    static void AddPhaseSpaceColumns(G4AnalysisManager *analysisManager, G4int ntupleID);
};
#endif