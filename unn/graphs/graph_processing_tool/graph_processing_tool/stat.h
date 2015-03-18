#ifndef __STAT__
#define __STAT__
#include <vector>
#include <numeric>

// Statistics function

template<typename T>
double mean(std::vector<T> in)
{
    double sum = std::accumulate(in.begin(), in.end(), 0.0);
    return sum/in.size();
}

template<typename T>
double mean(T* in, size_t size)
{
    double sum = std::accumulate(in, in+size, 0.0);
    return sum/size;
}

template<typename T>
double var(std::vector<T> in)
{
    double mean_value = mean(in);
    double sum = std::accumulate(in.begin(), in.end(), 0.0, 
        [=](double res, double elem) -> double {
            res += (elem - mean_value)*(elem - mean_value);
            return res;
    });
    return sum/(in.size() - 1);
}

template<typename T>
double var(T* in, size_t size)
{
    double mean_value = mean(in, size);
    double sum = std::accumulate(in, in+size, 0.0, 
        [=](double res, double elem) -> double {
            res += (elem - mean_value)*(elem - mean_value);
            return res;
    });
    return sum/(size - 1);
}

#endif /*__STAT__*/