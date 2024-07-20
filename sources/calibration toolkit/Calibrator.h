#include <map>
#include <string>
#include "distribution.h"

class Calibrator
{
private:
    std::unordered_map<std::string, Distribution*> parameters;
};