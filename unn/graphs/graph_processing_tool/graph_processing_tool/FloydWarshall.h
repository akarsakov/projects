#ifndef __FLOYDWARSHAL__
#define __FLOYDWARSHAL__

#include "Graph.h"
#include <iostream>
#include <tbb\parallel_for.h>
#include <tbb\blocked_range2d.h>

#ifdef USE_OPENCL
#include "ocl_util.h"

void FloydWarshallGPU(float* distance_map, int* shortest_paths, int num_vertex);
#endif

template<typename T>
class FloydWarshall
{
private:
	T *dist;		// матрица расстояний
	int *up;		// pi - функция
	int n;			// размер матрицы расстояний
	int k;			// текущая транзитная вершина

public:
	FloydWarshall(int *_up, T*_dist, int _n, int _k): up(_up), dist(_dist), k(_k), n(_n) { }

	void operator()(const tbb::blocked_range2d<int>& r) const
	{ 
		int l1 = r.rows().begin();
		int r1 = r.rows().end();

		int l2 = r.cols().begin();
		int r2 = r.cols().end();

		for(int i=l1; i < r1; i++) 
		{
			relax(k,i,n,l2,r2);
		}
	}

private:
	void relax(int k, int i, int n, int l, int r) const
	{
		for(int j=l; j< r; j++) 
		{
			if( dist[i * n + j] - dist[k * n + j] > dist[i * n + k]) 
			{
				dist[i * n + j]   = dist[i * n + k] + dist[k * n + j];
				up  [i * n + j]   = k;
			}
		}
	}
};

template<typename T>
void FloydWarshallParallel(T* distance_map, int* shortest_paths, long num_vertex)
{
    
	for(int k=0; k < num_vertex; k++) {
        FloydWarshall<T> o(shortest_paths, distance_map, num_vertex, k);
		o(tbb::blocked_range2d<int>(0, num_vertex, 0, num_vertex));
    }
}

#endif /*__FLOYDWARSHAL__*/