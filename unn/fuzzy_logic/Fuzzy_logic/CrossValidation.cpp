#include "CrossValidation.h"
#include <algorithm>

using namespace std;

CrossValidation::CrossValidation(Data data, int parts): validation_data(data), num_parts(parts)
{

}

float CrossValidation::getError()
{
	int testDataSize = validation_data.getNumExamples() / num_parts;
	for (int i=0; i<num_parts; i++)
	{
		int startIndex = i*testDataSize;
		int endIndex = min(validation_data.getNumExamples(), (i+1)*testDataSize);


	}
}