#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include "KohonenNetwork.h"
#include "utils.h"

using namespace std;

KohonenNetwork::KohonenNetwork(Data& d, int num_cl, float alpha_winner, float alpha_loser, float epsilon, int max_e): 
    data(d), num_clusters(num_cl), alpha_w(alpha_winner), alpha_l(alpha_loser), eps(epsilon), max_epoch(max_e) 
{
    if (num_clusters < 0 || alpha_w < 0.f || alpha_l < 0.f || eps < 0.f)
    {
        stringstream msg;
        msg << "Incorrect parameters for KohonenNetwork (clusters: " << num_clusters << ", alpha_w: " 
            << alpha_w << "alpha_l: " << alpha_l << "eps: " << eps;
        Error(msg.str());
    }
    dimension = data.getNumFeatures() - 1;
    num_victories.resize(num_clusters, 1);

    srand(time(NULL));
    centers.resize(num_clusters);
    for (int i=0; i<num_clusters; i++)
    {
        Point new_center;
        for (int j=0; j<dimension; j++)
            new_center[j] = (float) rand() / RAND_MAX;
        centers[i] = new_center;
    }
};

float KohonenNetwork::EuclidDistance(Point x, Point y)
{
    if (x.size() != y.size() != dimension)
        Error("Points dimensions not equal");

    float dist = 0.f;
    for (int i=0; i<dimension; i++)
    {
        if (x[i] < 0 || y[i] < 0)
            continue;
        dist += (x[i] - y[i])*(x[i] - y[i]);
    }
    return dist;
}

void KohonenNetwork::trainNetwork()
{
    int victories = num_clusters;
    int epoch_number = 0;

    do
    {
        epoch_number++;
        for (int i=0; i<data.getNumExamples(); i++)
        {
            Point x = data.getExampleVector(i);
            vector<float> distances(num_clusters);
            for (int j=0; j<num_clusters; j++)
            {
                distances.push_back(num_victories[j]*EuclidDistance(x, centers[j]) / victories);

            }


        }



    } while (epoch_number < max_epoch);
}