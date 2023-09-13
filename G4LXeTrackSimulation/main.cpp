#include "G4RunManager.hh"
#include "G4String.hh"

#include "LXeDetectorConstruction.hh"
#include "LXePhysicsList.hh"
#include "LXePrimaryGeneratorAction.hh"
#include "LXeRunAction.hh"
#include "LXeSteppingAction.hh"
#include "utils.hh"

// DEFINE GLOBAL VARIABLES
#ifndef n_event
G4int n_event = 1;
#endif

#ifndef e_keV
G4double e_keV = 15.0;
#endif

#ifndef file_name
G4String file_name = "LXe.root";
#endif

int main(int argc,char** argv)
{
    for(int i=0; i<argc; i++)
    {
        if      (argv[i] == G4String("--n_event"))   n_event   = StringToInt(argv[++i]);
        else if (argv[i] == G4String("--e_keV"))     e_keV     = StringToDouble(argv[++i]);
        else if (argv[i] == G4String("--file_name")) file_name = StringToString(argv[++i]);
    }
    
    G4cout << "=============================================" << G4endl
           << "|| n_event=" << n_event << G4endl
           << "|| e_keV=" << e_keV << G4endl
           << "|| file_name=" << file_name << G4endl
           << "=============================================" << G4endl;
    
    G4RunManager* runManager = new G4RunManager;

    auto* detector = new LXeDetectorConstruction;
    runManager->SetUserInitialization(detector);

    auto* physics = new LXePhysicsList;	
    runManager->SetUserInitialization(physics);

    auto* gen_action = new LXePrimaryGeneratorAction;
    runManager->SetUserAction(gen_action);

    auto* run_action = new LXeRunAction;
    runManager->SetUserAction(run_action);

    auto* step_action = new LXeSteppingAction;
    runManager->SetUserAction(step_action);

    runManager->Initialize();
    runManager->BeamOn(n_event);

    delete runManager;
    return 0;
}
