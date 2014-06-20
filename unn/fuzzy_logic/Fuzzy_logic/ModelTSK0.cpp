#include "Model_TSK0.h"
#include "KohonenNetwork.h"
#include <iostream>
#include <math.h>
using namespace std;

static const float h = 0.00001f;

ModelTSK0::ModelTSK0(Data _data, ModelTSK0Parameters params): train_data(_data), 
    learningSpeed(params.learningSpeed), maxEpoch(params.maxEpoch), eps(params.epsilon)
{
	input_dimension = train_data.getNumFeatures();
}

void ModelTSK0::modelIdentification(KohonenNetworkParameters KohonenParams)
{
    KohonenNetwork identification(train_data, KohonenParams);
	identification.trainNetwork();
	rules = identification.getRules();
	cout << "\tNumber rules: " << rules.size() << endl;
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

            float ans = getAnswer(x);
            float fx = (ans-y)*(ans-y);

            // change weights
            vector<Rule> new_rules = rules;
            for (size_t ruleId=0; ruleId<rules.size(); ruleId++)
            {
                int dim = rules[ruleId].getA().size();
                
                // change a
                for (int i=0; i<dim; i++)
                {
                    vector<Rule> mod_rules = rules;
                    float old_a = mod_rules[ruleId].getA()[i];
                    mod_rules[ruleId].setA(i, old_a + h);
                    float fx_delta = (getAnswer(x, mod_rules) - y)*(getAnswer(x, mod_rules) - y);
                    float df = (fx_delta - fx) / h;
                    float new_a = old_a - learningSpeed*df;
                    if (new_a < eps)
                        new_a = eps;
                    new_rules[ruleId].setA(i, new_a);
                }

                // change c
                for (int i=0; i<dim; i++)
                {
                    vector<Rule> mod_rules = rules;
                    float old_c = mod_rules[ruleId].getC()[i];
                    mod_rules[ruleId].setC(i, old_c + h);
                    float fx_delta = (getAnswer(x, mod_rules) - y)*(getAnswer(x, mod_rules) - y);
                    float df = (fx_delta - fx) / h;
                    float new_c = old_c - learningSpeed*df;
                    if (new_c < 0.f)
                        new_c = +0.f;
                    if (new_c > 1.f)
                        new_c = 1.f;
                    new_rules[ruleId].setC(i, new_c);
                }

                //change b
                {
                    vector<Rule> mod_rules = rules;
                    float old_b = mod_rules[ruleId].getConfedence();
                    mod_rules[ruleId].setConfedence(old_b + h);
                    float fx_delta = (getAnswer(x, mod_rules) - y)*(getAnswer(x, mod_rules) - y);
                    float df = (fx_delta - fx) / h;
                    float new_b = old_b - learningSpeed*df;
                    new_rules[ruleId].setConfedence(new_b);
                }
            }

            rules = new_rules;
			sumError += fx/2.f;
		}
		curError = sumError / train_data.getNumExamples();
		learningSpeed *= 1.0f - ((float) epochNumber)/maxEpoch;
	} while (epochNumber < maxEpoch || curError < eps);
	
	cout << "\tModel TSK0 trained!" << endl;
	cout << "\tEpoch number: " << epochNumber << endl;
	cout << "\tError: " << curError << endl;
}

float ModelTSK0::predict(vector<float> x)
{
    return floor(getAnswer(x) + 0.5f);
}

float ModelTSK0::getAnswer(vector<float> x)
{
    return getAnswer(x, rules);
}

float ModelTSK0::getAnswer(vector<float> x, vector<Rule>& spec_rules)
{
	float sum1 = 0.f;
	float sum2 = 0.f;

	for (size_t ruleId=0; ruleId<spec_rules.size(); ruleId++)
	{
		float ans = spec_rules[ruleId].getAnswer(x);
		sum1 += ans * spec_rules[ruleId].getConfedence();
		sum2 += ans;
	}
	
	// calculate answer
	float result = sum1/sum2;
	//return (float) floor(result + 0.5);
    return result;
}