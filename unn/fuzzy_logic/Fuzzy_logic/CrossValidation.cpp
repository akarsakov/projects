#include "CrossValidation.h"
#include <algorithm>
#include <iostream>

using namespace std;

CrossValidation::CrossValidation(Data data, int parts): validation_data(data), num_parts(parts)
{
    validation_data.shuffleExamples(1.f);
}

float CrossValidation::validate(KohonenNetworkParameters kohonenParams, ModelTSK0Parameters tskParams)
{
    validation_data.normalize(0,1);
	int testDataSize = validation_data.getNumExamples() / num_parts;
    float total_error = 0;
	for (int i=0; i<num_parts; i++)
	{
        cout << "################################" << endl;
        cout << "Experiment #" << i << endl;
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

        ModelTSK0 model(train, tskParams);
        model.modelIdentification(kohonenParams);
		model.modelOptimization();

        for (int j=0; j<test.getNumExamples(); j++)
        {
            int answer = (int) model.predict(test.getExampleXVector(j));
            int y = (int) test.getExampleY(j);
            if (answer != y)
                error += 1;
        }
        error /= test.getNumExamples();
        cout << "Error: " << error << endl;
        total_error += error;
	}
    
    float validation_error = (float) total_error / (num_parts);
    cout << "\n__________________________" << endl;
    cout << "Accuracy: " << 1.f - validation_error << endl;
    return validation_error;
}