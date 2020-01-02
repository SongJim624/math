#include "Distribution.h"

class KDE : public Distribution
{
private:    
    std::vector<float> h, xData;
    float(* kernel)(const float&);

private:
    std::vector<float> pdf(const std::vector<float>& X);
    std::vector<float> cdf(const std::vector<float>& X);
private:
    KDE (const std::vector<float>& h, const std::vector<float>& xData, const std::string& kernel)
    : h(h), xData(xData)
    {}

public:
    static Distribution* KDE_Construct(const std::vector<float>& h, const std::vector<float>& xData, const std::string& kernel)
    {
        return new KDE(h, xData, kernel);
    }
};

_declspec (dllexport) Distribution* _stdcall KDE(const std::vector<float>& h, const std::vector<float>& xData, const std::string& kernel)
{
    return KDE::KDE_Construct(h, xData, kernel);
}