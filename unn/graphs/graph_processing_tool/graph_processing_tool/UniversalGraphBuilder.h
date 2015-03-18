#ifndef __UNIVERSAL_GRAPH_BUILDER__
#define __UNIVERSAL_GRAPH_BUILDER__

#include <iostream>
#include <sstream>

#include <tbb\parallel_for.h>
#include <tbb\blocked_range.h>

#include "Data.h"
#include "UniversalGraph.h"
#include "LRWeightCalculator.h"

class UniversalGraphBuilder
{
public:
	UniversalGraphBuilder(Data<float>& _train, Data<float>& _test): train(_train), test(_test)
	{
		if (train.getNumFeatures() != test.getNumFeatures())
			throw std::logic_error("Error! Number of features in train and test file doesn't equal");
		numFeatures = train.getNumFeatures();
	};

    virtual ~UniversalGraphBuilder() {
        delete[] calculators;
    }

    void process(std::string output_path) {
        buildWeightCalculators();

        UniversalGraph graph(numFeatures);
        size_t numTestExamples = test.getNumExamples();

        for (size_t i=0; i<numTestExamples; i++) {
            std::cout << "======================================" << std::endl;
            std::cout << "Process: " << i << " graph:" << std::endl;
            buildWeightedGraph(i, &graph);

            std::cout << "save to file... ";
            std::stringstream ss;
            ss << output_path << "\\graph_" << i << ".bin";
            graph.writeToFile(ss.str());
            std::cout << " done." << std::endl;
            
            graph.clear();
        }
    }

private:

    void buildWeightCalculators() {
	    std::cout << "building weight calculators... ";
	    size_t numTrainExamples = train.getNumExamples();
        calculatorsSize = numFeatures*(numFeatures-1)/2;
	    calculators = new (std::nothrow) LRWeightCalculator<float>[calculatorsSize];
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
    LRWeightCalculator<float>* calculators;
};


#endif /* __UNIVERSAL_GRAPH_BUILDER__ */