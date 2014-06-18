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
	int maxEpoch;
	float sigma;
public:
	ModelTSK0(Data data);

	void modelOptimization();

	float getAnswer(std::vector<float> x);

private:
	std::vector<float> getLevel3(std::vector<float> x);
	std::vector<float> getLevel4(std::vector<float> level3);


};

#endif /*__MODEL_TSK__*/