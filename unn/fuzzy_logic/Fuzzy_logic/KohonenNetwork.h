#ifndef __KOHONEN_NETWORK__
#define __KOHONEN_NETWORK__

#include <vector>
#include "Data.h"

typedef std::vector<float> Point;

class KohonenNetwork
{
private:
    Data& data;
    int num_clusters;
    float alpha_w;
    float alpha_l;
    float eps;
    int max_epoch;
    int dimension;

    std::vector<Point> centers;
	std::vector<float> gaussWidths;
	std::vector<float> b0;
    std::vector<int> num_victories;
public:
    KohonenNetwork(Data& d, int num_clusters, float alpha_winner, float alpha_loser, float epsilon, int max_epoch);
    ~KohonenNetwork() { };

    void trainNetwork();
	std::vector<std::vector<float>> getCenters();
	std::vector<float> getGaussWidths();
	std::vector<float> getVectorB();

private:
    float EuclidDistance(Point x, Point y);
    void updateCenter(Point x, int index, float alpha);
	void filterCenters();
};

#endif /*__KOHONEN_NETWORK__*/