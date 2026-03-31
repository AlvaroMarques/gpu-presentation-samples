#include <stdio.h>
#include <stdlib.h>
#include <mach/mach_time.h>

int main() {
	int n;
	scanf("%d",  &n);

	float *a = (float*) malloc(n * sizeof(float));
	float *b = (float*) malloc(n * sizeof(float));
	float *output = (float*) malloc(n * sizeof(float));

	int i;
	for (i = 0; i < n; i++) {
		scanf("%f", a + i);
	}

	for (i = 0; i < n; i++) {
		scanf("%f", b + i);
	}


	mach_timebase_info_data_t info;
	mach_timebase_info(&info);

	uint64_t t_start = mach_absolute_time();
	for (i = 0; i < n; i++) {
		output[i] = a[i] + b[i];
	}
	uint64_t t_end = mach_absolute_time();

	double elapsed_ns = (double)(t_end - t_start) * info.numer / info.denom;
	printf("Compute: %.4f ms\n", elapsed_ns / 1e6);

	return 0;

}
