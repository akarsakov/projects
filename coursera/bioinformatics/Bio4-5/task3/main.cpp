#include <stdio.h>
#include <vector>
#include <string>
#include <map>
#include <list>
#include <algorithm>

using namespace std;

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
	FILE* out = fopen("result.txt", "w");
	char* c_str = new char[10000];
	int k;
	fscanf(file, "%d", &k);
	fscanf(file, "%s", c_str);
	string str(c_str);
	
	map<string, vector<string>> hash;
	for (int i=0; i < str.size() - k + 1; i++)
	{
		string pre = str.substr(i, k-1);
		string post = str.substr(i+1, k-1);

		auto f = hash.find(pre);
		if (f != hash.end())
		{
			f->second.push_back(post);
		}
		else
		{
			vector<string> l;
			l.push_back(post);
			hash[pre] = l;
		}
	}

	for (auto iter=hash.begin(); iter != hash.end(); iter++)
	{
		fprintf(out, "%s -> ", iter->first.c_str());
		for (int i=0; i<iter->second.size(); i++)
		{
			if (i != 0)
				fprintf(out, ",%s", iter->second[i].c_str());
			else
				fprintf(out, "%s", iter->second[i].c_str());
		}
		fprintf(out, "\n");
	}

	delete[] c_str;
	return 0;
}