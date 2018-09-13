#!/bin/bash

if [ "$3" == "-apriori" ]
then
    ./apriori $2 $1 $4".txt"
elif [[ "$3" == "-fptree" ]]
then
    ./fptree $1 $2 >$4".txt"
elif [[ "$2" == "-plot" ]]
then
    ./apriori 101 $1 garbage.txt
    ./fptree $1 101 > fptree_runtime.txt
    rm garbage.txt
    python plot.py
else
    echo "INCORRECT COMMAND LINE ARGUMENTS"
fi
