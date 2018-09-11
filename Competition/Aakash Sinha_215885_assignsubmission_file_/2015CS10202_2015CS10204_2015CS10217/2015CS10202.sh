#!/bin/bash
if [ $2 == "-plot" ]
then
	./compile.sh
	python plot.py $1
else
	./out $1 $2 $3 $4".txt"
fi
