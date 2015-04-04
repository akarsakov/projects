#include <iostream>
#include <string>
#include <chrono>
#include <fstream>

#include "UniversalGraphBuilder.h"
#include "UniversalMetricsCalculator.h"


using namespace std;

int main(int argc, char *argv[])
{    
    //UniversalGraph g(8);
    //g.addEdge(0, 1, 1);
    //g.addEdge(0, 2, 1);
    //g.addEdge(0, 3, 1);
    //g.addEdge(4, 1, 1);
    //g.addEdge(4, 2, 1);
    //g.addEdge(4, 3, 1);
    //g.addEdge(4, 5, 1);
    //g.addEdge(4, 6, 1);
    //g.addEdge(7, 5, 1);
    //g.addEdge(7, 6, 1);
    //float maxCentrality, centrality;
    //g.getBetweennessCentrality(maxCentrality, centrality);
    //cout << "max centrality = " << maxCentrality << ", centrality = " << centrality << endl;

    //UniversalGraph g = UniversalGraph(4);
    //g.addEdge(0, 1, 1);
    //g.addEdge(0, 2, 1);
    //g.addEdge(0, 3, 1);
    //g.getBetweennessCentrality(maxCentrality, centrality);
    //cout << "max centrality = " << maxCentrality << ", centrality = " << centrality << endl;
    //return 0;

    string operation(argv[1]);
    if (operation == "generate") {
	    string train_file(argv[2]);
	    string test_file(argv[3]);
	    string output(argv[4]);
        float coef = atof(argv[5]);
	    
	    cout << "reading train file... ";
	    Data<float> train(train_file);
	    cout << "end" << endl;
	    cout << "Summary: " << train_file << endl;
	    cout << "Number of train features: " << train.getNumFeatures() << endl;
	    cout << "Number of train examples: " << train.getNumExamples() << endl;

	    cout << "reading test file... ";
	    Data<float> test(test_file);
	    cout << "end" << endl;
	    cout << "Summary: " << test_file << endl;
	    cout << "Number of test features: " << test.getNumFeatures() << endl;
	    cout << "Number of test examples: " << test.getNumExamples() << endl;

        //UniversalGraphBuilder builder(train, test);
        UniversalMetricsCalculator2 calc(train, test);

	    auto start = std::chrono::system_clock::now();

        calc.process(output, coef);

	    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start);
	    cout << "Done!" << endl;
	    cout << "Execution time: " << duration.count() / 60 << " min " << duration.count() % 60 << "sec" << endl;
    } else if (operation == "metrics") {
        string graphPath(argv[2]);
        string outFile(argv[4]);
        int numGraphs = atoi(argv[3]);
        

        UniversalMetricsCalculator calc(numGraphs, graphPath);

	    auto start = std::chrono::system_clock::now();
        calc.process(outFile);

	    auto duration = std::chrono::duration_cast<std::chrono::seconds>(std::chrono::system_clock::now() - start);
	    cout << "Done!" << endl;
	    cout << "Execution time: " << duration.count() / 60 << " min " << duration.count() % 60 << "sec" << endl;
    }
	return 0;
}