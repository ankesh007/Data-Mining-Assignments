#!/bin/bash
if [ "$#" -eq 3 ] && [ $1 = "-kmeans" ]
then
	make KMEANS
	./KMEANS $2 $3
elif [ "$#" -eq 4 ] && [ $1 = "-dbscan" ]
then
    make DBSCAN
    ./DBSCAN $2 $3 $4
elif [ "$#" -eq 4 ] && [ $1 = "-optics" ]
then
    make OPTICS
    ./OPTICS $2 $3 $4
else
	echo "wrong format"
fi