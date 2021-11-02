#include "G4ParticleTypes.hh"
#include "G4SystemOfUnits.hh"
#include "G4ProductionCutsTable.hh"

#include "G4VUserPhysicsList.hh"
#include "G4ProcessManager.hh"
#include "G4PhysicsListHelper.hh"

#include "G4ComptonScattering.hh"
#include "G4GammaConversion.hh"
#include "G4PhotoElectricEffect.hh"

#include "G4eMultipleScattering.hh"
#include "G4eIonisation.hh"
#include "G4eBremsstrahlung.hh"

#include "LXePhysicsList.hh"

LXePhysicsList::LXePhysicsList() : G4VUserPhysicsList() 
{
    cutValue = 1.0 * nm;
    lowEnergy = 10.0 * eV;
}

LXePhysicsList::~LXePhysicsList() {}

void LXePhysicsList::ConstructParticle()
{
    G4Gamma::GammaDefinition();
    G4Electron::ElectronDefinition();
    G4GenericIon::GenericIonDefinition();
}

void LXePhysicsList::ConstructProcess()
{
    AddTransportation();
    ConstructEM();
    //AddStepMax();
}

void LXePhysicsList::SetCuts()
{
    G4int temp = GetVerboseLevel();
    // SetCutsWithDefault();
    SetVerboseLevel(temp);

    SetCutValue(cutValue, "gamma");
    SetCutValue(cutValue, "e-");
    SetCutValue(cutValue, "GenericIon");

    G4ProductionCutsTable::GetProductionCutsTable()->SetEnergyRange(lowEnergy, 1.0*GeV);
}

#include "G4VUserPhysicsList.hh"
#include "G4PhysicsListHelper.hh"

#include "G4LivermoreComptonModel.hh"
#include "G4LivermorePhotoElectricModel.hh"
#include "G4LivermoreRayleighModel.hh"

#include "G4eMultipleScattering.hh"
#include "G4LivermoreIonisationModel.hh"
#include "G4LivermoreBremsstrahlungModel.hh"

#include "G4hMultipleScattering.hh"
#include "G4ionIonisation.hh"

void LXePhysicsList::ConstructEM()
{
    G4PhysicsListHelper* ph = G4PhysicsListHelper::GetPhysicsListHelper();
    auto theParticleIterator = GetParticleIterator();

    theParticleIterator->reset();
    while( (*theParticleIterator)() )
    {
        G4ParticleDefinition* particle = theParticleIterator->value();
        G4String particleName = particle->GetParticleName();
    
        if (particleName == "gamma") {         
            ph->RegisterProcess(new G4PhotoElectricEffect, particle);
            ph->RegisterProcess(new G4ComptonScattering,   particle);
//             ph->RegisterProcess(new G4RayleighScattering,  particle);
        } else if (particleName == "e-") {
            ph->RegisterProcess(new G4eMultipleScattering, particle);
            ph->RegisterProcess(new G4eIonisation,         particle);
            ph->RegisterProcess(new G4eBremsstrahlung,     particle);
        } else if( particleName == "GenericIon" ) {
            ph->RegisterProcess(new G4hMultipleScattering, particle);
            ph->RegisterProcess(new G4ionIonisation,       particle);     
        }
    }
}

