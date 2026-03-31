
.PHONY=cpu gpu gpu-1c
gpu-1c: vec-add-cl-1-core.c
	clang -framework OpenCL -Wno-deprecated-declarations -o ./bin/vec-add-gpu-1c vec-add-cl-1-core.c
gpu: vec-add-cl.c
	clang -framework OpenCL -Wno-deprecated-declarations -o ./bin/vec-add-gpu vec-add-cl.c
cpu: vec-add.c
	gcc vec-add.c -o ./bin/vec-add-cpu
run-gpu-1c: gpu-1c
	./bin/vec-add-gpu-1c
run-gpu: gpu
	./bin/vec-add-gpu
run-cpu: cpu
	./bin/vec-add-cpu

clean:
	rm bin/*
