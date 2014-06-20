#ifndef __MODEL_TSK__
#define __MODEL_TSK__

#include <vector>
#include "Data.h"
#include "KohonenNetwork.h"
#include "Rule.h"

struct ModelTSK0Parameters
{
    int maxEpoch;
    float learningSpeed;
    float epsilon;

    ModelTSK0Parameters(int maxEp, float learningS, float eps): maxEpoch(maxEp),
        learningSpeed(learningS), epsilon(eps)
    { };
};

class ModelTSK0
{
private:
	Data train_data;
	std::vector<Rule> rules;
	
	int input_dimension;
	int num_rules;
	int maxEpoch;
	float learningSpeed;
	float eps;
public:
	ModelTSK0(Data data, ModelTSK0Parameters params);
    void modelIdentification(KohonenNetworkParameters params);
	void modelOptimization();
	float predict(std::vector<float> x);
private:
    float getAnswer(std::vector<float> x);
    float getAnswer(std::vector<float> x, std::vector<Rule>& specific_rules);
};

#endif /*__MODEL_TSK__*/