#ifndef __MODEL_TSK__
#define __MODEL_TSK__

#include <vector>
#include "Data.h"
#include "Rule.h"

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
	ModelTSK0(Data data);
	void modelOptimization();
	float getAnswer(std::vector<float> x);
};

#endif /*__MODEL_TSK__*/