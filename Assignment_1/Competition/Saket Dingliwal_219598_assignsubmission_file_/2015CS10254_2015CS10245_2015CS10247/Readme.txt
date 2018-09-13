Team Members-
Prakhar Ganesh 2015CS10245
Rahul Agarwal 2015CS10247

Files-
There are 2 cpp file containing the codes for apriori and fptree algorithm. 
Plot.py is for making graph of runtimes
In Compile.sh, commands for compiling the c++ codes is there
In 2015CS10254.sh there is bash sript to run different codes based on different values of arguments

Explanation for Part B
----------------------
The apriori algorithm takes a lot of time (~10s) for the support value of 1%. However the time taken reduced drastically as the value of support threshold increases. This is due the fact at 1% support, there are large number of candidate sets. We have to make a pass through the entire transaction database to compute the support for each the candidate set. As the transactions can't be loaded into memory, there is a huge time cost involved with the read from the hard disk. Therefore, more the number of scans of the transaction database, more is the time. When Support is 90 %, the apriori become faster as there is no overhead of construction of trees or any structure. 
However, the time taken by the fptree algorithm is much less. Even for support as low as 1%, the time is ~0.47s. This is again due to only 2 scans of the transaction dataset required in the fptree algorithm. Once the fptree is constructed it can be stored in memory and counting of sets can be done from this tree efficiently. Therefore it is much more scalable algorithm for smaller supports than apriori. Due to large construction overhead, and with almost no frequent items at support of 90%, the time taken by this at larger support value is higher and trend is irregular. All in all, Compact  representation of transactions is the main reason behind the speedup obtained.