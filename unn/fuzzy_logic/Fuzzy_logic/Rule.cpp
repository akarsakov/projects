#include "Rule.h"
#include <math.h>

using namespace std;

Rule::Rule(vector<float> _a, vector<float> _c, float b): a(_a), c(_c), b0(b)
{
}

float Rule::getAnswer(vector<float> x)
{
	float answer = 1.0f;
	for (size_t i = 0; i < x.size(); i++) 
	{
		if (x[i] < 0)
			continue;
		answer *= (float) exp(-(x[i]-c[i])*(x[i]-c[i])/(2.f*a[i]));
	}
	return answer;
}