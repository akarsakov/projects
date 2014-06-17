#ifndef __CROSS_VALIDATION__
#define __CROSS_VALIDATION__

#include "Data.h"

class CrossValidation
{
private:
	Data validation_data;
	int num_parts;

public:
	CrossValidation(Data data, int num_parts);
	float getError();
};

#endif /*__CROSS_VALIDATION__*/