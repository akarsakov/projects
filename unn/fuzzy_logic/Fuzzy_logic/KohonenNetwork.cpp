#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <iterator>
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
			new_center.push_back((float) rand() / RAND_MAX);
        centers[i] = new_center;
    }
};

float KohonenNetwork::EuclidDistance(Point x, Point y)
{
    if (x.size() != y.size() || y.size() != dimension)
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
	float shift;
    do
    {
        epoch_number++;
		vector<vector<float>> copy_centers = centers;
        for (int i=0; i<data.getNumExamples(); i++)
        {
            Point x = data.getExampleXVector(i);
			int w, r;
			float w_val, r_val = FLT_MAX;
			vector<float> distances;
            for (int j=0; j<num_clusters; j++)
            {
                float distance = num_victories[j]*EuclidDistance(x, centers[j]) / victories;
				distances.push_back(distance);
			}

			w_val = *min_element(distances.begin(), distances.end());
			w = distance(distances.begin(), min_element(distances.begin(), distances.end()));

			for (int j=0; j<num_clusters; j++)
			{
				if (distances[j] < r_val && distances[j] > w_val)
				{
					r_val = distances[j];
					r = j;
				}
            }

			updateCenter(x, w, alpha_w);
			updateCenter(x, r, -alpha_l);
			num_victories[w] += 1;
			victories++;
			alpha_w -= alpha_w*epoch_number/max_epoch;
			alpha_l -= alpha_l*epoch_number/max_epoch;
        }
		shift = 0.f;
		for (int i=0; i<num_clusters; i++)
		{
			shift += EuclidDistance(copy_centers[i], centers[i]);
		}
		shift /= num_clusters;
	} while (epoch_number < max_epoch && shift > eps);
	
	gaussWidths.resize(centers.size(), FLT_MAX);
	b0.resize(centers.size(), 0);
	
	// calculate Gauss widths
	for (int i=0; i<centers.size(); i++)
	{
		for (int j=0; j<centers.size(); j++)
		{
			if (i == j)
				continue;
			gaussWidths[i] = min(EuclidDistance(centers[i], centers[j]), gaussWidths[i]) / 1.5;
		}
	}

	//calculate b0
	for (int centerId=0; centerId<centers.size(); centerId++)
	{
		float sum_b = 0.f;
		float sum_alpha = 0.f;

		for (int exampleId=0; exampleId<data.getNumExamples(); exampleId++)
		{
			float alpha = 1.f;
			vector<float> example = data.getExampleXVector(exampleId);
			for (int i=0; i<dimension; i++)
			{
				alpha *= exp(-(example[i] - centers[centerId][i])*(example[i] - centers[centerId][i]) / (2*gaussWidths[centerId]));
			}
			sum_alpha += alpha;
			sum_b += alpha * data.getExampleY(exampleId);
		}

		b0[centerId] = sum_b / sum_alpha;
	}
}

vector<vector<float>> KohonenNetwork::getCenters()
{
	return centers;
}

vector<float> KohonenNetwork::getGaussWidths()
{
	return gaussWidths;
}

vector<float> KohonenNetwork::getVectorB()
{
	return b0;
}

void KohonenNetwork::updateCenter(Point x, int index, float alpha)
{
	for (int i=0; i<dimension; i++)
	{
		if (x[i] < 0)
			continue;
		centers[index][i] = centers[index][i] + alpha*(x[i] - centers[index][i]);
	}
}