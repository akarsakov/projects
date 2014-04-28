#include <stdio.h>
#include <vector>
#include <math.h>
using namespace std;

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	vector<int> permutation;
	permutation.push_back(0);
	while (!feof(file))
	{
		int temp;
		fscanf(file, "%d", &temp);
		permutation.push_back(temp);
	}
	permutation.push_back(permutation.size());
	int breakpoints = 0;
	for (int i=0; i<permutation.size()-1; i++)
	{
		if (permutation[i+1] != permutation[i] + 1)
			breakpoints++;
	}
	fprintf(out, "%d", breakpoints);
	fclose(out);
	fclose(file);
	return 0;
}