#include "Model_TSK0.h"
#include "KohonenNetwork.h"
#include <iostream>
using namespace std;

ModelTSK0::ModelTSK0(Data _data): train_data(_data)
{
	input_dimension = train_data.getNumFeatures();
	KohonenNetwork identification(train_data, 10, 0.1f, 0.1f, 0.00001f, 10);
	identification.trainNetwork();
	c = identification.getCenters();
	num_rules = c.size();
    std::cout << "Num rules: " << num_rules << endl;
	vector<float> gaussWidths = identification.getGaussWidths();
	a.resize(gaussWidths.size());
	for (size_t centerId=0; centerId<gaussWidths.size(); centerId++)
		a[centerId].resize(train_data.getNumFeatures(), gaussWidths[centerId]);
	b0 = identification.getVectorB();
	maxEpoch = 5;
	sigma = 0.01f;
}

void ModelTSK0::modelOptimization()
{
	int epochNumber = 0;
	do
	{
		epochNumber++;
		for (int i=0; i<train_data.getNumExamples(); i++)
		{
			vector<float> x = train_data.getExampleXVector(i);
			float y = train_data.getExampleY(i);
			float answer = getAnswer(x);

			vector<float> level3 = getLevel3(x);
			vector<float> level4 = getLevel4(level3);

			for (int ruleId=0; ruleId<num_rules; ruleId++)
			{
				float deltaB = 2*(answer - y)*level3[ruleId]/level4[1];
				
				float temp = 1.f;
				for (int k=0; k<x.size(); k++)
				{
					temp *= exp(-(x[k] - c[ruleId][k])*(x[k] - c[ruleId][k])/(2*a[ruleId][k]));
				}
			}
		}
	} while (epochNumber < maxEpoch);
}

vector<float> ModelTSK0::getLevel3(vector<float> x)
{
	vector<float> level3(num_rules);
	for (int i=0; i<num_rules; i++)
	{
		float out = 1.f;
		for (size_t j=0; j<x.size(); j++)
		{
			out *= exp(-(x[j] - c[i][j])*(x[j] - c[i][j])/(2*a[i][j]));
		}
		level3[i] = out;
	}
	return level3;
}

vector<float> ModelTSK0::getLevel4(vector<float> level3)
{
	vector<float> level4(2, 0.f);
	for (int i=0; i<num_rules; i++)
	{
		level4[0] += level3[i] * b0[i];
		level4[1] += level3[i];
	}
	return level4;
}

float ModelTSK0::getAnswer(vector<float> x)
{
	// calculate level 3
	vector<float> level3 = getLevel3(x);

	// calculate level 4
	vector<float> level4 = getLevel4(level3);
	
	// calculate answer
	float result = level4[0]/level4[1];
	return (float) floor(result + 0.5);
    //return result;
}