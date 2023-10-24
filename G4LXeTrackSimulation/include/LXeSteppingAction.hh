#include "G4UserSteppingAction.hh"
#include "G4Step.hh"

class G4Step;

class LXeSteppingAction : public G4UserSteppingAction
{
    public:
        LXeSteppingAction();
        ~LXeSteppingAction();
        virtual void UserSteppingAction(const G4Step* step);
    private:
        double total_e;
};