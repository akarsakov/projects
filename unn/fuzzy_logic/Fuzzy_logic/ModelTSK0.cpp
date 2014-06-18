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
	for (int centerId=0; centerId<gaussWidths.size(); centerId++)
		a[centerId].resize(train_data.getNumFeatures(), gaussWidths[centerId]);
	b0 = identification.getVectorB();
}

void ModelTSK0::modelOptimization()
{

}

float ModelTSK0::getAnswer(vector<float> x)
{
	// calculate level 3
	vector<float> level3(num_rules);
	for (int i=0; i<num_rules; i++)
	{
		float out = 1.f;
		for (int j=0; j<x.size(); j++)
		{
			float e = exp(-(x[j] - c[i][j])*(x[j] - c[i][j])/(2*a[i][j]));
			out *= e*e;
		}
		level3[i] = out;
	}

	// calculate level 4
	float sum1 = 0.f;
	float sum2 = 0.f;
	for (int i=0; i<num_rules; i++)
	{
		sum1 += level3[i] * b0[i];
		sum2 += level3[i];
	}

	// calculate answer
	float result = sum1/sum2;
	return floor(result + 0.5);
    //return result;
}