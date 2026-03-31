#!/bin/bash

shopt -s extglob

case $1 in
+([0-9_]))
	N=$1
	;;
*)
	echo "usage ./run-tests.sh <N> [optional: -1c | -all]"
	exit 0
	;;
esac

extract_time() {
	local label=$1
	local output=$2
	echo "$output" | grep "^${label}:" | awk '{print $2}'
}

run_gpu() {
	local out
	out=$(./bin/vec-add-gpu <"tests/${N}_numbers.txt")
	local t
	t=$(extract_time "Total" "$out")
	echo "gpu,$t"
}

run_cpu() {
	local out
	out=$(./bin/vec-add-cpu <"tests/${N}_numbers.txt")
	local t
	t=$(extract_time "Compute" "$out")
	echo "cpu,$t"
}

run_gpu_1c() {
	local out
	out=$(./bin/vec-add-gpu-1c <"tests/${N}_numbers.txt")
	local t
	t=$(extract_time "Total" "$out")
	echo "gpu-1c,$t"
}

# compile silently
case $2 in
"-all")
	make -s gpu cpu gpu-1c
	echo "env,time_ms"
	run_gpu
	run_cpu
	run_gpu_1c
	;;
"-1c")
	make -s cpu gpu-1c
	echo "env,time_ms"
	run_cpu
	run_gpu_1c
	;;
"")
	make -s gpu cpu
	echo "env,time_ms"
	run_gpu
	run_cpu
	;;
*)
	echo "usage ./run-tests.sh <N> [optional: -1c | -all]"
	exit 0
	;;
esac
