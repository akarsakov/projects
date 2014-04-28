#include <stdio.h>
#include <vector>
#include <math.h>
using namespace std;

void kSortingReversal(vector<int>& perm, int start, int end)
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

void printPermutation(const vector<int>& perm, FILE* out)
{
	fprintf(out, "(");
	for (auto it = perm.begin(); it!=perm.end(); it++)
	{
		if (it != perm.begin())
			fprintf(out, " ");
		if (*it > 0)
			fprintf(out, "+%d",*it);
		else
			fprintf(out, "%d",*it);
		
	}
	fprintf(out, ")\n");
}

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	vector<int> permutation;
	while (!feof(file))
	{
		int temp;
		fscanf(file, "%d", &temp);
		permutation.push_back(temp);
	}
	int distance = 0;
	for (int k=1; k<permutation.size()+1; k++)
	{
		if (abs(permutation[k-1]) != k)
		{
			int index = 0;
			for (int i=k-1; i<permutation.size(); i++)
				if (abs(permutation[i]) == k) 
				{
					index = i;
					break;
				}
			kSortingReversal(permutation, k-1, index);
			printPermutation(permutation, out);
			distance++;
		}
		if (permutation[k-1] == -k)
		{
			permutation[k-1] = k;
			printPermutation(permutation, out);
			distance++;
		}
	}

	fclose(out);
	fclose(file);
	return 0;
}