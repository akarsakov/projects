#ifndef __GRAPH_BUILDER__
#define __GRAPH_BUILDER__

#include "Data.h"
#include "UGraph.h"
#include "LRWeightCalculator.h"

#include <vector>
#include <iostream>
#include <math.h>
#include <iostream>
#include <sstream>

/*
 * Class for building graph for each example (using original algorithm)
 */
template<typename T=float, typename GraphType=UGraph<T>, typename WeightCalculatorType=LRWeightCalculator<T>>
class GraphBuilder
{
private:
	Data<T>& train;
	Data<T>& test;

	WeightCalculatorType* regressions;

	int num_features;
	long regSize;
public:
	GraphBuilder(Data<T>& _train, Data<T>& _test): train(_train), test(_test)
	{
		if (train.getNumFeatures() != test.getNumFeatures())
			throw std::logic_error("Error! Number of features in train and test file doesn't equal");
		num_features = train.getNumFeatures();
		regSize = num_features*(num_features-1)/2;
	};

	virtual ~GraphBuilder() 
	{ 
		delete[] regressions;
	};

	virtual void buildWeightCalculators();
	virtual void buildWeightedGraph(int example_index, GraphType* g);

	virtual void generateMetricsFile(std::string filename);
	virtual void writeGraphsToFiles(std::string prefix);
	virtual void writeEdgeHistogramToFile(std::string prefix, int num_banks);
	virtual void writeDegreesToFiles(std::string prefix);
    virtual void writeSignificantVertexToFile(std::string prefix);
};

template<typename T, typename GraphType, typename WeightCalculatorType>
void GraphBuilder<T, GraphType, WeightCalculatorType>::buildWeightCalculators()
{
	std::cout << "building weight calculators...";
	int num_train_examples = train.getNumExamples();
	regressions = new WeightCalculatorType[regSize];

	#pragma omp parallel for
	for (int i=0; i<num_features; i++)
	{
		for (int j=i+1; j<num_features; j++)
		{
			float* f1 = train.getFeatureVector(i);
			float* f2 = train.getFeatureVector(j);
			long ind = j*(j-1)/2 + i;
			regressions[ind].setModel(f1, f2, num_train_examples);
		}
	}
	std::cout << "end" << std::endl;
}

template<typename T, typename GraphType, typename WeightCalculatorType>
void GraphBuilder<T, GraphType, WeightCalculatorType>::buildWeightedGraph(int example_index, GraphType* g)
{
	std::cout << "building " << example_index << " graph...";
	int num_features = train.getNumFeatures();
	
	#pragma omp parallel for 
	for (int i=0; i<num_features; i++)
	{
		for (int j=i+1; j<num_features; j++)
		{
			long ind = j*(j-1)/2 + i;

			float t1 = test.getFeatureValue(i, example_index);
			float t2 = test.getFeatureValue(j, example_index);
			float weight = regressions[ind].getWeight(t1, t2);
			g->add_edge(i,j, weight);
		}
	}
	std::cout << "end" << std::endl;
}

template<typename T, typename GraphType, typename WeightCalculatorType>
void GraphBuilder<T, GraphType, WeightCalculatorType>::generateMetricsFile(std::string filename)
{
	int num_test_examples = test.getNumExamples();

	Data<T> out_metrics(13, num_test_examples);

	// create metrics file header
	std::vector<string> metrics_names;
	std::vector<string> feature_names = test.getFeatureNames();
	metrics_names.push_back("mean_degree");
	metrics_names.push_back("var_degree");
	metrics_names.push_back("max_degree");
	metrics_names.push_back("min_degree");
	metrics_names.push_back("mean_weight");
	metrics_names.push_back("var_weight");
	metrics_names.push_back("max_weight");
	metrics_names.push_back("min_weight");
	metrics_names.push_back("diameter");
	metrics_names.push_back("efficiency");
	metrics_names.push_back("mean_path");
	metrics_names.push_back("max_betw_cent");
	metrics_names.push_back("betw_cent");
	out_metrics.setFeatureNames(metrics_names);

	GraphType graph(num_features);

	buildWeightCalculators();

	for (int i=0; i<num_test_examples; i++)
	{
		buildWeightedGraph(i, &graph);

		cout << "calculating metrics: ";
		T mean, var, max, min;
		graph.getDegreeStatistics(mean, var, max, min);
		out_metrics.setFeatureValue(0, i, mean);
		out_metrics.setFeatureValue(1, i, var);
		out_metrics.setFeatureValue(2, i, max);
		out_metrics.setFeatureValue(3, i, min);
		cout << "degrees | ";

		graph.getEdgeWeightStatistic(mean, var, max, min);
		out_metrics.setFeatureValue(4, i, mean);
		out_metrics.setFeatureValue(5, i, var);
		out_metrics.setFeatureValue(6, i, max);
		out_metrics.setFeatureValue(7, i, min);
		cout << "weights | ";

		graph.invertWeights();
		cout << "invert weights | ";

		out_metrics.setFeatureValue(8, i, graph.diameterW());
		cout << "diameter ";
		out_metrics.setFeatureValue(9, i, graph.efficiency());
		graph.getShortestPathsStatistics(mean, var);
		out_metrics.setFeatureValue(10, i, mean);
		T betw_centrality;
		long max_b_centrality;
		graph.getBetweennessCentrality(betw_centrality, max_b_centrality);
		out_metrics.setFeatureValue(11, i, (T) max_b_centrality);
		out_metrics.setFeatureValue(12, i, betw_centrality);
		cout << "centrality" << endl;

		graph.clearGraph();
		out_metrics.writeExampleToFile(filename, i);
	}
}

template<typename T, typename GraphType, typename WeightCalculatorType>
void GraphBuilder<T, GraphType, WeightCalculatorType>::writeGraphsToFiles(std::string prefix)
{
	int num_test_examples = test.getNumExamples();
	GraphType graph(num_features);

	buildWeightCalculators();

	for (int i=0; i<num_test_examples; i++)
	{
		std::cout << "building " << i << " graph... ";
		buildWeightedGraph(i, &graph);

		std::cout << "thresholding... " << std::endl;
		int num_edges = 1000;
		double thres = graph.getQuantile(num_edges);
		graph.applyThreshold(thres);

		std::stringstream filename;
		filename << prefix << "_" << num_edges << "_" << i << ".csv";
		
		std::cout << "writing... ";
		graph.writeToFile(filename.str());
		graph.clearGraph();
		std::cout << "end" << endl;
	}
}

template<typename T, typename GraphType, typename WeightCalculatorType>
void GraphBuilder<T, GraphType, WeightCalculatorType>::writeEdgeHistogramToFile(std::string prefix, int num_banks)
{
	int num_test_examples = test.getNumExamples();
	GraphType graph(num_features);
	Data<int> histogram_file(num_banks, 1);
	int* hist = new int[num_banks];
	buildWeightCalculators();

	for (int i=0; i<num_test_examples; i++)
	{
		std::cout << "building " << i << " graph... ";
		buildWeightedGraph(i, &graph);

		T mean, var, min, max;
		graph.getEdgeWeightStatistic(mean, var, min, max);
		std::cout << "min: " << min << " max: " << max << std::endl; 

		graph.getWeightHistogram(hist, num_banks);
		histogram_file.setFeatureVector(0, hist);

		std::stringstream filename;
		filename << prefix << "_" << i << ".csv";
		std::cout << "writing... ";
		histogram_file.writeToFile(filename.str());
		graph.clearGraph();
		std::cout << "end" << endl;
	}
	delete[] hist;
}

template<typename T, typename GraphType, typename WeightCalculatorType>
void GraphBuilder<T, GraphType, WeightCalculatorType>::writeDegreesToFiles(std::string prefix)
{
	int num_test_examples = test.getNumExamples();
	GraphType graph(num_features);
	buildWeightCalculators();

	for (int i=0; i<num_test_examples; i++)
	{
		buildWeightedGraph(i, &graph);
		Data<double> degree_file(1, num_features);

		std::cout << "\tcalculating degrees...";
		for (long j=0; j<num_features; j++)
			degree_file.setFeatureValue(0, j, graph.avgDegree(j));
		std::cout << "end" << std::endl;

		std::stringstream filename;
		filename << prefix << "_" << i << ".csv";
		degree_file.writeToFile(filename.str());
	}
}

template<typename T, typename GraphType, typename WeightCalculatorType>
void GraphBuilder<T, GraphType, WeightCalculatorType>::writeSignificantVertexToFile(std::string prefix)
{
    int num_test_examples = test.getNumExamples();
	GraphType graph(num_features);
	buildWeightCalculators();
    long* centrality = new long[num_features];

	for (int i=0; i<num_test_examples; i++)
    {
        buildWeightedGraph(i, &graph);
		Data<T> file(3, num_features);

        cout << "calc degrees | ";
        for (long j=0; j<num_features; j++)
        {
            file.setFeatureValue(0, j, (T) j + 1);
			file.setFeatureValue(1, j, graph.avgDegree(j));
        }

        graph.invertWeights();
		cout << "invert weights | ";

        std::fill(centrality, centrality+num_features, 0);
        graph.getAllVertexBetweenness(centrality);
        for (long j=0; j<num_features; j++)
            file.setFeatureValue(2, j, (T) centrality[j]);

        std::stringstream filename;
		filename << prefix << "_" << i << ".csv";

        file.writeToFile(filename.str());
        graph.clearGraph();
		std::cout << "end" << endl;
    }
    delete[] centrality;
}

#endif /*__GRAPH_BUILDER__*/