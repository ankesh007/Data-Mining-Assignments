#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <unordered_map>
#include <unordered_set>
#include <assert.h>
#include <algorithm>
#include <ostream>
#include <iterator>

using namespace std;
bool plot;

class Apriori {
    private:
    /* List of itemsets */
    vector<vector<int>> itemsets;
    /* Name of the output file */
    string transactionFileName;
    /* Number of different items in the dataset */
    int numItems;
    /* Number of transactions in transaction file */
    int numTransactions;
    /* Minimum support for a itemset to be frequent */
    double minimumSupport;

    ofstream output;

    public:
    /* Constructor to get the parameters and initialize the algorithm */
    Apriori(string inFileName, string outFileName, double support) {
        transactionFileName = inFileName;
        minimumSupport = support/100.0;

        string line;
        numItems = 0;
    	numTransactions=0;
        ifstream input(transactionFileName, ios::in);
        int number;
        output.open(outFileName, ios::out);

        if (input.is_open()) {
            while (getline(input, line)) {
                numTransactions++;
                istringstream iss(line);
                while (iss >> number) {
                    if (number+1 > numItems)
                        numItems = number+1;
                }
            }
            input.close();
        }
        // cout << "Number of items: " << numItems << ", Number of transactions: " << numTransactions << endl;
    }

    void start() {
        clock_t begin;
        if (plot)
            begin = clock();

        // Generate candidates of size 1
        createCandidatesOfSizeOne();
        createFrequentSetsOfSizeOne();
        int itemsetSize = 1; // Current size of itemset 
        int totalFrequentSets = 0;
        while (itemsets.size() > 0) {
            if (itemsets.size() != 0) {
                totalFrequentSets += itemsets.size();
                createCandidateSetsFromFrequentSets();
                createFrequentSetsFromCandidateSets();
                itemsetSize++;
            }
        }
        if (plot) {
            clock_t end = clock();
            double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
            cout << elapsed_secs << endl;
        }
        // cout << "Execution time is: " << elapsed_secs << endl;
        output.close();
    }

    void createCandidatesOfSizeOne() {
        for (int i = 0; i < numItems; i++) {
            vector<int> temp{i};
            itemsets.push_back(temp);
        }
    }

    void createFrequentSetsOfSizeOne() {
        vector<int> counts(itemsets.size(), 0);
        vector<vector<int>> frequentItemsets;

        string line;
        ifstream input(transactionFileName, ios::in);
        int number;

        for (int i = 0; i < numTransactions; i++) {
            getline(input, line);
            istringstream iss(line);
            while (iss >> number)
                counts[number]++;
        }
        input.close();
        for (int i = 0; i < itemsets.size(); i++)
			if ((counts[i]/(double)numTransactions) >= minimumSupport) {
				frequentItemsets.push_back({i});
                output << i << endl;
            }

        itemsets = frequentItemsets;
    }

    void createFrequentSetsFromCandidateSets() {
        
        vector<vector<int>> frequentItemsets;

        bool isFrequent;
        vector<int> counts(itemsets.size(), 0);

        string line;
        ifstream input(transactionFileName, ios::in);
        int number;

        for (int i = 0; i < numTransactions; i++) {
            getline(input, line);
            istringstream iss(line);
            vector<bool> masks(numItems, false);
            while (iss >> number)
                masks[number] = true;

            for (int j = 0; j < itemsets.size(); j++) {
                isFrequent = true;
                for (int item: itemsets[j])
                    if (masks[item] == false) {    
                        isFrequent = false;
                        break;
                    }

                if (isFrequent)
                    counts[j]++;
            }
        }
        input.close();
        for (int i = 0; i < itemsets.size(); i++)
			if ((counts[i]/(double)numTransactions) >= minimumSupport) {
				frequentItemsets.push_back(itemsets[i]);
                for (auto &it: itemsets[i])
                    output << it << " ";
                output << endl;
            }

        itemsets = frequentItemsets;
        // cout << "Done finding frequent sets: " << itemsets.size() << endl;
    }

    void createCandidateSetsFromFrequentSets() {
        int itemsetSize = itemsets[0].size();
        unordered_map<string, vector<int>> hash_map;
        int size = itemsets.size();
        for (int i = 0; i < size; i++) {
            for (int j = i+1; j < size; j++) {
                vector<int> f1 = itemsets[i];
                vector<int> f2 = itemsets[j];
                assert(f1.size() == f2.size());

                /* New itemset to be made with n-1 elements same
                nth element from f1 and n+1th element from f2
                */
                vector<int> newItemSet(f1);
                newItemSet.push_back(0);

                // Number of different items in 2 itemsets
                int different = 0;
                for (int k = 0; k < f2.size(); k++) {
                    bool found = false;
                    for (int l = 0; l < f1.size(); l++) {
                        if (f1[l] == f2[k]) {
                            found = true;
                            break;
                        }
                    }
                    if (!found) {
                        different++;
                        newItemSet[f1.size()] = f2[k];
                    }
                }
                assert(different > 0);


                if (different == 1) {
                    sort(newItemSet.begin(), newItemSet.end());
                    // TODO Pruning

                    stringstream result;
                    copy(newItemSet.begin(), newItemSet.end(), ostream_iterator<int>(result, " "));
                    hash_map[result.str().c_str()] = newItemSet;
                }
            }
        }
        vector<vector<int>> temp;
        for (auto it: hash_map)
            temp.push_back(it.second);
    
        itemsets = temp;
        // cout << "Done making new itemsets: " << itemsets.size() << endl;
    }
};

void aprioriGo(string transactionFileName, string outputFileName, double minimumSupport) {
    Apriori apriori(transactionFileName, outputFileName, minimumSupport);
    apriori.start();
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    string transactionFileName(argv[1]);
    double minimumSupport = atof(argv[2]);
    string outputFileName(argv[3]);
    plot = atoi(argv[4]) == 1 ? true : false;
    outputFileName += ".txt";
    aprioriGo(transactionFileName, outputFileName, minimumSupport);
    return 0;
}