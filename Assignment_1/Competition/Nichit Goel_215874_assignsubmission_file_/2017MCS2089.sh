#!/bin/bash

if [ "$2" = "-plot" ]; then
	echo "Working hard now..."
	tf="time.csv"
	echo 1,5,10,25,50,90 > $tf
	arr=("1" "5" "10" "25" "50" "90")
	apr=""
	fpt=""
	for t in "${arr[@]}"
	do
		
		val="$( TIMEFORMAT='%R'; time ( ./apriori $t $1 out.txt ) 2>&1 1>/dev/null )"
		apr="$apr$val,"
		
		val2="$( TIMEFORMAT='%R'; time ( ./fptree $t $1 out.txt ) 2>&1 1>/dev/null )"
		fpt="$fpt$val2,"
	done
	apr=${apr%?}
	echo "$apr" >> $tf
	fpt=${fpt%?}
	echo "$fpt" >> $tf
	echo python plot.py
	eval python plot.py
elif [ "$3" = "-apriori" ]; then
	out="$4.txt"
	echo ./apriori $2 $1 $out
	eval ./apriori $2 $1 $out
elif [ "$3" = "-fptree" ]; then
	out="$4.txt"
	echo ./fptree $2 $1 $out
	eval ./fptree $2 $1 $out
fi
