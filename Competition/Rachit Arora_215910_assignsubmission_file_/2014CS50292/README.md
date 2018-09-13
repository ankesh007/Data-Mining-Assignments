# freq-itemset

Implementation of:
* Apriori Algorithm
* FP-Growth Algorithm

for frequent itemset mining in C++

## Project Structure
The project contains the following files and folders:

* **compile.sh**     : sh script to compile C++ code
* **2014CS50292.sh** : sh script to run apriori/fptree/plot relative performance
* **Makefile**       : make targets to compile/clean source ( uses O3 )
* **plot.py**        : python script to plot relative performance of the two algorithms
* **include**        : c++ header files containing code for mining algorithms
  * AprioriHandler.h  -> Handler for running apriori algorithm
  * FPHandler.h       -> Handler for running fpgrowth algorithm
  * ReadTransaction.h -> Handler for reading one transaction in memory at a time
* **src**            : c++ source file directory
  * Main.cpp          -> Entry point. Uses handler header files in include  

## Observations

### Faster runs of FPTree in comparison to Apriori

* We observed that Apriori is slower than FPTree algorithm for the same file
  input and support value by around an order of 100.
* This is primarily because Apriori deals with exponential number of candidates
  for a given number of transactions. Any such algorithm is bound to be 
  intractable in practice. Candidate generation from frequent sets of preceding
  size quickly becomes very slow with increasing pattern size.
* FPTree algorithm runs faster because it has polynomial time complexity in the
  number of transactions, unique elements and average transaction length.

### Decreasing run time with increasing minimal support
* Both apriori and FPTree algorithms eliminate possible candidates by
  checking their support value.
* As minimal support percentage is increased both algorithms are able to reject more candidates and hence run faster.
* Infact, with 90% min support the provided transaction file produces no frequent item set.

## Team Members
* Rachit Arora - 2014CS50292
* Akshit Goyal - 2014CS50278
* Nikhil Gupta - 2014CS50462

## Usage
* Compilation: `sh compile.sh`
* Run:
  * Apriori: `sh 2014CS50292.sh input-file support% -apriori output-file`
  * FPTree : `sh 2014CS50292.sh input-file support% -fptree output-file`
  * Plotting relative performance
      ```
      sh 2014CS50292.sh input-file -plot
      ```


