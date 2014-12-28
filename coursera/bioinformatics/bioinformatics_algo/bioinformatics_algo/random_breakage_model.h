#ifndef RANDOM_BREAKAGE_MODEL
#define RANDOM_BREAKAGE_MODEL

#include <vector>
#include <string>

namespace bio {

void reversal(std::vector<int>& permutation, int start, int end);
std::string permutationString(const std::vector<int>& permutation);
void readPermutationString(const std::string str, std::vector<int>& permutation);
int getNumBreakpoints(const std::vector<int>& permutation);

namespace week6 {

    void greedySorting(const std::vector<int>& permutation, std::vector<std::vector<int>>& seq_permutations);

} /* week6 */
    
} /* boi */

#endif /* RANDOM_BREAKAGE_MODEL */