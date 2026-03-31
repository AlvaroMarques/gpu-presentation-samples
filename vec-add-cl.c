#include <stdio.h>
#include <stdlib.h>
#include <OpenCL/opencl.h>

const char *kernel_source =
    "__kernel void vec_add(__global const float *a,\n"
    "                      __global const float *b,\n"
    "                      __global float *c,\n"
    "                      const unsigned int n) {\n"
    "    int i = get_global_id(0);\n"
    "    if (i < n) c[i] = a[i] + b[i];\n"
    "}\n";

void check_err(cl_int err, const char *msg) {
    if (err != CL_SUCCESS) {
        fprintf(stderr, "OpenCL error (%d): %s\n", err, msg);
        exit(1);
    }
}

int main() {
	int N;
	scanf("%d",  &N);

	const int bytes = N * sizeof(float);
	float *a = (float*) malloc(bytes);
	float *b = (float*) malloc(bytes);
	float *c = (float*) malloc(bytes);

	int i;
	for (i = 0; i < N; i++) {
		scanf("%f", a + i);
	}

	for (i = 0; i < N; i++) {
		scanf("%f", b + i);
	}



    cl_int err;

    // Get GPU device
    cl_device_id device;
    err = clGetDeviceIDs(NULL, CL_DEVICE_TYPE_GPU, 1, &device, NULL);
    check_err(err, "clGetDeviceIDs");

    // Print device name
    char name[256];
    clGetDeviceInfo(device, CL_DEVICE_NAME, sizeof(name), name, NULL);
    printf("Device: %s\n", name);

    // Create context and command queue
    cl_context ctx = clCreateContext(NULL, 1, &device, NULL, NULL, &err);
    check_err(err, "clCreateContext");

    cl_command_queue queue = clCreateCommandQueue(ctx, device, 0, &err);
    check_err(err, "clCreateCommandQueue");

    // Create buffers
    cl_mem buf_a = clCreateBuffer(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, bytes, a, &err);
    check_err(err, "clCreateBuffer a");
    cl_mem buf_b = clCreateBuffer(ctx, CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, bytes, b, &err);
    check_err(err, "clCreateBuffer b");
    cl_mem buf_c = clCreateBuffer(ctx, CL_MEM_WRITE_ONLY, bytes, NULL, &err);
    check_err(err, "clCreateBuffer c");

    // Build kernel
    cl_program program = clCreateProgramWithSource(ctx, 1, &kernel_source, NULL, &err);
    check_err(err, "clCreateProgramWithSource");

    err = clBuildProgram(program, 1, &device, NULL, NULL, NULL);
    if (err != CL_SUCCESS) {
        char log[4096];
        clGetProgramBuildInfo(program, device, CL_PROGRAM_BUILD_LOG, sizeof(log), log, NULL);
        fprintf(stderr, "Build error:\n%s\n", log);
        exit(1);
    }

    cl_kernel kernel = clCreateKernel(program, "vec_add", &err);
    check_err(err, "clCreateKernel");

    // Set args
    cl_uint n = N;
    clSetKernelArg(kernel, 0, sizeof(cl_mem), &buf_a);
    clSetKernelArg(kernel, 1, sizeof(cl_mem), &buf_b);
    clSetKernelArg(kernel, 2, sizeof(cl_mem), &buf_c);
    clSetKernelArg(kernel, 3, sizeof(cl_uint), &n);

    // Execute
    size_t global_size = N;
    err = clEnqueueNDRangeKernel(queue, kernel, 1, NULL, &global_size, NULL, 0, NULL, NULL);
    check_err(err, "clEnqueueNDRangeKernel");

    // Read result
    err = clEnqueueReadBuffer(queue, buf_c, CL_TRUE, 0, bytes, c, 0, NULL, NULL);
    check_err(err, "clEnqueueReadBuffer");

    for (int i = 0; i < N; i++) {
		printf("%.4f\n", c[i]);
    }

    // Cleanup
    clReleaseMemObject(buf_a);
    clReleaseMemObject(buf_b);
    clReleaseMemObject(buf_c);
    clReleaseKernel(kernel);
    clReleaseProgram(program);
    clReleaseCommandQueue(queue);
    clReleaseContext(ctx);
    free(a); free(b); free(c);

    return 0;
}
