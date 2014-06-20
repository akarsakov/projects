#ifndef __CROSS_VALIDATION__
#define __CROSS_VALIDATION__

#include "Data.h"
#include "KohonenNetwork.h"
#include "Model_TSK0.h"

class CrossValidation
{
private:
	Data validation_data;
	int num_parts;
public:
	CrossValidation(Data data, int num_parts);
    float validate(KohonenNetworkParameters kohonenParams, ModelTSK0Parameters tskParams);
};

#endif /*__CROSS_VALIDATION__*/