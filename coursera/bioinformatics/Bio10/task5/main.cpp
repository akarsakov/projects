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
	char* c_str = new char[11000];
	char* c_str2 = new char[11000];
	fscanf(file, "%s", c_str);

	vector<string> patterns;
	while (!feof(file))
	{
		fscanf(file, "%s", c_str2);
		string temp(c_str2);
		patterns.push_back(temp);
	}

	vector<string> BWT;
	strcat(c_str, "$");
	int size = strlen(c_str);
	for (int i=0; i<size; i++)
	{
		string begin(c_str + size - i, i);
		string end(c_str, size - i);
		BWT.push_back(begin + end);
	}

	sort(BWT.begin(), BWT.end());

	string last = "";
	string first = "";
	vector<int> suffix_array;
	for (int i=0; i<size; i++)
	{
		int pos = BWT[i].find('$');
		suffix_array.push_back(size - pos - 1);
		last += BWT[i][size-1];
		first += BWT[i][0];
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

	vector<int> result;
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
					//printf("0 ");
					break;
				}

				top = lastToFirst[f];
				bottom = lastToFirst[l];
			}
			else
			{
				//printf("%d ", bottom - top + 1);
				for (int i=top; i<=bottom; i++)
				{
					//printf("%d ", suffix_array[i]);
					result.push_back(suffix_array[i]);
				}
				break;
			}
		}

	}

	sort(result.begin(), result.end());

	for (auto it=result.begin(); it!=result.end(); it++)
	{
		printf("%d ", *it);
	}
	
	delete[] c_str2;
	delete[] c_str;
	fclose(file);
	return 0;
}