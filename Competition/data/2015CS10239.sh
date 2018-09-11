#!/bin/bash

if [ $# -eq 2 ]
then
	java -cp ./build Main $1 $2
	python ./src/plot.py times.txt
else
	java -cp ./build Main $1 $2 $3 $4
fi
