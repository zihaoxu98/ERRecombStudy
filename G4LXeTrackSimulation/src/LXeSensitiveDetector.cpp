#include "LXeSensitiveDetector.hh"
#include "G4AnalysisManager.hh"

LXeSensitiveDetector::LXeSensitiveDetector(G4String name) : G4VSensitiveDetector(name) {}

LXeSensitiveDetector::~LXeSensitiveDetector() {}

G4bool LXeSensitiveDetector::ProcessHits(G4Step *aStep, G4TouchableHistory *ROhist) {}
