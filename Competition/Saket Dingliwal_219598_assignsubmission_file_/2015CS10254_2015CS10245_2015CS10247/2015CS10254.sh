#!/bin/bash

# g++ -O3 apriori.cpp -o apriori
# g++ -O3 fptree.cpp -o fptree

if [ "$#" -eq 2 ]; then
	python plot.py $1
else
	if [ "$3" != "-fptree" ]; then
		
	    ./apriori $1 $2 $4
	else
		
	    ./fptree $1 $2 $4
	fi
fi