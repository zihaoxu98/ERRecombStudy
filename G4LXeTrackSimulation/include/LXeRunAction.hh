#include "G4UserRunAction.hh"

class G4Run;

class LXeRunAction : public G4UserRunAction
{
    public:
        LXeRunAction();
        ~LXeRunAction();
        virtual void BeginOfRunAction(const G4Run* run);
        virtual void EndOfRunAction(const G4Run* run);
};
