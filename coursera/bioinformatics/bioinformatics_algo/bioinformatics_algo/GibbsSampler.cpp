#include <stdlib.h>
#include <numeric>
#include <time.h>

#include "GibbsSampler.h"

using namespace std;

namespace bio
{

GibbsSampler::GibbsSampler(const vector<double>& init_probabilities)
{
    srand(time(NULL));

    double sum_probs = accumulate(init_probabilities.begin(), init_probabilities.end(), 0.0);
    probabilities = init_probabilities;

    for (size_t i=0; i<probabilities.size(); i++)
    {
        probabilities[i] /= sum_probs;
    }

    for (size_t i=1; i<probabilities.size(); i++)
    {
        probabilities[i] = probabilities[i] + probabilities[i-1];
    }
}

int GibbsSampler::sample()
{
    double rand_point = (double) rand() / RAND_MAX;

    for (size_t i=0; i<probabilities.size(); i++)
    {
        if (rand_point < probabilities[i])
            return (int) i;
    }
    return (int) probabilities.size()-1;
}

};