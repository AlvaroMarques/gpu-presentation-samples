#include <stdio.h>
#include <stdlib.h>

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


	for (i = 0; i < n; i++) {
		output[i] = a[i] + b[i];
	}

	for (i = 0; i < n; i++) {
		printf("%.2f\n", output[i]);
	}

	return 0;

}
