#include <vector>
#include <mkl.h>

class Broyden
{
private:
    //tolerance, two types
    float relative_, absolute_;
    //std::function<void(float *, float *)> solver_;
    std::function<int(float *)> updater_;

    //the class should keep a reference of the function to be solved
    std::function<void(float *, float *)> function_;

private:
    class Strategy
    {
    private:
        std::map<std::string, std::function<int(float *)>> updaters_;

    public:
        Strategy();
        std::function<int(float *, float *)> search(const char *);
    };

public:
    std::vector<float> Solve(const std::vector<float>& initial);

public:
    Broyden(std::function<void(float *, float *)>&, size_t, size_t, float, float, const char *);
};