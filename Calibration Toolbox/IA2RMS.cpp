#include "KDE.h"

Proposal::Proposal(float& s, float&p, float& t, long&l)
    :sample(s), proposal(p), target(t), location(l){}

Proposal::Proposal(const Proposal& p)
    :sample(p.sample), proposal(p.proposal),
    target(p.target), location(p.location){}

void KDE::Build(long N)
{
    std::vector<float> X(N + 2);
    std::vector<float> Y(N + 2);

//the random number begin in the second element of the vector
// the first element store the minimum value
 // the last element store the maximum value
    vsRngUniform(0, stream, N, &X[1], min, max);
    X[0] = min; X[N + 1] = max; 
    
    std::sort(X.begin(), X.end());
    density(&X[0], &Y[0], N + 2);

//Since the X has been sorted, 
//the push_back could be used for the slope, interception and area    
    S.insert(std::pair<float, float>(X[0], Y[0]));
    total = 0;
    for(long i = 1; i < N + 2; ++i)
    {
        S.insert(std::pair<float, float>(X[i], Y[i]));
        
        slope.push_back((Y[i] - Y[i - 1]) / (X[i] - X[i - 1]));
        interception.push_back(Y[i] - *slope.rbegin() * X[i]);
        
        float s = 0.5f * (Y[i] + Y[i - 1]) * (X[i] - X[i - 1]);
        area.push_back(s);
        total += s;
    }
}

Proposal KDE::Sample()
{
    float r = (float)rand() / RAND_MAX * total;

    long location = 0;
    
    for (auto a : area)
    {
        r -= a;

        if (r < 0)
            break;
        else
            location++;
    }

//once the r is one
    if (location == area.size())
    {
        r = 0;
        location = area.size();
    }
/*
* Si is for the left side of the interval
* Sj for the right side
*/
    auto Si = std::next(S.begin(), location);
    auto Sj = std::next(Si);

/*
* r is a random number between 0 and 1
* r means the proportion that the area of the sample and the left side of the interval
* of the left side and the right side of the interval
* In the orignal codes from Nikolay, two randon value between 0 and 1 is used.
* The first value is used to settle the interval via the area.
* The second is used to settle the sample via the area.
* I  thus think the there some unnecessary replicants. 
*
* When the proportion of the area is r,
* the proporion of X is \frac{\sqrt{rY{i}^2+(1-r)Y_{i}^2}}{Y_{j} - Y{i}}
*/
    r = 1 + r / *std::next(area.begin(), location);
    float proportion = (sqrtf(r * powf(Sj->second, 2) + (1 - r) * powf(Si->second, 2)) 
        - Si->second) / (Sj->second - Si->second);

    float sample = Si->first+ (Sj->first - Si->first) * proportion;
    float proposal = *std::next(slope.begin(), location) * sample + *std::next(interception.begin(), location);
    float target = 0;
    density(&sample, &target, 1);

    return Proposal(sample, proposal, target, location);
}

void KDE::Update(const Proposal& X, Proposal& Xp)
{
    if(X.location != Xp.location)
    {
        if(X.sample < Xp.sample)
        {
            Xp.location++;
        }

        return;
    }

    auto k = std::next(slope.begin(), X.location);
    auto b = std::next(interception.begin(), X.location);

    if(Xp.sample > X.sample)
    {
        k++; b++; Xp.location++;
    }

    Xp.sample = Xp.sample * *k + *b;
}

void KDE::Insert(const Proposal& X)
{
    //insert a point in the middle of S(location) and S(location + 1)
    auto k = std::next(slope.begin(), X.location);
    auto b = std::next(interception.begin(), X.location);
    auto s = std::next(area.begin(), X.location);

    auto left = std::next(S.begin(), X.location);

    //Update the left side
    *k = (X.target - left->second) / (X.sample - left->first);
    *b = X.target - *k * X.sample;

    total -= *s;
    *s = 0.5f * (left->second + X.target) * (X.sample - left->first);
    total += *s;

    left++;
    //insert the right side
    slope.insert(++k, (X.target - left->second) / (X.sample - left->first));
    interception.insert(++b, X.target - X.sample * *(--k));
    area.insert(++s, 0.5f * (left->first - X.sample) * (left->second + X.target));
    total += *(--s);

//insert the point into the set of the support point set
    S.insert(std::pair<float, float>(X.sample, X.target));
    return;
}

void KDE::sampling(float * X, long N)
{
    Proposal previous = Sample();

    long i = 0;

    while(i < N)
    {
       Proposal proposal = Sample();
    
        if((float) rand() / RAND_MAX > proposal.target / proposal.proposal)
        {
            Insert(proposal);
            Update(proposal, previous);
            continue;
        }      

        //Conduct the MH
        float numerator = proposal.target * fminf(previous.target, previous.proposal);
        float denominator = previous.target * fminf(proposal.target, proposal.proposal);
        
        bool accept = (float) rand() / RAND_MAX < fminf(1, numerator / denominator);
        
        Proposal auxiliary = accept ? previous : proposal;

        if (accept)
        {
            X[i] = proposal.sample;
            previous = proposal;
            ++i;
        }

        if((float)rand() / RAND_MAX > auxiliary.proposal / auxiliary.target)
        {
            Insert(auxiliary);
            Update(auxiliary, previous);
        }
    }
}