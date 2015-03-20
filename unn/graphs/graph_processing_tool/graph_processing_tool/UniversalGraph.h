#ifndef __UNIVERSAL_GRAPH__
#define __UNIVERSAL_GRAPH__

#include <algorithm>
#include <limits>
#include <list>
#include <stack>
#include <queue>
#include <iostream>

#include <tbb\parallel_for.h>
#include <tbb\parallel_reduce.h>
#include <tbb\blocked_range.h>
#include <tbb\blocked_range2d.h>
#include <tbb\parallel_sort.h>

#include "stat.h"
#include "LinkedList.h"

class UniversalGraph
{
public:
    UniversalGraph(size_t num_vertex): numVertex(num_vertex), isDistanceMapCalculated(false), distanceMap(NULL), calc(NULL) {
		matSize = numVertex*(numVertex-1)/2;
		adjMat = new (std::nothrow) float[matSize];
        if (adjMat == NULL)
            std::cout << "Can't allocate memory for graph" << std::endl;
        std::fill(adjMat, adjMat + matSize, 0.0f);
    }

    UniversalGraph(std::string filename): matSize(0), adjMat(NULL) {
        readFromFile(filename);
    }

    virtual ~UniversalGraph() {
        delete[] adjMat;
	    if (distanceMap != NULL)
            delete[] distanceMap;

	    if (calc != NULL)
            delete calc;
    }

    void clear() {
        std::fill(adjMat, adjMat + matSize, 0.0f);
    }

    void addEdge(size_t i, size_t j, float val) {
        size_t ind = getIndex(i, j);
        adjMat[ind] = val;
    }

    float getWeight(size_t i, size_t j) {
        size_t ind = getIndex(i, j);
        return adjMat[ind];
    }

    bool getEdge(size_t i, size_t j) {
        size_t ind = getIndex(i, j);
        return adjMat[ind] > 0.f;
    }

    inline int getDegree(size_t v) {
        int degree = 0;
        for (size_t i=0; i<numVertex; i++) {
            if (i == v)
                continue;

            if (getEdge(v, i))
                degree++;
        }
        return degree;
    }

    void getEdgeWeightStatistic(float& m, float& v, float& max, float& min)
    {
        m = (float) mean(adjMat, matSize);
	    v = (float) var(adjMat, matSize);
	    max = *std::max_element(adjMat, adjMat+matSize);
	    min = *std::min_element(adjMat, adjMat+matSize);
    }

    void getDegreeStatistics(float& m, float& v, float& max, float& min, float& centrality)
    {
	    int* temp = new int[numVertex];
        tbb::parallel_for(tbb::blocked_range<size_t>(0, numVertex, 1000), 
            [=] (const tbb::blocked_range<size_t>& r) {
                for (size_t i=r.begin(); i<r.end(); i++)
		            temp[i] = getDegree(i);
            });

	    m = (float) mean(temp, numVertex);
	    v = (float) var(temp, numVertex);
	    max = (float) *std::max_element(temp,temp+numVertex);
	    min = (float) *std::min_element(temp,temp+numVertex);
        centrality = 0.f;
        for (size_t i=0; i<numVertex; i++) {
            centrality += max - temp[i];
        }
        centrality /= (numVertex-1)*(numVertex-1);

	    delete[] temp;
    }

    float getQuantile(double percent)
    {
	    size_t ind = (size_t) (matSize * percent);
	    return getQuantile(ind);
    }

    float getQuantile(size_t number)
    {
	    float* temp = new float[matSize];
	    std::copy(adjMat, adjMat + matSize, temp);
        tbb::parallel_sort(temp, temp + matSize);
	    float val = temp[matSize - number];
	    delete[] temp;
	    return val;
    }

    void applyThreshold(float threshold)
    {
        tbb::parallel_for(tbb::blocked_range<size_t>(0, matSize, 1000), 
        [=] (const tbb::blocked_range<size_t>& r) {
            for (size_t i=r.begin(); i<r.end(); i++)
		        if (adjMat[i] < threshold)
			        adjMat[i] = 0.0f;
        });
    }

    void getShortestPathsStatistics(float& m, int& min, int& max) {
        if (!isDistanceMapCalculated)
            fillDistanceMap();
        
        size_t sum = 0;
        min = std::numeric_limits<int>::max();
        max = std::numeric_limits<int>::min();
        size_t numDistPairs = 0;

        for (size_t i=0; i<numVertex; i++)
            for (size_t j=i+1; j<numVertex; j++) {
                if (distanceMap[i*numVertex + j] < std::numeric_limits<int>::max()) {
                    sum += distanceMap[i*numVertex + j];
                    numDistPairs++;
                    min = std::min(min, distanceMap[i*numVertex + j]);
                    max = std::max(max, distanceMap[i*numVertex + j]);
                }
            }

        m = (float) sum / numDistPairs;
    }

    void getBetweennessCentrality(float& maxCentrality, float& centralization) {
        if (!isDistanceMapCalculated)
            fillDistanceMap();

        float* centrality = calc->centrality;
        size_t numAllPaths = calc->numPaths;

        maxCentrality = *std::max_element(centrality, centrality+numVertex);
        
        float centralitySum = 0.f;
        for (size_t i=0; i<numVertex; i++) {
            centralitySum += maxCentrality - centrality[i];
        }
        maxCentrality /= numAllPaths;
        centralization = centralitySum / numAllPaths / (numVertex - 1);
    }

    void writeToFile(std::string filename) {
        std::ofstream out(filename, std::ios::binary);
        if (out.is_open()) {
            out.write((const char*) &numVertex, sizeof(size_t));

            for (size_t i=0; i<matSize; i++) {
                out.write((const char*) adjMat + i, sizeof(float));
            }
        } else {
            std::cout << "Incorrect path for saving graph: " << filename << std::endl;
        }

        out.flush();
        out.close();
    }

    void readFromFile(std::string filename) {
        std::ifstream in(filename, std::ios::binary);

        if (in.is_open()) {
            in.read((char*) &numVertex, sizeof(size_t));
            size_t newMatSize = numVertex*(numVertex-1)/2;
        
            if (newMatSize != matSize) {
                matSize = newMatSize;

                if (adjMat != NULL)
                    delete[] adjMat;

                adjMat = new (std::nothrow) float[matSize];
                if (adjMat == NULL)
                    std::cout << "Can't allocate memory for graph" << std::endl;
            }

            for (size_t i=0; i<matSize; i++) {
                in.read((char*) adjMat + i, sizeof(float));
            }
        } else {
            std::cout << "Incorrect graph file: " << filename << std::endl;
        }

        in.close();
    }

private:

    //size_t fillBetweennessCentralityArray(float* centrality, size_t start, size_t end) {
    //    std::stack<size_t> st;
    //    size_t numPaths = pathMap[start*numVertex + end].getHead()->data != end ? 1 : 0;
    //    st.push(start);

    //    while (!st.empty()) {
    //        size_t curVertex = st.top();
    //        st.pop();

    //        if (curVertex == end)
    //            continue;
    //        
    //        if (curVertex != start) {
    //            centrality[curVertex] += 1;
    //        }

    //        if (pathMap[curVertex*numVertex + end].size() > 1)
    //            numPaths += pathMap[curVertex*numVertex + end].size() - 1;

    //        for (auto head = pathMap[curVertex*numVertex + end].getHead(); ; head = head->next) {
    //            if (head == NULL)
    //                break;
    //            st.push(head->data);
    //        }
    //    }

    //    return numPaths;
    //}

    class FloydWarshall
    {
    private:
	    int* dist;
	    LinkedList* up;	
	    size_t n;		
	    size_t k;		
    public:
        FloydWarshall(LinkedList* _up, int*_dist, size_t _n, size_t _k): up(_up), dist(_dist), k(_k), n(_n) { }

	    void operator()(const tbb::blocked_range2d<size_t>& r) const { 
		    size_t l1 = r.rows().begin();
		    size_t r1 = r.rows().end();
		    size_t l2 = r.cols().begin();
		    size_t r2 = r.cols().end();

		    for(size_t i=l1; i < r1; i++) {
			    relax(k,i,n,l2,r2);
		    }
	    }
    private:
	    void relax(size_t k, size_t i, size_t n, size_t l, size_t r) const {
		    for(size_t j=l; j< r; j++) {
			    if (dist[i * n + j] - dist[k * n + j] > dist[i * n + k]) {
				    dist[i * n + j] = dist[i * n + k] + dist[k * n + j];
                    up[i * n + j].clear();
                    up[i * n + j].add(k);
			    } else if (dist[i * n + j] - dist[k * n + j] == dist[i * n + k]) {
                    up[i * n + j].add(k);
                }
		    }
	    }
    };

    class ShortestPathsCalculator {
    public:
        ShortestPathsCalculator(UniversalGraph& g_): g(g_), numPaths(0) {
            centrality = new float[g.numVertex];
            std::fill(centrality, centrality+g.numVertex, 0);
            paths = new LinkedList[g.numVertex];
        }

        ShortestPathsCalculator(ShortestPathsCalculator& calc, tbb::split): g(calc.g), numPaths(0) {
            centrality = new float[g.numVertex];
            std::fill(centrality, centrality+g.numVertex, 0);
            paths = new LinkedList[g.numVertex];
        }

        virtual ~ShortestPathsCalculator() {
            delete[] centrality;
            delete[] paths;
        }

        void operator()( const tbb::blocked_range<size_t>& r ) {
            for (size_t v=r.begin(); v<r.end(); v++) {
                Wave(v);
            }
        }

        void join( ShortestPathsCalculator& rhs ) {
            for (size_t i=0; i<g.numVertex; i++) {
                centrality[i] += rhs.centrality[i];
            }
            numPaths += rhs.numPaths;
        }

        UniversalGraph& g;
        float* centrality;
        size_t numPaths;

    private:
        LinkedList* paths;

        void Wave(size_t v) {
            std::queue<size_t> q;
            int* distanceMap = g.distanceMap + v*g.numVertex;
            q.push(v);
            distanceMap[v] = 0;

            while (!q.empty()) {
                size_t cur = q.front();
                q.pop();

                for (size_t i=0; i<g.numVertex; i++) {
                    if (i == cur)
                        continue;

                    if (g.getEdge(i, cur)) {
                        if (distanceMap[i] > distanceMap[cur] + 1) {
                            distanceMap[i] = distanceMap[cur] + 1;
                            paths[i].add(cur);
                            q.push(i);
                        } else if (distanceMap[i] == distanceMap[cur] + 1) {
                            paths[i].add(cur);
                        }
                    }
                }
            }

            for (size_t i=0; i<g.numVertex; i++) {
                if (i == v)
                    continue; 

                if (distanceMap[i] < std::numeric_limits<int>::max()) {
                    numPaths += followPath(v, i);
                }
            }

            for (size_t i=0; i<g.numVertex; i++) {
                paths[i].clear();
            }
        }

        size_t followPath(size_t start, size_t end) {
            std::stack<size_t> st;
            if (paths[end].getHead()->data == start)
                return 0;

            size_t numPaths = 1;
            st.push(end);

            while (!st.empty()) {
                size_t curVertex = st.top();
                st.pop();

                if (curVertex == start)
                    continue;

                if (curVertex != end)
                    centrality[curVertex] += 1;

                if (paths[curVertex].size() > 1)
                    numPaths += paths[curVertex].size() - 1;

                for (auto head = paths[curVertex].getHead(); ; head = head->next) {
                    if (head == NULL)
                        break;
                    st.push(head->data);
                }
            }
            return numPaths;
        }
    };

    void fillDistanceMap() {
        if (distanceMap == NULL) {
		    distanceMap = new (std::nothrow) int[numVertex*numVertex];
            if (distanceMap == NULL) {
                std::cout << "Can't allocate memory for distance map" << std::endl;
            }
        }
        std::fill(distanceMap, distanceMap+(numVertex*numVertex), std::numeric_limits<int>::max());

        if (calc != NULL) {
            delete calc;
        }
        calc = new ShortestPathsCalculator(*this);

        tbb::parallel_reduce(tbb::blocked_range<size_t>(0, numVertex, 1), *calc);

        isDistanceMapCalculated = true;
    }

    /*void fillDistanceMap()
    {
	    if (distanceMap == NULL) {
		    distanceMap = new (std::nothrow) int[numVertex*numVertex];
            if (distanceMap == NULL) {
                std::cout << "Can't allocate memory for distance map" << std::endl;
            }
        }

	    if (pathMap == NULL) {
            pathMap = new LinkedList[numVertex*numVertex];
            if (pathMap == NULL) {
                std::cout << "Can't allocate memory for path map" << std::endl;
            }
        }
	
	    for (size_t i=0; i<numVertex; i++) {
		    for (size_t j=0; j<numVertex; j++) {
			    if (i == j) {
				    distanceMap[i*numVertex + j] = std::numeric_limits<int>::max();
                } else {
                    if (getEdge(i, j)) {
                        distanceMap[i*numVertex + j] = 1;
                        pathMap[i*numVertex + j].add(j);
                    } else 
                        distanceMap[i*numVertex + j] = std::numeric_limits<int>::max();
                }
		    }
	    }

        std::cout << "\tInitialized distance map " << std::endl << "\tCalculating: ";

	    for(int k=0; k < numVertex; k++) {
		    tbb::parallel_for(tbb::blocked_range2d<size_t>(0, numVertex, 0, numVertex), FloydWarshall(pathMap, distanceMap, numVertex, k));
            if (k % 1000 == 0)
                std::cout << ".";
        }

        std::cout << " finished!" << std::endl;

	    isDistanceMapCalculated = true;
    }*/

    inline size_t getIndex(size_t s, size_t e)
	{
		if (s == e) 
			throw std::logic_error("Self-loop is not allowed");

		if ((s < 0 || s > numVertex-1) || (e < 0 || e > numVertex-1))
		{
			char error_msg[100];
			sprintf_s(error_msg, "Incorrect index - (%d, %d)", s, e);
			throw std::logic_error(error_msg);
		}

		if (s < e) std::swap(s,e);
		return s*(s-1)/2 + e;
	}

    size_t matSize;
    size_t numVertex;
    float* adjMat;

    int* distanceMap;
    ShortestPathsCalculator* calc;

    bool isDistanceMapCalculated;
}; 

#endif /* __UNIVERSAL_GRAPH__ */