This is the README document for the Data Mining - Assignment 1 [FREQUENT ITEMSET MINING].
Submitted by: KARTIKEYA SHARMA(2015CS10234), MEHAK PREET DHALIWAL(2015CS10238), ANKUR SHARMA (2015CS50278)

Implementation:
1. Entire assignment has been implemented in C++, with extensive usage of STLs. 
2. Hashmaps and other relevant data structures like vector, pair, etc. have been used to optimise the running time.

Assumptions:
1. Requisite Libraries: matplotlib, subprocess, Python 3 environment, UNIX/LINUX shell environment(bash), STLs in C++, g++ compiler, etc.
2. Ordering of the frequent patterns is dependent on the algorithm used for mining the dataset.

Apriori Algorithm:
Relevant files : 'main.cpp', 'apriori.cpp'
'main.cpp' -> main.cpp just calls the apriori function from apriori.cpp to execute the apriori algorithm
'apriori.cpp' -> This file has the implementation of the apriori algorithm to extract frequent itemsets. Initially it scans the database to generate the first candidate set, using a map from the C++ STL to count each item. Then it creates the first frequent itemset from this . The function candidate-gen is responsible to create the candidates from the previous frequent itemset, by joining frequent sets and pruning subsets. The counts of the candidate are then updated by iterating over the transaction dataset once, followed by generation of the next frequent set. This is repeated until no further frequent itemset can be generated.

FP Tree Algorithm:
Relevant Files: 'fptree.cpp'
'fptree.cpp' -> The code takes as arguments the input file, minimum support threshold and the output file name and runs the FP-Tree algorithm on the transaction database in the input file. Frequency of each itemset is calculated in the first input file scan and an flist is created based on which the FP-Tree and header table are created after a second input scan. Then, recursively traversing over the header table in decreasing order of the flist, the tree is updated and the element is added to the frequent itemset if it’s support crosses the threshold.

Other files:
compile.sh : This file is used for compiling the code files, and generating the executable which can be ran directly. O3 flag has been used for compiler level optimizations.
To compile the code for Apriori Algorithm, 
    -> g++ main.cpp apriori.cpp -O3 -o <executable-name> -std=c++11
To compile the code for FP-Tree Algorithm,
    -> g++ fptree.cpp -O3 -o <executable-name> -std=c++11
Both of the executable files are generated after compilation, and can be used by the <Entry-Number>.sh file to run the mining algorithm directly.

<Entry-Number>.sh : This is the main file used for executing the mining algorithm.
Executing the command: sh <Entry-Number>.sh <inputfile> X -apriori <filename> should generate a file <filename>.txt containing the frequent itemsets at >= X% support threshold with the Apriori algorithm.
Similarly, executing the command: sh <Entry-Number>.sh <inputfile> X -fptree <filename> should generate a file <filename>.txt containing the frequent itemsets using FP-tree algorithm.

Plotting files:
1. Python file 'plot.py' has been used for plotting the execution time graphs between the two algorithms. 
2. Plotting has been achieved using matplotlib, after saving the data for different threshold processes by forking a new subprocess for each algorithm.
3. Observations for the plot:
Both the curves are decreasing in nature with FP-Tree showing much better execution times than the Apriori Algorithm.

Explanation:
For Apriori, with increase in the minimum support threshold, fewer candidate sets are generating resulting in fewer database scans, thus decreasing the runtime.
For the FP-Tree algorithm, with increase in the minimum support threshold, fewer items cross the threshold resulting in smaller trees which are faster to scan.
FP-Tree performs better than Apriori since we do not need to generate candidate sets for each different sized itemset. This eliminates the multiple database scans done in Apriori for generating the item sets. Instead, only 2 database scans are done for creating the FP-tree.
