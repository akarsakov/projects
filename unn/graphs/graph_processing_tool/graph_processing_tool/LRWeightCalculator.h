#ifndef __LR_WEIGHT_CALCULATOR__
#define __LR_WEIGHT_CALCULATOR__
#include <vector>
#include "WeightCalculator.h"

template<typename T=float>
class LRWeightCalculator: public WeightCalculator<T>
{
private:
	T a;
	T b;
	T v;

public:
    LRWeightCalculator(): a(0.0), b(0.0), v(1.0) {};
    ~LRWeightCalculator() {};

    void setModel(const T* x, const T* y, const size_t size)
	{
		T Sx=0,Sy=0,Sxy=0,Sxx=0;

		for(int i=0; i < size; i++)
		{
			Sx+=x[i];
			Sy+=y[i];
			Sxy+=x[i]*y[i];
			Sxx+=x[i]*x[i];
		}

		Sx/=size;
		Sy/=size;
		Sxy/=size;
		Sxx/=size;

		a=(Sx*Sy-Sxy)/(Sx*Sx-Sxx);
		b=(Sxy-a*Sxx)/Sx;

		std::vector<T> error(size);
		for (int i=0; i<size; i++)
			error.push_back(y[i] - a*x[i]+b);

		v = (T) var(error);
	}

    T getWeight(const T x, const T y)
	{
		return (T) fabs(y - (a*x+b))/v;
	}
};

#endif /*__LR_WEIGHT_CALCULATOR__*/