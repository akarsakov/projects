#ifndef __OCL_UTIL__
#define __OCL_UTIL__

#include <CL\opencl.h>

cl_context CreateContext(int platrform_num = 0);
cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device);
cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName);
cl_kernel CreateKernel(cl_program prog, const char* kernelName);
cl_mem_flags GetOptimalMemoryFlags(cl_device_id device_id);
void CleanUp(cl_context context, cl_command_queue queue, cl_device_id device, cl_program prog, cl_kernel kernel);

#endif /*__OCL_UTIL__*/