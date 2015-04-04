#ifndef __UNIVERSAL_METRICS_CALCULATOR__
#define __UNIVERSAL_METRICS_CALCULATOR__

#include <string>
#include <sstream>
#include <iostream>

#include "Data.h"
#include "UniversalGraph.h"
#include "MahalanobisWeightCalculator.h"

class UniversalMetricsCalculator
{
public:
    UniversalMetricsCalculator(int numExamples, std::string path):numTestExamples(numExamples), graphPath(path)  { 

    }

    void process(std::string outputFile) {
        std::string metricsNames[] = { "mean_weight", "var_weight", "min_weight", "max_weight",
                                       "mean_degree", "var_degree", "min_degree", "max_degree", "degree_centrality" };

        std::vector<std::string> names(std::begin(metricsNames), std::end(metricsNames));
        Data<float> metrics(names.size(), numTestExamples);
        metrics.setFeatureNames(names);

        UniversalGraph graph(1);

        for (size_t i=0; i<numTestExamples; i++) {
            std::cout << "=============================" << std::endl 
                      << "Process " << i << " graph of " << numTestExamples << std::endl;
            std::stringstream ss;
            ss << graphPath << "\\graph_" << i << ".bin";
            std::cout << "- reading graph from file...";
            graph.readFromFile(ss.str());
            std::cout << "done" << std::endl;

            float mean_value, var_value, min_value, max_value;
            std::cout << "- calculating edge weight statistics... ";
            graph.getEdgeWeightStatistic(mean_value, var_value, max_value, min_value);
            std::cout << "end" << std::endl;
            metrics.setFeatureValue(0, i, mean_value);
            metrics.setFeatureValue(1, i, var_value);
            metrics.setFeatureValue(2, i, max_value);
            metrics.setFeatureValue(3, i, min_value);

            float sd = sqrtf(var_value);
            std::cout << "- thresholding... ";
            graph.applyThreshold(sd*0.75);
            std::cout << "end" << std::endl;

            std::cout << "- non-zero: " << graph.countNonZero() << std::endl;

            std::cout << "- calculating degree centrality.. ";
            float centrality;
            graph.getDegreeStatistics(mean_value, var_value, max_value, min_value, centrality);
            metrics.setFeatureValue(4, i, mean_value);
            metrics.setFeatureValue(5, i, var_value);
            metrics.setFeatureValue(6, i, max_value);
            metrics.setFeatureValue(7, i, min_value);
            metrics.setFeatureValue(8, i, centrality);
            std::cout << "end" << std::endl;

            /*int min_path, max_path;
            std::cout << "- calculating shortest paths.. " << std::endl;
            graph.getShortestPathsStatistics(mean_value, min_path, max_path);
            metrics.setFeatureValue(9, i, mean_value);
            metrics.setFeatureValue(10, i, (float) min_path);
            metrics.setFeatureValue(11, i, (float) max_path);
            std::cout << "end" << std::endl;*/

            /*std::cout << "- calculating betweeness centrality.. ";
            graph.getBetweennessCentrality(max_value, centrality);
            metrics.setFeatureValue(12, i, max_value);
            metrics.setFeatureValue(13, i, centrality);
            std::cout << "end" << std::endl;*/

            metrics.writeExampleToFile(outputFile, i);
            graph.clear();
        }
    }

private:
    int numTestExamples;
    std::string graphPath;
};

class UniversalMetricsCalculator2
{
public:
    UniversalMetricsCalculator2(Data<float>& _train, Data<float>& _test): train(_train), test(_test)
	{
		if (train.getNumFeatures() != test.getNumFeatures())
			throw std::logic_error("Error! Number of features in train and test file doesn't equal");
		numFeatures = train.getNumFeatures();
	};

    void process(std::string outputFile, float coef) {
        std::string metricsNames[] = { "mean_weight", "var_weight", "min_weight", "max_weight",
                                       "mean_degree", "var_degree", "min_degree", "max_degree", "degree_centrality" };

        std::vector<std::string> names(std::begin(metricsNames), std::end(metricsNames));
        Data<float> metrics(names.size(), test.getNumExamples());
        metrics.setFeatureNames(names);

        buildWeightCalculators();

        UniversalGraph graph(numFeatures);

        for (size_t i=0; i<test.getNumExamples(); i++) {
            std::cout << "=============================" << std::endl 
                      << "Process " << i+1 << " graph of " << test.getNumExamples() << std::endl;
            buildWeightedGraph(i, &graph);

            float mean_value, var_value, min_value, max_value;
            std::cout << "- calculating edge weight statistics... ";
            graph.getEdgeWeightStatistic(mean_value, var_value, max_value, min_value);
            std::cout << "end" << std::endl;
            metrics.setFeatureValue(0, i, mean_value);
            metrics.setFeatureValue(1, i, var_value);
            metrics.setFeatureValue(2, i, max_value);
            metrics.setFeatureValue(3, i, min_value);

            float sd = sqrtf(var_value);
            std::cout << "- thresholding... ";
            graph.applyThreshold(coef*sd);
            std::cout << "end" << std::endl;

            std::cout << "- non-zero: " << graph.countNonZero() << std::endl;

            std::cout << "- calculating degree centrality.. ";
            float centrality;
            graph.getDegreeStatistics(mean_value, var_value, max_value, min_value, centrality);
            metrics.setFeatureValue(4, i, mean_value);
            metrics.setFeatureValue(5, i, var_value);
            metrics.setFeatureValue(6, i, max_value);
            metrics.setFeatureValue(7, i, min_value);
            metrics.setFeatureValue(8, i, centrality);
            std::cout << "end" << std::endl;

            metrics.writeExampleToFile(outputFile, i);
            graph.clear();
        }
    }

private:

    void buildWeightCalculators() {
	    std::cout << "building weight calculators... ";
	    size_t numTrainExamples = train.getNumExamples();
        calculatorsSize = numFeatures*(numFeatures-1)/2;
	    calculators = new (std::nothrow) MahalanobisWeightCalculator<float>[calculatorsSize];
        if (calculators == NULL)
            std::cout << "Can't allocate memory for linear regressions" << std::endl;

        tbb::blocked_range<size_t> r(0, numFeatures);
        tbb::parallel_for(tbb::blocked_range<size_t>(0, numFeatures),
            [=] (const tbb::blocked_range<size_t>& r) {
                for (size_t i=r.begin(); i<r.end(); i++) {
		            for (size_t j=i+1; j<numFeatures; j++) {
			            float* f1 = train.getFeatureVector(i);
			            float* f2 = train.getFeatureVector(j);
			            size_t ind = j*(j-1)/2 + i;
			            calculators[ind].setModel(f1, f2, numTrainExamples);
		            }
	            }
            });

	    std::cout << "end" << std::endl;
    }

    void buildWeightedGraph(size_t exampleIndex, UniversalGraph* g) {
	    std::cout << "- building " << exampleIndex << " graph... ";
	
        tbb::parallel_for(tbb::blocked_range<size_t>(0, numFeatures),
            [=] (const tbb::blocked_range<size_t>& r) {
	            for (size_t i=r.begin(); i<r.end(); i++) {
		            for (size_t j=i+1; j<numFeatures; j++) {
			            size_t ind = j*(j-1)/2 + i;

			            float t1 = test.getFeatureValue(i, exampleIndex);
			            float t2 = test.getFeatureValue(j, exampleIndex);
			            float weight = calculators[ind].getWeight(t1, t2);
			            g->addEdge(i,j, weight);
		            }
	            }
            });

	    std::cout << "end" << std::endl;
    }



    Data<float>& train;
    Data<float>& test;

    size_t numFeatures;
    size_t calculatorsSize;
    MahalanobisWeightCalculator<float>* calculators;
};

#endif /* __UNIVERSAL_METRICS_CALCULATOR__ */