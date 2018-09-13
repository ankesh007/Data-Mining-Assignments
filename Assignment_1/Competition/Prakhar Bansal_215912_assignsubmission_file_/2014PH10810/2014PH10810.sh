#!/bin/bash
# Done by 2014PH10810(Prakhar Bansal) alone
x="valid"
if [[ "$3" == "-apriori" ]]
then
python3 Apriori.py "$1" "$2" "$3" "$4"
elif [[ "$3" == "-fptree" ]]
then
    python3 fpt.py "$1" "$2" "$3" "$4"
elif [[ "$2" == "-plot" ]]
then
    python3 plot.py $1

fi

