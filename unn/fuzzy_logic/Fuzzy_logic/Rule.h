#ifndef __RULE__
#define __RULE__

#include <vector>

class Rule
{
private:
	std::vector<float> a;
	std::vector<float> c;
	float b0;

public:
	Rule() { };
	Rule(std::vector<float> _a, std::vector<float> _c, float b=0.f);
	float getAnswer(std::vector<float> x);
	
	void setConfedence(float b) { b0 = b; };
	float getConfedence() { return b0; };

	void setA(std::vector<float> _a) { a = _a; };
	std::vector<float> getA() { return a; };

	void setC(std::vector<float> _c) { c = _c; };
	std::vector<float> getC() { return c; };
};

#endif /*__RULE__*/