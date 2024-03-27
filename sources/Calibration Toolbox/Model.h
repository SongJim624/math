//#include "Function.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <array>
#include <list>
#include <utility>
#include <map>
#include "Indicators.h"
#include "Optimization/Optimizor.h"

class Model : public Function
{
private:
    std::string parameter, output;

    std::ifstream content;
    std::ofstream buffer;

    size_t location, end;

    std::vector<long> lines;
    std::array<std::map<long, std::pair<float, float>>, 3> observations;
    std::array<std::list<float>, 3> simulations;
    std::pair<float, float> yield;

    int TS2E;

private:
/*
* the functions Change and Write are responsible for
* writing the changed parameters into the crop paramter file
*/
    template<typename T>
    void Change(const size_t& line, const T& value);
    void Write();

/*
* the function Read is to abstract the simulation results
*/
    void Read(std::array<std::list<float>, 3>& simulations, float& yield);

/*
* function to abstract the observations
*/
    void Observation(const std::string& obserations);

public:
    Model(const std::string& parameters, const std::string& templates, const std::string& outputs,
        const std::string& obeservations, const std::vector<long>& lines);
    ~Model();

    virtual void function(float* decisions, const long& decision_size, float* objectives, const long& objective_size);
};