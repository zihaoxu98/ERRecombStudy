#include "G4RunManager.hh"
#include "G4Box.hh"
#include "G4LogicalVolume.hh"
#include "G4PVPlacement.hh"
#include "G4SystemOfUnits.hh"

#include "LXeDetectorConstruction.hh"

LXeDetectorConstruction::LXeDetectorConstruction() {}

LXeDetectorConstruction::~LXeDetectorConstruction() {}

G4VPhysicalVolume* LXeDetectorConstruction::Construct()
{
    G4double A = 131.293 * g/mole, Z = 54.0;
    G4double density = 3.1 * g/cm3;
    G4Material* LXe = new G4Material("LiquidXenon", Z, A, density);

    G4double lenx = 5. * cm, leny = 5. * cm, lenz = 5. * cm;
    G4Box* box = new G4Box("LXe_box", lenx, leny, lenz);

    G4LogicalVolume* log = new G4LogicalVolume(box, LXe, "LXe_log", 0, 0, 0);

    G4PVPlacement* world = new G4PVPlacement(0, G4ThreeVector(), log, "LXe_world", 0, false, 0);

    return world;
}

void LXeDetectorConstruction::ConstructSDandField()
{
    LXeSensitiveDetector *sensDet = new LXeSensitiveDetector("LXeSensitiveDetector");
    SetSensitiveDetector("LXe_log", sensDet, true);
}

