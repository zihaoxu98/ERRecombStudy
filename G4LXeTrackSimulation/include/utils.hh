#include "G4Types.hh"
#include "G4String.hh"
#include <iostream>

G4int StringToInt(char* string)
{
    return G4int(std::stoi(string));
}

G4double StringToDouble(char* string)
{
    return G4double(std::stod(string));
}

G4String StringToString(char* string)
{
    return G4String(string);
}
