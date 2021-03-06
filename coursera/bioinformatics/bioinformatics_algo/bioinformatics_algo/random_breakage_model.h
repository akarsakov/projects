#ifndef RANDOM_BREAKAGE_MODEL
#define RANDOM_BREAKAGE_MODEL

#include <vector>
#include <string>

namespace bio {

void reversal(std::vector<int>& permutation, int start, int end);
std::string permutationString(const std::vector<int>& permutation);
void readPermutationString(const std::string str, std::vector<std::vector<int>>& permutation);
int getNumBreakpoints(const std::vector<int>& permutation);

std::vector<int> chromosomeToCycle(const std::vector<int>& chromosome);
std::vector<int> cycleToChromosome(const std::vector<int>& cycle);
std::vector<std::pair<int, int>> getColoredEdges(const std::vector<std::vector<int>>& permutations);
std::vector<std::vector<int>> edgesToGenome(const std::vector<std::pair<int, int>>& edges);
int getNumCycles(const std::vector<std::pair<int, int>>& edges);
void twoBrakeOnGenomeGraph(std::vector<std::pair<int, int>>& edges, int i1, int i2, int j1, int j2);
void twoBrakeOnGenome(std::vector<std::vector<int>>& genome, int i1, int i2, int j1, int j2);



namespace week6 {

    void greedySorting(const std::vector<int>& permutation, std::vector<std::vector<int>>& seq_permutations);
    void twoBreakSorting(const std::vector<int>& P, const std::vector<int>& Q, std::vector<std::vector<int>>& seq_permutations);
} /* week6 */
    
} /* boi */

#endif /* RANDOM_BREAKAGE_MODEL */