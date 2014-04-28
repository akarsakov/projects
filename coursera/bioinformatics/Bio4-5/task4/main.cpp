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
	map<string, vector<string>> hash;
	while(!feof(file))
	{
		fscanf(file, "%s", c_str);
		string str(c_str);
		string pre = str.substr(0, str.size() - 1);
		string post = str.substr(1, str.size() - 1);

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
		sort(iter->second.begin(), iter->second.end());
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