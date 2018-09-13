#!/bin/bash

ap="-apriori"
fp="-fptree"
pl="-plot"

if [ "$2" == "$pl" ]
then
    if [ -f logs.txt ]
    then
        rm logs.txt
    fi
    for x in {1,5,10,25,50,90}
    do
        echo $x
        echo -n "$(./apriori $1 $x temp 1)" >> logs.txt 
        echo -n " " >> logs.txt
        echo -n "$(./fptree $1 $x temp 1)" >> logs.txt
        echo "" >> logs.txt 
    done
    python plot.py
    rm temp.txt
else
    if [ "$3" == "$ap" ]
    then
        ./apriori $1 $2 $4 0
    elif [ "$3" == "$fp" ]
    then
        ./fptree $1 $2 $4 0
    fi
fi