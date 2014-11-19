#ifndef GIBBS_SAMPLER
#define GIBBS_SAMPLER

#include <vector>

namespace bio
{

class GibbsSampler
{
private:
    std::vector<double> probabilities;

public:
    GibbsSampler(const std::vector<double>& init_probabilities);
    int sample();
};

} /* bio */

#endif /* GIBBS_SAMPLER */