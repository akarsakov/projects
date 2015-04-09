#ifndef __UGRAPH__
#define __UGRAPH__

#include <stdexcept>
#include <queue>
#include <algorithm>
#include <limits>
#include <list>
#include "stat.h"
#include "FloydWarshall.h"
#include "Graph.h"

/*
 * Class for representing undirected graph without self-loops
 */
template<typename T>
class UGraph: public Graph<T>
{
private:
	T* adj_mat;
	long mat_size;
	
	bool need_calc_dist_map;
	T* distance_map;
	int* shortest_paths;
public:
	UGraph(long v): Graph(v)
	{
		mat_size = v*(v-1)/2;
		adj_mat = new T[mat_size];
        std::fill(adj_mat, adj_mat + mat_size, (T) 0.0);
		need_calc_dist_map = true;
		distance_map = NULL;
		shortest_paths = NULL;
	}

	virtual ~UGraph()
	{
		delete[] adj_mat;
		if (distance_map != NULL)
			delete[] distance_map;
		if (shortest_paths != NULL)
			delete[] shortest_paths;
	}

	virtual void add_edge(long s, long e, T value)
	{
		long long index = get_index(s, e);
		adj_mat[index] = value;
	}

	virtual T get_edge(long s, long e)
	{
		long long index = get_index(s, e);
		return adj_mat[index];
	}

    virtual void delete_edge(long s, long e)
    {
        long long index = get_index(s, e);
		adj_mat[index] = 0;
    }

    bool isNonZeroEdge(long s, long e);

    std::vector<long> getNeighbors(long vertex)
    {
        std::vector<long> neighbors;
		for (long i=0; i<num_vertex; i++)
			if (i!=vertex)
				if(isNonZeroEdge(i,vertex)!=0)
					neighbors.push_back(i);
		return neighbors;
    }

	// path statistics
    long diameter();
	T diameterW();
	T efficiency();
	void getShortestPathsStatistics(T& mean, T& variance);

	// degree statistics
	void getDegreeStatistics(T& mean, T& variance, T& max, T& min);
	long degree(long v);
	T avgDegree(long v);
	T getDegreeCentrality();
	void getBetweennessCentrality(T& cent, long& max_cent);
    void getAllVertexBetweenness(long* centrality);

	// edge weight statistics
	void getEdgeWeightStatistic(T& mean, T& variance, T& max, T& min);
	T getQuantile(double percent);
	T getQuantile(int number);
	void getWeightHistogram(int* hist, int num_banks);

	// helpful functions 
	void applyThreshold(double threshold);
	void invertWeights();
	void clearGraph();
	void writeToFile(std::string filename);
	virtual void copyTo(UGraph<T>& to);

private:

	inline long get_index(long s, long e)
	{
		if (s == e) 
			throw std::logic_error("Self-loop is not allowed");

		if ((s < 0 || s > num_vertex-1) || (e < 0 || e > num_vertex-1))
		{
			char error_msg[100];
			sprintf_s(error_msg, "Incorrect index - (%d, %d)", s, e);
			throw std::logic_error(error_msg);
		}

		if (s < e) std::swap(s,e);

		return s*(s-1)/2 + e;
	}

	void fill_distance_map();
	void go_shortest_path(long start, long end, long* vertexes);
};

template<typename T>
bool UGraph<T>::isNonZeroEdge(long s, long e)
{
    long index = get_index(s, e);
	return adj_mat[index] > std::numeric_limits<T>::min();
}

template<typename T>
void UGraph<T>::copyTo(UGraph<T>& to)
{
	if (to.mat_size != mat_size)
	{
		delete[] to.adj_mat;
		to.adj_mat = new T[mat_size];
	}
	std::copy(adj_mat, adj_mat + mat_size, to.adj_mat);
}

template<typename T>
void UGraph<T>::getEdgeWeightStatistic(T& m, T& v, T& max, T& min)
{
	m = (T) mean(adj_mat, mat_size);
	v = (T) var(adj_mat, mat_size);
	max = *std::max_element(adj_mat,adj_mat+mat_size);
	min = *std::min_element(adj_mat,adj_mat+mat_size);
}

template<typename T>
void UGraph<T>::getWeightHistogram(int* hist, int num_banks)
{
	std::fill(hist, hist+num_banks, 0);
	T max = *std::max_element(adj_mat,adj_mat+mat_size);
	T min = *std::min_element(adj_mat,adj_mat+mat_size);

	for (int i=0; i<mat_size; i++)
	{
		int bank = (adj_mat[i]-min)/(max-min)*(num_banks-1);
		hist[bank] += 1;
	}
}

template<typename T>
void UGraph<T>::getDegreeStatistics(T& m, T& v, T& max, T& min)
{
	T* temp = new T[num_vertex];
	for (long i=0; i<num_vertex; i++)
		temp[i] = avgDegree(i);
	m = (T) mean(temp, num_vertex);
	v = (T) var(temp, num_vertex);
	max = *std::max_element(temp,temp+num_vertex);
	min = *std::min_element(temp,temp+num_vertex);
	delete[] temp;
}

template<typename T>
T UGraph<T>::getQuantile(double percent)
{
	long ind = (long) (mat_size * percent);
	return getQuantile((int) index);
}

template<typename T>
T UGraph<T>::getQuantile(int number)
{
	T* temp = new T[mat_size];
	std::copy(adj_mat, adj_mat + mat_size, temp);
	std::sort(temp, temp + mat_size);
	T val = temp[mat_size - number];
	delete[] temp;
	return val;
}

template<typename T>
void UGraph<T>::invertWeights()
{
	float norm_coef = (float) *std::max_element(adj_mat,adj_mat+mat_size) +	(float) *std::min_element(adj_mat,adj_mat+mat_size);
	for (long i=0; i<mat_size; i++)
		adj_mat[i] = norm_coef - adj_mat[i];
}

template<typename T>
void UGraph<T>::applyThreshold(double threshold)
{
	for (long i=0; i<mat_size; i++)
		if (adj_mat[i] < threshold)
			adj_mat[i] = 0.0;
}

template<typename T>
long UGraph<T>::degree(long vertex)
{
	long count=0;
	for (long i=0; i<num_vertex; i++)
		if (i!=vertex)
			if (isNonZeroEdge(i,vertex)!=0) count++;
	return count;
}

template<typename T>
T UGraph<T>::avgDegree(long vertex)
{
	T degree = 0.0;
	for (long i=0; i<num_vertex; i++)
	{
		if (i == vertex)
			continue;
		degree += get_edge(i, vertex);
	}
	return degree / (num_vertex - 1);
}

template<typename T>
void UGraph<T>::clearGraph()
{
	std::fill(adj_mat, adj_mat + mat_size, (T) 0.0);
	need_calc_dist_map = true;
}

template<typename T>
void UGraph<T>::writeToFile(std::string filename)
{
	ofstream file(filename, std::ifstream::out);
	if (!file.is_open())
	{
		char error_msg[100];
		sprintf_s(error_msg, "Cannot open file - %s!", filename.c_str());
		throw std::logic_error(error_msg);
	}
	for (int i=0; i<num_vertex; i++)
	{
		for (int j=i+1; j<num_vertex; j++)
		{
			if (isNonZeroEdge(i,j))
			{
				file << i << ", " << j << endl;
			}
		}
	}
	file.close();
}

template<typename T>
void UGraph<T>::fill_distance_map()
{
	if (distance_map == NULL)
		distance_map = new T[num_vertex*num_vertex];

	if (shortest_paths == NULL)
		shortest_paths = new int[num_vertex*num_vertex];
	
	for (long i=0; i<num_vertex; i++)
	{
		for (long j=0; j<num_vertex; j++)
		{
			if (i == j)
				distance_map[i*num_vertex + j] = std::numeric_limits<T>::max();
			else
                distance_map[i*num_vertex + j] = get_edge(i,j) == 0.0 ? std::numeric_limits<T>::max() : get_edge(i,j);

			shortest_paths[i*num_vertex + j] = j;
		}
	}

	FloydWarshallParallel<T>(distance_map, shortest_paths, num_vertex);

	need_calc_dist_map = false;
}

template<typename T>
T UGraph<T>::diameterW()
{
	if (need_calc_dist_map)
		fill_distance_map();

	T diam = 0.0;
	for (int i=0; i<num_vertex; i++)
		for (int j=i+1; j<num_vertex; j++)
			diam = max(diam, distance_map[i*num_vertex + j]);

	return diam;
}

template<typename T>
T UGraph<T>::efficiency()
{
	if (need_calc_dist_map)
		fill_distance_map();

	T eff = 0.0;
	for (int i=0; i<num_vertex; i++)
		for (int j=i+1; j<num_vertex; j++)
			eff += 1 / (1 + distance_map[i*num_vertex + j]);

	return eff/(num_vertex*(num_vertex-1));
}

template<typename T>
void UGraph<T>::go_shortest_path(long st, long en, long* vert)
{
	long cur = shortest_paths[en*num_vertex + st];
	while (cur != en)
	{
		vert[cur] += 1;
		cur = shortest_paths[en*num_vertex + cur];
	}
}

template<typename T>
void UGraph<T>::getShortestPathsStatistics(T& m, T& v)
{
	if (need_calc_dist_map)
		fill_distance_map();
	double sum = 0.0;
	for (int i=0; i<num_vertex; i++)
		for (int j=i+1; j<num_vertex; j++)
			sum += distance_map[i*num_vertex + j];
	m = (T) (sum / mat_size);
}

template<typename T>
void UGraph<T>::getAllVertexBetweenness(long* centrality)
{
    if (need_calc_dist_map)
		fill_distance_map();
    
    for (int i=0; i<num_vertex; i++)
		for (int j=i+1; j<num_vertex; j++)
			go_shortest_path(i, j, centrality);
}

template<typename T>
void UGraph<T>::getBetweennessCentrality(T& cent, long& max_cent)
{
	long* centrality = new long[num_vertex];
	std::fill(centrality, centrality+num_vertex, 0);

    getAllVertexBetweenness(centrality);

	max_cent = *std::max_element(centrality,centrality+num_vertex);
	cent = 0.0;
	for (long i=0; i<num_vertex; i++)
		cent += abs(centrality[i] - max_cent);
	cent /= (num_vertex*(num_vertex - 1));
	delete[] centrality;
}

template<typename T>
T UGraph<T>::getDegreeCentrality()
{
	T cent = 0.0;
	long* temp = new long[num_vertex];
	for (long i=0; i<num_vertex; i++)
		temp[i] = degree(i);

	long max_degree = *std::max_element(temp, temp + num_vertex);
	for (long i=0; i<num_vertex; i++)
		cent += abs(max_degree - temp[i]);

	delete[] temp;
	return cent / ((num_vertex-1)*(num_vertex-2));
}

#endif