#include <math.h>
#include <stdio.h>
#include <numeric>
#include <string>
#include <vector>

int str2int(char a)
{
	switch (a)
	{
	case 'A': return 0;
	case 'C': return 1;
	case 'G': return 2;
	case 'T': return 3;
	}
	return 5;
}

int main()
{
	FILE* file = fopen("stepic_dataset.txt", "r");
	int k;
	char* c_str = new char[1000];
	fscanf(file, "%s", c_str);
	fscanf(file, "%d", &k);
	std::string str(c_str);

	float** profile = new float*[k];
	for (int i=0; i<k; i++)
		profile[i] = new float[4];

	for (int i=0; i<k; i++)
	{
		float a1,a2,a3,a4;
		fscanf(file, "%f %f %f %f", &a1, &a2, &a3, &a4);
		profile[i][0] = a1;
		profile[i][1] = a2;
		profile[i][2] = a3;
		profile[i][3] = a4;
	}

	std::string best = "";
	double max = 0.0;
	for (int i=0; i<str.size() - k + 1; i++)
	{
		std::string current = str.substr(i, k);
		double score = 1.0;
		for (int j=0; j<k; j++)
		{
			score *= profile[j][str2int(current[j])];
		}
		if (score > max)
		{
			max = score;
			best = current;
		}
	}
	printf("%s ", best.c_str());
	return 0;
}