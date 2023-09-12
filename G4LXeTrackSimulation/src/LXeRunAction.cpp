#include "G4Run.hh"
#include "G4RunManager.hh"
#include "G4AnalysisManager.hh"

#include "LXeRunAction.hh"
#include "global.hh"

LXeRunAction::LXeRunAction()
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->SetVerboseLevel(1);
    analysisManager->SetFileName(file_name);

    analysisManager->CreateNtuple("LXe", "Particles");
    analysisManager->CreateNtupleIColumn("Event");
    analysisManager->CreateNtupleDColumn("Ed");
    analysisManager->CreateNtupleDColumn("X");
    analysisManager->CreateNtupleDColumn("Y");
    analysisManager->CreateNtupleDColumn("Z");
    analysisManager->FinishNtuple();
}

LXeRunAction::~LXeRunAction()
{
    delete G4AnalysisManager::Instance();
}

void LXeRunAction::BeginOfRunAction(const G4Run* run)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->OpenFile(file_name);
}

void LXeRunAction::EndOfRunAction(const G4Run* run)
{
    G4AnalysisManager* analysisManager = G4AnalysisManager::Instance();
    analysisManager->Write();
    analysisManager->CloseFile();
}
