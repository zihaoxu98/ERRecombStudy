#include "G4VUserDetectorConstruction.hh"
#include "G4SDManager.hh"

#include "LXeSensitiveDetector.hh"

class G4LogicalVolume;
class G4VPhysicalVolume;

class LXeDetectorConstruction : public G4VUserDetectorConstruction
{
    public:
        LXeDetectorConstruction();
        ~LXeDetectorConstruction();
        G4VPhysicalVolume* Construct();

    private:
        G4LogicalVolume* log;
        G4SDManager* DetectorManager;
        virtual void ConstructSDandField();
};


