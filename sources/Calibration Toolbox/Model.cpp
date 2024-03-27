#include "Model.h"

Model::Model(const std::string& parameters, const std::string& templates, const std::string& output, const std::string& observations, const std::vector<long>& lines) :
    parameter(parameters), output(output), content(templates), buffer(parameters),
    location(0), end(0), lines(lines), length_CC(0), length_BM(0), length_SWC(0), length_sim(0),
    days_CC(nullptr), days_BM(nullptr), days_SWC(nullptr), CC_obs(nullptr), BM_obs(nullptr), SWC_obs(nullptr), CC(nullptr),
    BM(nullptr), SWC(nullptr)
{
    std::string s;
    while(std::getline(content, s))
    {
        end++;
    }

    content.ios_base::clear();
    content.seekg(0, std::ios::beg);

    Observation(observations);
}

Model::~Model()
{
    if(length_sim != 0)
    {
        delete[] CC;
        delete[] BM;
        delete[] SWC;
    }

    if(length_CC != 0)
    {
        delete[] days_CC;
        delete[] CC_obs;
    }

    if (length_BM != 0)
    {
        delete[] days_BM;
        delete[] BM_obs;
    }

    if (length_SWC != 0)
    {
        delete[] days_SWC;
        delete[] SWC_obs;
    }
}

template<typename T>
void Model::Change(const size_t& line, const T& value)
{
    if(!buffer.is_open())
    {
        buffer.open(parameter, std::ios::trunc);
    }
    
    std::string s;
    
    while(true)
    {
        if(location++ == line)
        {
            switch (line)
            {
            case(68): 
            {
                TS2E = value;
                std::getline(content, s);
                buffer << value << std::endl;
                return;
            }
            case(70):
            {
                std::getline(content, s);
                buffer << TS2E + value << std::endl;
                return;
            }
            case(71):
            {
                std::getline(content, s);
                buffer << TS2E + value << std::endl;
                return;
            }
            case(72):
            {
                std::getline(content, s);
                buffer << TS2E + value << std::endl;
                return;
            }
            default:
            {
                std::getline(content, s);
                buffer << value << std::endl;
                return;
            }
            }
        }
        else
        {
            std::getline(content, s);
            buffer << s << std::endl;
        }
    }
}

void Model::Write()
{
    std::string s;

    while(location++ != end)
    {
        std::getline(content, s);
        buffer << s << std::endl;
    }
    
    buffer.close();
    location = 0;
    content.ios_base::clear();
    content.seekg(0, std::ios::beg);
}

void Model::Read(std::array<std::list<float>, 3>& simulations, float& yield)
{
    std::map<long, std::list<float>&> temporary;
    temporary[12] = simulations[0];
    temporary[21] = simulations[1];
    temporary[36] = simulations[2];

    for (auto& iter : temporary)
        iter.second.clear();

    std::ifstream results(output);
    
    std::string s;
    
    std::getline(results, s);
    std::getline(results, s);
    std::getline(results, s);
    std::getline(results, s);
    
    float value = 0;

    while (std::getline(results, s))
    {
        std::stringstream line(s);

        for (long i = 0; i < 37; ++i)
        {
            line >> value;

            if (i == 23) yield = value; continue;

            if (i == 36)
            {
                float r = 0;
                for (long j = 0; j < 10; ++j)
                {
                    line >> value;
                    r += value;
                }
                value = r;
            }

            auto iter = temporary.find(i);
            if (iter != temporary.end()) iter->second.push_back(value);
        }
    }

    results.close();
}

void Model::function(float * decisions, const long& decision_size, float * objectives, const long& objective_size)
{
//Change the paramters   

    for(long i = 0; i <  decision_size; ++i)
    {
        Change(lines[i], decisions[i]);
    }

//Generate the crop parameter file
    Write();

//Execute the AquaCrop model
    system("ACsaV60.exe");

//Abstract the simulation results
    float yield = 0;
    Read(simulations, yield);

    *objectives = 0;
//Compute the log-likehood
    for (long i = 0; i < 3; ++i)
    {
        for (auto iter : observations[i])
        {
            auto it = std::next(simulations[i].begin(), iter.first);
            *objectives += -0.9189 - logf(iter.second.second) - 
                0.5f * powf((*it - iter.second.first) / iter.second.second, 2);
        }
    }

    *objectives += -0.9189 - logf(this->yield.second) -
        0.5f * powf((yield - this->yield.first) / this->yield.second, 2);
}

void Model::Observation(const std::string& observations)
{
    std::ifstream observation(observations);

    std::string s;

    std::getline(observation, s);
    std::getline(observation, s);
    std::getline(observation, s);
    std::getline(observation, s);
    std::getline(observation, s);
    std::getline(observation, s);

    std::getline(observation, s);
    yield = std::stof(s);

    std::getline(observation, s);
    std::getline(observation, s);
    std::getline(observation, s);
    std::getline(observation, s);

    int day = 0;
    float vCC = 0, vBM = 0, vSWC = 0;
    float sCC = 0, sBM = 0, sSWC = 0;

    while (std::getline(observation, s))
    {
        std::stringstream ss(s);
        ss >> day >> vCC >> sCC >> vBM >> sBM >> vSWC >> sSWC;

        if (vCC > 0)
            CC.insert(std::pair<long, std::pair<float, float>>(day, std::pair<float, float>(vCC, sCC)));

        if (vBM > 0)
            BM.insert(std::pair<long, std::pair<float, float>>(day, std::pair<float, float>(vBM, sBM)));

        if (vSWC > 0)
            SWC.insert(std::pair<long, std::pair<float, float>>(day, std::pair<float, float>(vSWC, sSWC)));
    }
}