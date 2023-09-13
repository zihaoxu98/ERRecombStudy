#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "LXeSteppingAction.hh"

LXeSteppingAction::LXeSteppingAction() {}

LXeSteppingAction::~LXeSteppingAction() {}

void LXeSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector pos = preStepPoint->GetPosition();
    G4double Ekin = preStepPoint->GetKineticEnergy();
    G4double Edel = aStep->GetDeltaEnergy();
    
    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
    if(aStep->GetTrack()->GetParentID() == 0)
    {
        G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
        analysisManager->FillNtupleIColumn(0, evt);
        analysisManager->FillNtupleDColumn(1, Edel / eV);
        analysisManager->FillNtupleDColumn(2, pos[0] / um);
        analysisManager->FillNtupleDColumn(3, pos[1] / um);
        analysisManager->FillNtupleDColumn(4, pos[2] / um);
        analysisManager->AddNtupleRow();
    }
}
