#!/bin/bash
if [ "$#" -eq 2 ] && [ $2 = "-plot" ]
then
	echo "plot selected"
	python gen_graph.py $1 
elif [ "$#" -eq 4 ] && [ $3 = "-apriori" ]
then
    echo "apriori selected"
    ./apriori $1 $4 $2 0 0
elif [ "$#" -eq 4 ] && [ $3 = "-fptree" ]
then
    echo "fptree selected"
    ./FP_Tree $1 $2 $4 0 0
else
	echo "wrong format"
fi
