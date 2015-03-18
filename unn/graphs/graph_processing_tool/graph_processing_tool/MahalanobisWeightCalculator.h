#ifndef __MAHALANOBIS_WEIGHT_CALCULATOR__
#define __MAHALANOBIS_WEIGHT_CALCULATOR__

#include <vector>
#include <math.h>
#include <limits>
#include "WeightCalculator.h"

#define COEF 100

template<typename T>
class MahalanobisWeightCalculator: public WeightCalculator<T>
{
private:
	T x0, y0, a, b, d;

public:
    MahalanobisWeightCalculator(): x0(0.0), y0(0.0), a(0.0), b(0.0), d(0.0) {};
    ~MahalanobisWeightCalculator() {};

	void setModel(const T* x, const T* y, const int size)
	{
		T meanX = 0, meanY = 0, meanX2 = 0, meanY2 = 0, meanXY = 0;
        
		for (int i=0; i < size; i++)
		{
			meanX+=x[i];
			meanY+=y[i];
			meanX2+=x[i]*x[i];
			meanY2+=y[i]*y[i];
			meanXY+=x[i]*y[i];
		}

		meanX  /= size;
		meanY  /= size;
		meanX2 /= size;
		meanY2 /= size;
		meanXY /= size;

		// calc covariation matrix
		a = COEF*(meanX2 - meanX*meanX);
		b = COEF*(meanXY - meanX*meanY);
		d = COEF*(meanY2 - meanY*meanY);

		// invert covariation matrix
		T det = a*d - b*b;
		if (fabs(det) < 1e-8)
			throw std::logic_error("Singular matrix");
		
		a = d/det;
		b = -b/det;
		d = a/det;

		x0 = meanX;
		y0 = meanY;

		//// calc closest point to mean
		//T dist = std::numeric_limits<T>::max();
		//for (int i=0; i < size; i++)
		//{
		//	T d=(x[i]-meanX)*(x[i]-meanX)+(y[i]-meanY)*(y[i]-meanY);
		//	if (d<dist) 
		//	{
		//		dist=d; 
		//		x0=x[i]; 
		//		y0=y[i];
		//	}
		//}
	}

	T getWeight(const T x, const T y)
	{
		T temp1 = (x - x0)*a + (y - y0)*b;
		T temp2 = (x - x0)*b + (y - y0)*d;
		return sqrt(fabs(temp1*(x - x0) + temp2*(y - y0)));
	}

	void getWeight(const T* x, const T* y, T* out, int size)
	{
		for (int i=0; i<size; i++)
			out[i] = getWeight(x[i], y[i]);
	}	
	
	void getWeight(const std::vector<T>& x, const std::vector<T>& y, std::vector<T>& out)
	{
		for (int i=0; i<x.size(); i++)
            out[i].push_back(getWeight(x[i], y[i]));
	}
};

#endif