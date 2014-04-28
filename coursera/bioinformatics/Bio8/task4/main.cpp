#include <stdio.h>
#include <string>
#include <math.h>
#include <unordered_map>
#include <list>
using namespace std;

string reverse(string str)
{
	string res(str.size(), 'A');
	int size = str.size();
	for (int i=0; i<size; i++)
	{
		switch (str[i])
		{
		case 'A': res[size - i - 1] = 'T'; break;
		case 'T': res[size - i - 1] = 'A'; break;
		case 'C': res[size - i - 1] = 'G'; break;
		case 'G': res[size - i - 1] = 'C'; break;
		}
	}
	return res;
}

int main()
{
	FILE* file = fopen("input.txt", "r");
	FILE* out = fopen("result.txt", "w");
	int k;
	fscanf(file, "%d", &k);
	char* str_c = new char[205000];
	fscanf(file, "%s", str_c);
	string first(str_c);
	fscanf(file, "%s", str_c);
	string second(str_c);
	string first_rev = reverse(first);
	printf("begin!\n");
	unordered_map<string, list<int>> hash;

	for (int i=0; i<first.size() - k + 1; i++)
	{
		string tmp = first.substr(i, k);
		auto f = hash.find(tmp);
		if (f != hash.end())
			f->second.push_back(i);
		else {
			list<int> l;
			l.push_back(i);
			hash[tmp] = l;
		}

	}

	for (int j=0; j<second.size() - k + 1; j++)
	{
		string s = second.substr(j, k);
		string s_rev = reverse(s);
		auto f = hash.find(s);
		if (f != hash.end())
		{
			for (auto it=f->second.begin(); it!=f->second.end(); it++)
			{
				fprintf(out, "(%d, %d)\n", *it, j);
			}
		}
		f = hash.find(s_rev);
		if (f != hash.end())
		{
			for (auto it=f->second.begin(); it!=f->second.end(); it++)
			{
				fprintf(out, "(%d, %d)\n", *it, j);
			}
		}
	}
	delete[] str_c;
	fclose(out);
	fclose(file);
	return 0;
}