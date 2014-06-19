#include "CrossValidation.h"
#include "Model_TSK0.h"
#include <algorithm>
#include <iostream>

using namespace std;

CrossValidation::CrossValidation(Data data, int parts): validation_data(data), num_parts(parts)
{
    validation_data.shuffleExamples(1.f);
}

float CrossValidation::getError()
{
	int testDataSize = validation_data.getNumExamples() / num_parts;
    float total_error = 0;
	for (int i=0; i<num_parts; i++)
	{
        float error = 0;
		int startIndex = i*testDataSize;
		int endIndex = min(validation_data.getNumExamples(), (i+1)*testDataSize);

        vector<int> test_indexes;
        vector<int> train_indexes;

        for (int index=0; index<validation_data.getNumExamples(); index++)
        {
            if (index >= startIndex && index < endIndex)
                test_indexes.push_back(index);
            else
                train_indexes.push_back(index);
        }

        Data test = validation_data.getSubset(test_indexes);
        Data train = validation_data.getSubset(train_indexes);

        ModelTSK0 model(train);
		model.modelOptimization();

        for (int j=0; j<test.getNumExamples(); j++)
        {
            int answer = (int) model.getAnswer(test.getExampleXVector(j));
            int y = (int) test.getExampleY(j);
            if (answer != y)
                error += 1;
        }
        error /= test.getNumExamples();
        cout << "Error: " << error << endl;
        total_error += error;
	}
    return (float) total_error / (num_parts);
}