

__kernel void FloydWarshall(__global float* distance_map, __global int* shortest_paths, int k, int n)
{
    int i = get_global_id(0);
    int j = get_global_id(1);

    if (i < n && j < n)
    {
        if(distance_map[i*n + j] - distance_map[k*n + j] > distance_map[i*n + k]) 
		{
			distance_map[i*n + j] = distance_map[i*n + k] + distance_map[k*n + j];
			shortest_paths[i*n + j] = shortest_paths[k*n + j];
		}
    }
}