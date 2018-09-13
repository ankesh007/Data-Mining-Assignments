#!/bin/bash
if [ "$#" -eq 2 ] && [ $2 = "-plot" ]
then
	echo "plot selected"
	cd src_plot
	make FP_Tree
	g++ -o apriori -std=c++0x apriori.cpp -O3
	python gen_graph.py $1 
elif [ "$#" -eq 4 ] && [ $3 = "-apriori" ]
then
    echo "apriori selected"
elif [ "$#" -eq 4 ] && [ $3 = "-fptree" ]
then
    echo "fptree selected"
else
	echo "wrong format"
fi
