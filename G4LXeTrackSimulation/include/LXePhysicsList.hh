#include "G4VUserPhysicsList.hh"

class LXePhysicsList : public G4VUserPhysicsList
{
    public:
        LXePhysicsList();
        ~LXePhysicsList();
    protected:
        void ConstructParticle();
        void ConstructProcess();
        void ConstructEM();
        void SetCuts();
    private:
        G4double cutValue;
        G4double lowEnergy;
};





