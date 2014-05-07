#include <CL\opencl.h>

cl_context CreateContext()
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

	// Choose first platform
	cl_context_properties contextProperties[] = { CL_CONTEXT_PLATFORM, (cl_context_properties)platformIds[0], 0 };
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

cl_kernel CreateKernel(cl_program prog, const char kernelName[])
{
	cl_kernel kernel = clCreateKernel(prog, kernelName, NULL);
	if (kernel == NULL)
	{
		cout << "Failed to create kernel" << endl;
		Cleanup(context, commandQueue, program, kernel, memObjects);
		return 1;
	}
 // Create memory objects that will be used as arguments to
 // kernel. First create host memory arrays that will be
 // used to store the arguments to the kernel
 float result[ARRAY_SIZE];
 float a[ARRAY_SIZE];
 float b[ARRAY_SIZE];
 for (int i = 0; i < ARRAY_SIZE; i++)
 {
 a[i] = (float)i;
 b[i] = (float)(i * 2);
 }
 if (!CreateMemObjects(context, memObjects, a, b))
 {
 Cleanup(context, commandQueue, program, kernel, memObjects);
 return 1;
 }
}