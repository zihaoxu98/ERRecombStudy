#include "G4Event.hh"
#include "G4ParticleGun.hh"
#include "G4ParticleTable.hh"
#include "G4ParticleDefinition.hh"
#include "G4SystemOfUnits.hh"

#include "LXePrimaryGeneratorAction.hh"
#include "global.hh"

#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include <random>

LXePrimaryGeneratorAction::LXePrimaryGeneratorAction()
{
    particleGun = new G4ParticleGun();
}

LXePrimaryGeneratorAction::~LXePrimaryGeneratorAction()
{
    delete particleGun;
}

G4ThreeVector LXePrimaryGeneratorAction::RandomDirection()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dis(0.0, 1.0);

    double z = 1.0 - dis(gen) * 2.0;
    double theta = dis(gen) * 2.0 * M_PI;
    double x = std::sqrt(1.0 - z * z) * std::cos(theta);
    double y = std::sqrt(1.0 - z * z) * std::sin(theta);

    return G4ThreeVector(x, y, z);
}

void LXePrimaryGeneratorAction::GeneratePrimaries(G4Event* anEvent)
{
    if (sim_flag == 0)
    {
        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        G4String particleName;
        particleGun->SetParticleDefinition(particleTable->FindParticle(particleName="e-"));
        particleGun->SetParticleEnergy(e_keV * keV);
        particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
        G4ThreeVector v(1.0,0.0,0.0);
        particleGun->SetParticleMomentumDirection(v);
        particleGun->GeneratePrimaryVertex(anEvent);
    }
    else if (sim_flag == 1) // neutrino CC
    {
        std::ifstream file("../spectra/neutrino/neutrino_cc_products_" + std::to_string(int(e_keV)) + ".dat");
        if (!file.is_open()) {
            std::cerr << "Error opening file." << std::endl;
            return;
        }

        int totalLines = 0;
        std::string line;
        while (std::getline(file, line)) {
            totalLines++;
        }
        // Generate a random line number
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<> dis(1, totalLines);
        int randomLineNumber = dis(gen);
        // Go back to the beginning of the file
        file.clear();
        file.seekg(0, std::ios::beg);
        // Get the random line
        int currentLine = 0;
        while (std::getline(file, line)) {
            currentLine++;
            if (currentLine == randomLineNumber) {
                file.close();
                break;
            }
        }

        G4ParticleTable* particleTable = G4ParticleTable::GetParticleTable();
        std::istringstream iss(line);
        std::string token;
        while (std::getline(iss, token, ',')) {
            std::istringstream tokenStream(token);
            std::string particle_name;
            double e_eV;
            tokenStream >> particle_name >> e_eV;
            particleGun->SetParticleDefinition(particleTable->FindParticle(particle_name));
            particleGun->SetParticleEnergy(e_eV * eV);
            particleGun->SetParticlePosition(G4ThreeVector(0.0, 0.0, 0.0));
            particleGun->SetParticleMomentumDirection(RandomDirection());
            particleGun->GeneratePrimaryVertex(anEvent);

            G4cout << "Particle: " << particle_name << ", Energy: " << e_eV << " eV" << G4endl;
        }
    }
}
