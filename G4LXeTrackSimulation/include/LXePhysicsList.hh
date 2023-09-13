#include "G4VModularPhysicsList.hh"

class LXePhysicsList : public G4VModularPhysicsList
{
    public:
        LXePhysicsList();
        ~LXePhysicsList();
    protected:
        void ConstructEM();
        void SetCuts();
    private:
        G4double cutValue;
        G4double lowEnergy;
};
