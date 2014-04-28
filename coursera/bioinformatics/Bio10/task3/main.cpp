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
	char* c_str = new char[21000];
	fscanf(file, "%s", c_str);
	string last(c_str);
	string first = last;
	sort(first.begin(), first.end());
	int size = first.size();

	vector<string> patterns;
	while (!feof(file))
	{
		fscanf(file, "%s", c_str);
		string temp(c_str);
		patterns.push_back(temp);
	}
	
	vector<int> first_ind(size, 0);
	vector<int> last_ind(size, 0);
	vector<int> lastToFirst(size, 0);

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

	// fill lastToFirst
	for (int i=0; i<size; i++)
	{
		int j = 0;
		for (j=0; j<size; j++)
		{
			if ((first[j] == last[i]) && (first_ind[j] == last_ind[i]))
				break;
		}
		lastToFirst[i] = j;
	}

	for (auto it=patterns.begin(); it!=patterns.end(); it++)
	{
		string pattern = *it;
		int top = 0;
		int bottom = size - 1;

		while (top <= bottom)
		{
			if (!pattern.empty())
			{
				char sym = pattern[pattern.size() - 1];
				pattern = pattern.substr(0, pattern.size() - 1);

				int f = -1;
				int l = -1;

				for (int i=top; i<=bottom; i++)
				{
					if (last[i] == sym)
					{
						f = i;
						break;
					}
				}

				for (int i=bottom; i>=top; i--)
				{
					if (last[i] == sym)
					{
						l = i;
						break;
					}
				}

				if ((f == -1) || (l == -1))
				{
					printf("0 ");
					break;
				}

				top = lastToFirst[f];
				bottom = lastToFirst[l];
			}
			else
			{
				printf("%d ", bottom - top + 1);
				break;
			}
		}

	}

	

	fclose(file);
	return 0;
}