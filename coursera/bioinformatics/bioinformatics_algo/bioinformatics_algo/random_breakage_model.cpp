#include <sstream>

#include "random_breakage_model.h"

using namespace std;

namespace bio
{

void reversal(vector<int>& perm, int start, int end)
{
	while (start < end)
	{
		int st = -perm[start];
		int en = -perm[end];
		perm[start] = en;
		perm[end] = st;
		start++;
		end--;
	}
	if (start == end)
		perm[start] = -perm[start];
}

void readPermutationString(string str, vector<int>& permutation) {
    stringstream ss(str);    
    char bracket;
    ss >> bracket;

    int prev = 0;
    while (!ss.eof()) {
        int num;
        ss >> num;
        if (num == prev)
            break;
        permutation.push_back(num);
        prev = num;
    }
}

string permutationString(const vector<int>& perm) {
    stringstream result;
    result << "(";
	for (auto it = perm.begin(); it!=perm.end(); it++)
	{
		if (it != perm.begin())
			result << " ";
		if (*it > 0)
			result << "+" << *it;
		else
            result << *it;
	}
	result << ")";
    return result.str();
}

int getNumBreakpoints(const vector<int>& perm) {
    vector<int> permutation = perm;
    permutation.push_back(permutation.size());
	int breakpoints = 0;
	for (size_t i=0; i<permutation.size()-1; i++)
	{
		if (permutation[i+1] != permutation[i] + 1)
			breakpoints++;
	}
    return breakpoints;
}

namespace week6
{

void greedySorting(const vector<int>& perm, vector<vector<int>>& seq_permutations) {
    vector<int> permutation = perm;
    int distance = 0;

	for (int k=1; k<(int) permutation.size()+1; k++) {
		if (abs(permutation[k-1]) != k)
		{
			int index = 0;
			for (int i=k-1; i<(int) permutation.size(); i++)
				if (abs(permutation[i]) == k) 
				{
					index = i;
					break;
				}
			reversal(permutation, k-1, index);
            seq_permutations.push_back(permutation);
			distance++;
		}
		if (permutation[k-1] == -k)
		{
			permutation[k-1] = k;
			seq_permutations.push_back(permutation);
			distance++;
		}
	}
}

} /* week6 */
} /* bio */