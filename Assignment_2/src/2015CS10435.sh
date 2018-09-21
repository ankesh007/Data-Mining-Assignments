#!/bin/bash
if [ "$#" -eq 3 ] && [ $1 = "-kmeans" ]
then
	make KMEANS
	./KMEANS $3 $2
elif [ "$#" -eq 4 ] && [ $1 = "-dbscan" ]
then
    make DBSCAN
    ./DBSCAN $2 $3 $4
elif [ "$#" -eq 4 ] && [ $1 = "-optics" ]
then
    make OPTICS
    ./OPTICS $4 $3 $2
else
	echo "wrong format"
fi