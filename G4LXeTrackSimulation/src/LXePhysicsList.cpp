#include "G4SystemOfUnits.hh"
// #include "G4ParticleTypes.hh"
// #include "G4ProductionCutsTable.hh"

#include "G4EmLivermorePhysics.hh"
#include "G4IonElasticPhysics.hh"
#include "G4IonPhysics.hh"

#include "G4VModularPhysicsList.hh"
#include "LXePhysicsList.hh"

LXePhysicsList::LXePhysicsList() : G4VUserPhysicsList() 
{
    cutValue = 1.0 * nm;
    lowEnergy = 10.0 * eV;
    ConstructEM();
}

LXePhysicsList::~LXePhysicsList() {}

void LXePhysicsList::SetCuts()
{
    G4int verbose = GetVerboseLevel();
    SetVerboseLevel(verbose);

    SetCutValue(cutValue, "gamma");
    SetCutValue(cutValue, "e-");
    SetCutValue(cutValue, "GenericIon");

    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowEnergy, 1.0*GeV);
}

void LXePhysicsList::ConstructEM()
{
    G4int verbose = GetVerboseLevel();
    RegisterPhysics(new G4EmLivermorePhysics(verbose, ""));
    RegisterPhysics(new G4IonElasticPhysics(verbose));
}
