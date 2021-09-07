#include "LXeSensitiveDetector.hh"
#include "g4root.hh"

LXeSensitiveDetector::LXeSensitiveDetector(G4String name) : G4VSensitiveDetector(name)
{}

LXeSensitiveDetector::~LXeSensitiveDetector()
{}

G4bool LXeSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist)
{
//     G4Track *track = aStep->GetTrack();
    
//     G4StepPoint *preStepPoint = aStep->GetPreStepPoint();
//     G4StepPoint *postStepPoint = aStep->GetPostStepPoint();

//     G4ThreeVector pos = preStepPoint->GetPosition();
//     G4double Ekin = preStepPoint->GetKineticEnergy();
//     G4double Edel = aStep->GetDeltaEnergy();
    
//     G4int evt = G4RunManager::GetRunManager()->GetCurrentEvent()->GetEventID();
    
//     G4cout << "ID: "             << evt
//            << "\tposition[um]: " << pos/um 
//            << "\tEdel[keV]: "    << Edel/keV
//            << G4endl;
    
//     return true;
}
