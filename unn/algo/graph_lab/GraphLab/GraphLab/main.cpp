#include <fstream>
#include <chrono>
#include <string>

#include "GraphSPT.h"

using namespace std;
using namespace std::chrono;

void experiment1(string filename, int algo, int edge_strategy) {
    ofstream out(filename);

    for (int n=201; n<=10001; n+=200) {
        Graph g(n);
        double m;
        
        switch (edge_strategy) {
        case 0:
            m = n * n / 10;
            break;
        case 1:
            m = n * (n-1) / 2;
            break;
        case 2:
            m = 100 * n;
            break;
        case 3:
            m = 1000 * n;
            break;
        };

        g.generate(m, 1, 1000000);
        out << n << ", ";

        auto start = high_resolution_clock::now();
        
        switch (algo) {
        case 0:
            Deijkstra_SPT(g, 0, n-5);
            break;
        case 1:
            FordBellman_SPT(g, 0, n-5);
            break;
        };
        
        out << chrono::duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << endl;
        cout << n << ": done" << endl;
    }
}

void experiment2(string filename, int algo, int edge_strategy) {
    int n = 10001;
    ofstream out(filename);
    for (double m = 10000; m <= 10000000; m+=50000) {
        Graph g(n);

        g.generate(m, 1, 1000000);
        out << n << ", ";

        auto start = high_resolution_clock::now();
        
        switch (algo) {
        case 0:
            Deijkstra_SPT(g, 0, n-5);
            break;
        case 1:
            FordBellman_SPT(g, 0, n-5);
            break;
        };
        
        out << chrono::duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << endl;
        cout << n << ": done" << endl;
    }
}

void experiment3(string filename, int algo, int edge_strategy) {
    int n = 10001;
    double m;
    switch (edge_strategy) {
    case 0:
        m = n * n / 10;
        break;
    case 1:
        m = 1000 * n;
        break;
    };
    ofstream out(filename);

    for (double r = 1; r <= 200; r+=5) {
        Graph g(n);

        g.generate(m, 1, r);
        out << n << ", ";

        auto start = high_resolution_clock::now();
        
        switch (algo) {
        case 0:
            Deijkstra_SPT(g, 0, n-5);
            break;
        case 1:
            FordBellman_SPT(g, 0, n-5);
            break;
        };
        
        out << chrono::duration_cast<milliseconds>(high_resolution_clock::now() - start).count() << endl;
        cout << n << ": done" << endl;
    }
}

int main(int argc, char *argv[]) {
    srand(0);
    string filename(argv[1]);
    int exp_num = atoi(argv[2]);
    int algo = atoi(argv[3]);
    int parameter = atoi(argv[4]);

    switch (exp_num) {
    case 0:
        experiment1(filename, algo, parameter);
        break;
    case 1:
        experiment2(filename, algo, parameter);
        break;
    case 2:
        experiment3(filename, algo, parameter);
        break;
    };
    return 0;
}