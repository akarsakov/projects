#ifndef __MODEL_TSK__
#define __MODEL_TSK__

#include <vector>
#include "Data.h"

class ModelTSK0
{
private:
	Data train_data;

	std::vector<std::vector<float>> c;
	std::vector<std::vector<float>> a;
	std::vector<float> b0;
	int input_dimension;
	int num_rules;
public:
	ModelTSK0(Data data);

	void modelOptimization();

	float getAnswer(std::vector<float> x);


};

#endif /*__MODEL_TSK__*/