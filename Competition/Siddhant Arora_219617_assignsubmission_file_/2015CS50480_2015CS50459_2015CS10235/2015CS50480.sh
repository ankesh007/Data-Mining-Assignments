#!/bin/bash
if [ "$2" = "-plot" ]; then
	if [ -e  output_result.txt ]
		then
		rm output_result.txt
	fi
	./fp "$1" 1 output1.txt >> output_result.txt
	./fp "$1" 5 output1.txt >> output_result.txt
	./fp "$1" 10 output1.txt >> output_result.txt
	./fp "$1" 25 output1.txt >> output_result.txt
	./fp "$1" 50 output1.txt >> output_result.txt
	./fp "$1" 90 output1.txt >> output_result.txt
	./apriori "$1" 1 output1.txt >> output_result2.txt
	./apriori "$1" 5 output1.txt >> output_result2.txt
	./apriori "$1" 10 output1.txt >> output_result2.txt
	./apriori "$1" 25 output1.txt >> output_result2.txt
	./apriori "$1" 50 output1.txt >> output_result2.txt
	./apriori "$1" 90 output1.txt >> output_result2.txt
	python plot.py
fi
if [ "$3" = "-apriori" ]; then
	./apriori "$1" "$2" "$4"
fi
if [ "$3" = "-fptree" ]; then
	./fp "$1" "$2" "$4"
fi
