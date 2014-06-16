#include "Model_TSK0.h"
#include "KohonenNetwork.h"

using namespace std;

ModelTSK0::ModelTSK0(Data _data): train_data(_data)
{
	KohonenNetwork identification(train_data, train_data.getNumClasses(), 0.1f, 0.1f, 0.00001f, 5);
	identification.trainNetwork();
	c = identification.getCenters();
	vector<float> gaussWidths = identification.getGaussWidths();
	a.resize(gaussWidths.size());
	for (int centerId=0; centerId<gaussWidths.size(); centerId++)
		a[centerId].resize(train_data.getNumFeatures(), gaussWidths[centerId]);
	b0 = identification.getVectorB();
}

void ModelTSK0::modelOptimization()
{

}