#include "FloydWarshall.h"

#ifdef USE_OPENCL

void FloydWarshallGPU(float* distance_map, int* shortest_paths, int num_vertex)
{
    cl_context context = CreateContext(0);
    cl_device_id device;
    cl_command_queue queue = CreateCommandQueue(context, &device);
    cl_program prog = CreateProgram(context, device, "FloydWarshall.cl");
    cl_kernel kernel = CreateKernel(prog, "FloydWarshall");

    if (kernel == NULL)
    {
        std::cout << "Failed to build kernel for FloydWarshall. Run CPU parallel version." << std::endl;
        CleanUp(context, queue, device, prog, kernel);
        return;
    }

    cl_mem_flags mem_flag = GetOptimalMemoryFlags(device);

    cl_mem memObjects[2];
    cl_int errNum;
    memObjects[0] = clCreateBuffer(context, mem_flag, sizeof(float) * num_vertex * num_vertex, distance_map, &errNum);
    if (errNum != CL_SUCCESS)
        std::cout << "Failed to create OpenCL buffer for distance map" << std::endl;
    memObjects[1] = clCreateBuffer(context, mem_flag, sizeof(int) * num_vertex * num_vertex, shortest_paths, &errNum);
    if (errNum != CL_SUCCESS)
        std::cout << "Failed to create OpenCL buffer for shortest_paths" << std::endl;

    if (memObjects[0] == NULL || memObjects[1] == NULL)
        std::cout << "Failed to create memory objects" << std::endl;
    size_t global_size[2] = { num_vertex, num_vertex };
    int num_v = (int) num_vertex;
    for (int k=0; k<num_vertex; k++)
    {
        errNum = clSetKernelArg(kernel, 0, sizeof(cl_mem), &memObjects[0]);
        errNum |= clSetKernelArg(kernel, 1, sizeof(cl_mem), &memObjects[1]);
        errNum |= clSetKernelArg(kernel, 2, sizeof(int), &k);
        errNum |= clSetKernelArg(kernel, 3, sizeof(int), &num_v);
        
        if (errNum != CL_SUCCESS)
        {
            std::cout << "Failed setting kernel arguments." << std::endl;
            CleanUp(context, queue, device, prog, kernel);
            return;
        }

        
        errNum = clEnqueueNDRangeKernel(queue, kernel, 2, NULL, global_size, NULL, 0, NULL, NULL);
        if (errNum != CL_SUCCESS)
        {
            std::cout << "Failed to run kernel: " << k << std::endl;
            CleanUp(context, queue, device, prog, kernel);
            return;
        }
    }

    errNum = clEnqueueReadBuffer(queue, memObjects[0], CL_FALSE, 0, sizeof(float)*num_vertex * num_vertex, distance_map, 0, NULL, NULL);
    errNum |= clEnqueueReadBuffer(queue, memObjects[1], CL_TRUE, 0, sizeof(int)*num_vertex * num_vertex, shortest_paths, 0, NULL, NULL);
    if (errNum != CL_SUCCESS)
    {
        std::cout << "Failed to read buffers" << std::endl;
        CleanUp(context, queue, device, prog, kernel);
    }
}

#endif