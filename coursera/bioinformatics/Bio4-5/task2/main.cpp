#include <stdio.h>
#include <vector>
#include <string>
#include <algorithm>

using namespace std;

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
	FILE* out = fopen("result.txt", "w");
	char* c_str = new char[10000];
	int k;
	vector<string> vec;
	while(!feof(file))
	{
		fscanf(file, "%s", c_str);
		vec.push_back(c_str);
	}

	sort(vec.begin(), vec.end());

	for (int i = 0; i<vec.size(); i++)
	{
		string postfix = vec[i].substr(1, vec[i].size() - 1);
		for (int j = 0; j < vec.size(); j++)
		{
			if (i == j)
				continue;
			if (vec[j].substr(0, vec[j].size() - 1) == postfix)
			{
				fprintf(out, "%s -> %s\n", vec[i].c_str(), vec[j].c_str());
			}
		}
	}
	delete[] c_str;
	return 0;
}