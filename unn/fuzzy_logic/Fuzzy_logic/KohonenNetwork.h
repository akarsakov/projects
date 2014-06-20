#ifndef __KOHONEN_NETWORK__
#define __KOHONEN_NETWORK__

#include <vector>
#include "Data.h"
#include "Rule.h"

typedef std::vector<float> Point;

struct KohonenNetworkParameters
{
    int num_clusters;
    int maxEpoch;
    float alpha_winner;
    float alpha_loser;
    float epsilon;

    KohonenNetworkParameters(int num_cl, int maxEp, float alpha_w, float alpha_l, float eps):
        num_clusters(num_cl), maxEpoch(maxEp), alpha_winner(alpha_w), alpha_loser(alpha_l), epsilon(eps)
    { };
};

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
    KohonenNetwork(Data& d, KohonenNetworkParameters params);
    ~KohonenNetwork() { };

    void trainNetwork();
	std::vector<Rule> getRules();

private:
    float EuclidDistance(Point x, Point y);
    void updateCenter(Point x, int index, float alpha);
	void filterCenters();
};

#endif /*__KOHONEN_NETWORK__*/