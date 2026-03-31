#!/bin/bash
N=$1

uv run main.py $N >"tests/${N}_numbers.txt"
