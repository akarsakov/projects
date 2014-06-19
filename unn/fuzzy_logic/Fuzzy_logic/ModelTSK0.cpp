#include "Model_TSK0.h"
#include "KohonenNetwork.h"
#include <iostream>
#include <math.h>
using namespace std;

ModelTSK0::ModelTSK0(Data _data): train_data(_data)
{
	input_dimension = train_data.getNumFeatures();
	KohonenNetwork identification(train_data, 10, 0.01f, 0.005f, 0.00001f, 100);
	identification.trainNetwork();
	rules = identification.getRules();
	cout << "Number rules: " << rules.size() << endl;
	
	maxEpoch = 80;
	learningSpeed = 0.01f;
	eps = 0.001f;
}

void ModelTSK0::modelOptimization()
{
	int epochNumber = 0;
	float curError = 1.f; 
	do
	{
		epochNumber++;
		train_data.shuffleExamples(1.f);
		float sumError = 0.f;
		for (int i=0; i<train_data.getNumExamples(); i++)
		{
			vector<float> x = train_data.getExampleXVector(i);
			float y = train_data.getExampleY(i);

			// calc sums and answers
			float sum1 = 0.f;
			float sum2 = 0.f;
			vector<float> answers;
			for (size_t ruleId=0; ruleId<rules.size(); ruleId++)
			{
				float ans = rules[ruleId].getAnswer(x);
				answers.push_back(ans);
				sum1 += ans * rules[ruleId].getConfedence();
				sum2 += ans;
			}
			float error = sum1/sum2 - y;

			// calc deltas
			for (size_t ruleId=0; ruleId<rules.size(); ruleId++)
			{
				vector<float> old_a = rules[ruleId].getA();
				vector<float> old_c = rules[ruleId].getC();
				float factor = error*answers[ruleId]/sum2;

				for (int i=0; i<x.size(); i++)
				{
					if (x[i] < 0)
						continue;
					float centr = factor*rules[ruleId].getConfedence();
					float dif = x[i]-old_c[i];
					float aVal = old_a[i];
					old_c[i] -= learningSpeed*centr*dif/(aVal*aVal);
					old_a[i] -= learningSpeed*centr*dif*dif/(aVal*aVal*aVal);
				}
				float old_b = rules[ruleId].getConfedence();
				rules[ruleId].setConfedence(old_b - learningSpeed*factor);
				rules[ruleId].setA(old_a);
				rules[ruleId].setC(old_c);
			}
			sumError += error*error/2.f;
		}
		curError = sumError / train_data.getNumExamples();
		learningSpeed *= 1.0f - ((float) epochNumber)/maxEpoch;
	} while (epochNumber < maxEpoch || curError < eps);
	
	cout << "Trained!" << endl;
	cout << "\tEpoch number: " << epochNumber << endl;
	cout << "\tError: " << curError << endl;
}

float ModelTSK0::getAnswer(vector<float> x)
{
	float sum1 = 0.f;
	float sum2 = 0.f;

	for (size_t ruleId=0; ruleId<rules.size(); ruleId++)
	{
		float ans = rules[ruleId].getAnswer(x);
		sum1 += ans * rules[ruleId].getConfedence();
		sum2 += ans;
	}
	
	// calculate answer
	float result = sum1/sum2;
	return (float) floor(result + 0.5);
    //return result;
}