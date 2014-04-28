#include <math.h>
#include <stdio.h>
#include <numeric>
#include <string>
#include <vector>

char DNA[4] = {'A', 'C', 'G', 'T'};

std::string getNext(std::vector<char>& pep)
{
	std::string result = "";
	for (int i=0; i<pep.size()-1; i++)
	{
		result += DNA[pep[i]];
	}

	int i=0; 
	pep[i] += 1;
	while (pep[i] == 4)
	{
		pep[i] = 0;
		i++;
		pep[i] += 1;
	}
	return result;
}

int score(std::string a, std::string b)
{
	int score = 0;
	for (int i=0; i<a.size(); i++)
	{
		if (a[i] != b[i])
			score++;
	}
	return score;
}

int d(std::string pattern, std::vector<std::string>& DNA)
{
	int* scor = new int[DNA.size()];
	int k = pattern.size();
	for (int i=0; i<DNA.size(); i++)
	{
		std::string current = DNA[i];
		scor[i] = INT_MAX;
		for (int j=0; j<current.size() - k + 1; j++)
		{
			std::string cur = current.substr(j, k);
			int sc = score(pattern, cur);
			if (sc < scor[i])
				scor[i] = sc;
		}
	}
	return std::accumulate(scor, scor+DNA.size(), 0);
}

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
	int k=0;
	fscanf(file, "%d", &k);
	std::vector<std::string> DNA;
	char buffer[80];

	while(!feof(file))
	{
		fscanf(file, "%s", buffer);
		std::string temp(buffer);
		DNA.push_back(temp);
	}

	std::vector<char> pattern;
	pattern.resize(k+1, 0);

	int numberPatterns = pow(4, k);
	int minimum = INT_MAX;

	std::string best = "";
	for (int i=0; i<numberPatterns; i++)
	{
		std::string pat = getNext(pattern);
		if (d(pat, DNA) < minimum)
		{
			best = pat;
			minimum = d(pat, DNA);
		}
	}
	printf("%s ", best.c_str());
	return 0;
}