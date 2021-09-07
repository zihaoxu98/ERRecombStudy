#include "G4VSensitiveDetector.hh"
#include "G4RunManager.hh"
#include "G4SystemOfUnits.hh"


class LXeSensitiveDetector : public G4VSensitiveDetector
{
    public:
        LXeSensitiveDetector(G4String);
        ~LXeSensitiveDetector();
    
    private:
        virtual G4bool ProcessHits(G4Step *, G4TouchableHistory *);
};
