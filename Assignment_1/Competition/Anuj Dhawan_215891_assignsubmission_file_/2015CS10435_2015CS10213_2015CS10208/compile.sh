#!/bin/bash
make FP_Tree
g++ -o apriori -std=c++0x apriori.cpp -O3