#!/bin/sh

if [ "$2" = "-plot" ]
then
  python plot.py $1
elif [ "$3" = "-fptree" ]
then
  ./fp.out $1 $2 $4
elif [ "$3" = "-apriori" ]
then 
  ./apriori.out $1 $2 $4
fi

