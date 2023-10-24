#include "G4SystemOfUnits.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"
#include "G4TrackStatus.hh"

#include "LXeSteppingAction.hh"
#include "global.hh"

LXeSteppingAction::LXeSteppingAction() {total_e = 0;}

LXeSteppingAction::~LXeSteppingAction() {}

void LXeSteppingAction::UserSteppingAction(const G4Step* aStep)
{
    G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
    G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

    G4ThreeVector pos = preStepPoint->GetPosition();
    G4double Ekin = preStepPoint->GetKineticEnergy();
    G4double Edel = aStep->GetDeltaEnergy();

    G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    G4String proc = postStepPoint->GetProcessDefinedStep()->GetProcessName();

    G4AnalysisManager *analysisManager = G4AnalysisManager::Instance();
    total_e += Edel;
    analysisManager->FillNtupleIColumn(0, evt);
    analysisManager->FillNtupleDColumn(1, Edel / keV);
    analysisManager->FillNtupleDColumn(2, pos[0] / um);
    analysisManager->FillNtupleDColumn(3, pos[1] / um);
    analysisManager->FillNtupleDColumn(4, pos[2] / um);
    analysisManager->AddNtupleRow();

    if ((aStep->IsFirstStepInVolume()) && (aStep->GetTrack()->GetParentID() > 0))
    {
        total_e += preStepPoint->GetKineticEnergy();
        analysisManager->FillNtupleIColumn(0, evt);
        analysisManager->FillNtupleDColumn(1, preStepPoint->GetKineticEnergy()/keV);
        analysisManager->FillNtupleDColumn(2, pos[0] / um);
        analysisManager->FillNtupleDColumn(3, pos[1] / um);
        analysisManager->FillNtupleDColumn(4, pos[2] / um);
        analysisManager->AddNtupleRow();
    }
    // G4cout << total_e / keV << G4endl;
}
