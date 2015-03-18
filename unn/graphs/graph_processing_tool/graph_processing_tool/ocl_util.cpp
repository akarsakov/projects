#ifdef USE_OPENCL

#pragma comment(lib, "OpenCL.lib") 

#include <iostream>
#include <fstream>
#include <sstream>
#include "ocl_util.h"

using namespace std;

cl_context CreateContext(int platrform_num)
{
	cl_int errNum;
	cl_uint numPlatforms;
	cl_platform_id platformIds[5];
	cl_context context = NULL;

	errNum = clGetPlatformIDs(5, platformIds, &numPlatforms);

	if (errNum != CL_SUCCESS || numPlatforms <= 0)
	{
		 cout << "Failed to find any OpenCL platforms." << endl;
		 return NULL;
	}

	for (cl_uint i=0; i<numPlatforms; i++)
	{
		char platform_name[80], platform_vendor[80];
		size_t size;
		errNum = clGetPlatformInfo(	platformIds[i], CL_PLATFORM_NAME, sizeof(platform_name), platform_name, &size);
		errNum = clGetPlatformInfo(	platformIds[i], CL_PLATFORM_VENDOR, sizeof(platform_vendor), platform_vendor, &size);
		cout << "Platform #" << i << ":" << endl;
		cout << "Vendor: " << platform_vendor << endl;
		cout << "Platform name: " << platform_name << endl;
	}

	cl_context_properties contextProperties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platformIds[platrform_num], 0 };
	context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_GPU, NULL, NULL, &errNum);

	if (errNum != CL_SUCCESS)
	{
		cout << "Could not create GPU context, trying CPU..." << endl;
		context = clCreateContextFromType(contextProperties, CL_DEVICE_TYPE_CPU, NULL, NULL, &errNum);
		if (errNum != CL_SUCCESS)
		{
			cout << "Failed to create an OpenCL GPU or CPU context.";
			return NULL;
		}
	}

	return context;
}

cl_command_queue CreateCommandQueue(cl_context context, cl_device_id *device)
{
	cl_int errNum;
	cl_device_id *devices;
	cl_command_queue commandQueue = NULL;
	size_t deviceBufferSize = -1;
	
	// First get the size of the devices buffer
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, 0, NULL, &deviceBufferSize);
	if (errNum != CL_SUCCESS)
	{
		cout << "Failed call to clGetContextInfo(...,GL_CONTEXT_DEVICES,...)";
		return NULL;
	}
 
	if (deviceBufferSize <= 0)
	{
		cout << "No devices available.";
		return NULL;
	}

	// Allocate memory for the devices buffer
	int num_devices = (int) deviceBufferSize / sizeof(cl_device_id);
	devices = new cl_device_id[num_devices];
	errNum = clGetContextInfo(context, CL_CONTEXT_DEVICES, deviceBufferSize, devices, NULL);
	if (errNum != CL_SUCCESS)
	{
		cout << "Failed to get device IDs";
		return NULL;
	}

	for (int i=0; i<num_devices; i++)
	{
		char device_name[80];
		size_t size;
		errNum = clGetDeviceInfo(devices[i], CL_DEVICE_NAME, sizeof(device_name), device_name, &size);
		cout << "Device #" << i << ":" << endl;
		cout << "\tDevice name: " << device_name << endl;
	}

	// Choose first device
	commandQueue = clCreateCommandQueue(context, devices[0], 0, NULL);
	if (commandQueue == NULL)
	{
		cout << "Failed to create commandQueue for device 0";
		return NULL;
	}

	*device = devices[0];
	delete [] devices;
	return commandQueue;
}

cl_program CreateProgram(cl_context context, cl_device_id device, const char* fileName)
{
	cl_int errNum;
	cl_program program;
	ifstream kernelFile(fileName, ios::in);
	if (!kernelFile.is_open())
	{
		cout << "Failed to open file for reading: " << fileName << endl;
		return NULL;
	}
	ostringstream oss;
	oss << kernelFile.rdbuf();
	string srcStdStr = oss.str(); 
	const char *srcStr = srcStdStr.c_str();
	program = clCreateProgramWithSource(context, 1, (const char**)&srcStr, NULL, NULL);
	if (program == NULL)
	{
		cout << "Failed to create CL program from source: " << fileName << endl;
		return NULL;
	}

	errNum = clBuildProgram(program, 0, NULL, NULL, NULL, NULL);
	if (errNum != CL_SUCCESS)
	{
		// Determine the reason for the error
		char buildLog[16384];
		clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(buildLog), buildLog, NULL);
		cout << "Error in kernel: " << endl;
		cout << buildLog;
		clReleaseProgram(program);
		return NULL;
	}
	return program;
}

cl_kernel CreateKernel(cl_program prog, const char* kernelName)
{
	cl_kernel kernel = clCreateKernel(prog, kernelName, NULL);
    return kernel;
}

cl_mem_flags GetOptimalMemoryFlags(cl_device_id device_id)
{
    cl_int errNum;
    cl_bool isUnified = CL_FALSE;
    size_t sz = 0;
    cl_mem_flags flag = CL_MEM_COPY_HOST_PTR;
    errNum = clGetDeviceInfo(device_id, CL_DEVICE_HOST_UNIFIED_MEMORY, sizeof(isUnified), &isUnified, &sz);
    if (errNum == CL_SUCCESS && isUnified)
        flag = CL_MEM_USE_HOST_PTR;
    return flag;
}

void CleanUp(cl_context context, cl_command_queue queue, cl_device_id device, cl_program prog, cl_kernel kernel)
{
    if (kernel != NULL)
        clReleaseKernel(kernel);
    if (prog != NULL)
        clReleaseProgram(prog);
    if (device != NULL)
        clReleaseDevice(device);
    if (queue != NULL)
        clReleaseCommandQueue(queue);
    if (context != NULL)
        clReleaseContext(context);
}

#endif