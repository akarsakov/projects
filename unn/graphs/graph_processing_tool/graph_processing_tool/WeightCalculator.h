#ifndef __WEIGHT_CALCULATOR__
#define __WEIGHT_CALCULATOR__

#include <vector>

template<typename T>
class WeightCalculator
{
public:
	WeightCalculator() { };
	~WeightCalculator() { };

	virtual void setModel(const T* x, const T* y, const size_t size) = 0;

	void setModel(const std::vector<T>& x, const std::vector<T>& y)
	{
		if (x.size() != y.size())
			throw std::logic_error("Vector lengths not equal");
		setModel(x.begin(), y.begin(), (int) x.size());
	}

	virtual T getWeight(const T x, const T y) = 0;
	
	void getWeight(const T* x, const T* y, T* out, const size_t size)
	{
		for (int i=0; i<size; i++)
			out[i] = getWeight(x[i], y[i]);
	}

	void getWeight(const std::vector<T>& x, const std::vector<T>& y, std::vector<T>& out)
	{
		for (size_t i=0; i<x.size(); i++)
			out.push_back(getWeight(x[i], y[i]));
	}
};

#endif