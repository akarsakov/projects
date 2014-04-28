#include <stdio.h>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

int DNA2num(char DNA)
{
	switch (DNA)
	{
	case 'A': return 0;
	case 'T': return 1;
	case 'C': return 2;
	case 'G': return 3;
	case '$': return 4;
	}
	return 5;
}

int main()
{
	FILE* file = fopen("input.txt", "r");
	freopen ("result.txt","w",stdout);
	char* c_str = new char[10000];
	fscanf(file, "%s", c_str);
	string last(c_str);
	string first = last;
	sort(first.begin(), first.end());
	int size = first.size();
	
	vector<int> first_ind(size, 0);
	vector<int> last_ind(size, 0);

	int counters[5];

	// fill indexes for first column
	for (int i=0; i<5; i++)
		counters[i] = 0;
	for (int i=0; i<size; i++)
	{
		first_ind[i] = counters[DNA2num(first[i])];
		counters[DNA2num(first[i])] += 1;
	}

	// fill indexes for first column
	for (int i=0; i<5; i++)
		counters[i] = 0;
	for (int i=0; i<size; i++)
	{
		last_ind[i] = counters[DNA2num(last[i])];
		counters[DNA2num(last[i])] += 1;
	}

	string result("$");
	char cur_char = '$';
	int cur_ind = 0;

	for (int i=0; i<size-1; i++)
	{
		int j = 0;
		for (j=0; j<size; j++)
		{
			if ((first[j] == cur_char) && (first_ind[j] == cur_ind))
				break;
		}
		result = last[j] + result;
		cur_char = last[j];
		cur_ind = last_ind[j];
	}

	printf("%s", result.c_str());
	

	fclose(file);
	return 0;
}