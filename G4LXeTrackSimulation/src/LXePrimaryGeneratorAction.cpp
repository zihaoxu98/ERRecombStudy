#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "LXePrimaryGeneratorAction.hh"
#include "global.hh"

LXePrimaryGeneratorAction::LXePrimaryGeneratorAction()
{
    G4int n_particle = 1;
    particleGun = new G4ParticleGun(n_particle);

    G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
    G4String particleName;
    particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="e-"));
    particleGun->SetParticleEnergy(e_keV*keV);
    particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
}

LXePrimaryGeneratorAction::~LXePrimaryGeneratorAction()
{
    delete particleGun;
}

void LXePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    G4ThreeVector v(1.0,0.0,0.0);
    particleGun->SetParticleMomentumDirection(v);
    particleGun->GeneratePrimaryVertex(anEvent);
}

