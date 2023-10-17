#include "G4VUserPrimaryGeneratorAction.hh"

#include "G4ThreeVector.hh"

#include <random>

class G4ParticleGun;
class G4Event;

class LXePrimaryGeneratorAction : public G4VUserPrimaryGeneratorAction
{
    public:
        LXePrimaryGeneratorAction();
        ~LXePrimaryGeneratorAction();

    public:
        void GeneratePrimaries(G4Event* anEvent);

    private:
        G4ParticleGun* particleGun;
        G4ThreeVector RandomDirection();
};
