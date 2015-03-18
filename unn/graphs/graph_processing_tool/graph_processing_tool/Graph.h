#ifndef __GRAPH__
#define __GRAPH__

/*
 * Base class for graph representation
 */
template <typename T>
class Graph
{
protected:
	long num_vertex;
	long num_edges;
public:
	Graph(long v): num_vertex(v) { num_edges = 0; }

	virtual ~Graph() { }

	inline virtual int V() { return num_vertex; }
	inline virtual long E() { return num_edges; }

	virtual void add_edge(long s, long e, T value) = 0;
	virtual T get_edge(long s, long e) = 0;

    virtual void delete_edge(long s, long e) = 0;
};

#endif /*__GRAPH__*/