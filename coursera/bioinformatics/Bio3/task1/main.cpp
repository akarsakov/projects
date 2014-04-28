#include <stdio.h>
#include <vector>
#include <string>
#include <unordered_set>

void mutateOne(std::string cur, int pos, std::vector<std::string>& res)
{
	std::string temp = cur;
	temp[pos] = 'A';  res.push_back(temp);
	temp[pos] = 'C';  res.push_back(temp);
	temp[pos] = 'T';  res.push_back(temp);
	temp[pos] = 'G';  res.push_back(temp);
}

std::vector<std::string> generateKmers(std::string kMer, int d)
{
	std::vector<std::string> result;
	result.push_back(kMer);
	int start = 0;
	int end = 1;
	for (int m=0; m<d; m++)
	{
		for (int i=start; i<end; i++)
		{
			std::string cur = result[i];
			for (int j=0; j<cur.size(); j++)
				mutateOne(cur, j, result);
		}
		start = end;
		end = result.size();
	}
	std::unordered_set<std::string> unique(result.begin(), result.end());
	result.clear();
	for (auto it=unique.begin(); it!=unique.end(); it++)
		result.push_back(*it);
	return result;
}

bool compare(std::string a, std::string b, int d)
{
	if (a.size() != b.size())
		return false;

	int errors = 0;
	for (int i=0; i<a.size(); i++)
	{
		if (a[i] != b[i])
			errors++;
		if (errors > d)
			return false;
	}
	return true;
}

bool find(std::vector<std::string> DNA, std::string pat, int d)
{
	int k = pat.size();
	for (auto it=DNA.begin(); it!=DNA.end(); it++)
	{
		std::string cur = *it;
		bool found = false;
		for (int i=0; i<cur.size()-k+1; i++)
		{
			std::string current = cur.substr(i, k);
			if (compare(current, pat, d))
			{
				found = true;
				break;
			}
		}
		if (!found)
			return false;
	}
	return true;
}

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
	int k, d;
	fscanf(file, "%d %d", &k, &d);
	std::vector<std::string> DNA;
	char buffer[80];

	while(!feof(file))
	{
		fscanf(file, "%s", buffer);
		std::string temp(buffer);
		DNA.push_back(temp);
	}

	std::string all = "";
	for (auto it=DNA.begin(); it!=DNA.end(); it++)
	{
		all = all + *it;
	}

	std::unordered_set<std::string> result;

	for (int i=0; i<DNA[0].size() - k + 1; i++)
	{
		std::string current = DNA[0].substr(i, k);
		std::vector<std::string> patterns = generateKmers(current, d);
		for (auto it=patterns.begin(); it!=patterns.end(); it++)
		{
			if (find(DNA, *it, d))
			{
				result.insert(*it);
			}
		}
	}

	for (auto it=result.begin(); it!=result.end(); it++)
	{
		printf("%s ", it->c_str());
	}
	return 0;
}