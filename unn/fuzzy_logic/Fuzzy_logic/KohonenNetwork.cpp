#include <sstream>
#include <stdlib.h>
#include <time.h>
#include <algorithm>
#include <functional>
#include <iterator>
#include <iostream>
#include "KohonenNetwork.h"
#include "utils.h"

using namespace std;

KohonenNetwork::KohonenNetwork(Data& d, KohonenNetworkParameters params): 
    data(d), num_clusters(params.num_clusters), alpha_w(params.alpha_winner), alpha_l(params.alpha_loser), 
    eps(params.epsilon), max_epoch(params.maxEpoch) 
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

    srand((unsigned int) time(NULL));
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
    return sqrt(dist);
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

	filterCenters();

    cout << "\tKohonen network trained!" << endl;
    cout << "\tEpoch number: " << epoch_number << endl;
    cout << "\tNumber of centers: " << centers.size() << endl;
	
	gaussWidths.resize(centers.size(), FLT_MAX);
	b0.resize(centers.size(), 0);
	
	// calculate Gauss widths
	for (size_t i=0; i<centers.size(); i++)
	{
		for (size_t j=0; j<centers.size(); j++)
		{
			if (i == j)
				continue;
			gaussWidths[i] = (float) min(EuclidDistance(centers[i], centers[j]), gaussWidths[i]) / 1.5f;
		}
	}

	//calculate b0
	for (size_t centerId=0; centerId<centers.size(); centerId++)
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

vector<Rule> KohonenNetwork::getRules()
{
	vector<Rule> rules;
	for (size_t centerId=0; centerId<centers.size(); centerId++)
	{
		vector<float> a(dimension, gaussWidths[centerId]);
		Rule rule(a, centers[centerId], b0[centerId]);
		rules.push_back(rule);
	}
	return rules;
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

void KohonenNetwork::filterCenters()
{
	for (size_t i=0; i<centers.size();)
	{
		bool inRange = true;
		for (int j=0; j<dimension; j++)
		{
			if (centers[i][j] < 0 || centers[i][j] > 1)
			{
				inRange = false;
				break;
			}
		}
		if (!inRange)
			centers.erase(centers.begin()+i);
		else
			i++;
	}
}