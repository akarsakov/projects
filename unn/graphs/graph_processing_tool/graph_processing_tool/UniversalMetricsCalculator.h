#ifndef __UNIVERSAL_METRICS_CALCULATOR__
#define __UNIVERSAL_METRICS_CALCULATOR__

#include <string>
#include <sstream>
#include <iostream>

#include "Data.h"
#include "UniversalGraph.h"

class UniversalMetricsCalculator
{
public:
    UniversalMetricsCalculator(int numExamples, std::string path):numTestExamples(numExamples), graphPath(path)  { 

    }

    void process(std::string outputFile) {
        std::string metricsNames[] = { "mean_weight", "var_weight", "min_weight", "max_weight",
                                       "mean_degree", "var_degree", "min_degree", "max_degree", "degree_centrality",
                                       "mean_distance", "min_distance", "max_distance",
                                       "max_betweeness", "betweeness centrality" };

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

            std::cout << "- calculating degree centrality.. ";
            float centrality;
            graph.getDegreeStatistics(mean_value, var_value, max_value, min_value, centrality);
            metrics.setFeatureValue(4, i, mean_value);
            metrics.setFeatureValue(5, i, var_value);
            metrics.setFeatureValue(6, i, max_value);
            metrics.setFeatureValue(7, i, min_value);
            metrics.setFeatureValue(8, i, centrality);
            std::cout << "end" << std::endl;

            int min_path, max_path;
            std::cout << "- calculating shortest paths.. " << std::endl;
            graph.getShortestPathsStatistics(mean_value, min_path, max_path);
            metrics.setFeatureValue(9, i, mean_value);
            metrics.setFeatureValue(10, i, (float) min_path);
            metrics.setFeatureValue(11, i, (float) max_path);
            std::cout << "end" << std::endl;

            std::cout << "- calculating betweeness centrality.. ";
            graph.getBetweennessCentrality(max_value, centrality);
            metrics.setFeatureValue(12, i, max_value);
            metrics.setFeatureValue(13, i, centrality);
            std::cout << "end" << std::endl;

            metrics.writeExampleToFile(outputFile, i);
        }
    }

private:
    int numTestExamples;
    std::string graphPath;
};

#endif /* __UNIVERSAL_METRICS_CALCULATOR__ */