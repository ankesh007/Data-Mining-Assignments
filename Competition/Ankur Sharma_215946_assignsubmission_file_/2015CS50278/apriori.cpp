//
// Created by karthik.sharma98 on 16-08-2018.
//
using namespace std;
#include <iostream>
#include <unordered_map>
#include <set>
#include <vector>
#include <fstream>
#include <algorithm>
#include <sstream>

struct candidate {
    set<int> itemset;
    double frequency;
    int count;
};

//template class std::unordered_map<int,double>;

bool subset_check(set<int> itemset, int excluded, vector<candidate> frequent_set) {
    itemset.erase(excluded);
    bool result = false;
    for(auto candidate: frequent_set) {
        if (candidate.itemset == itemset) {
            result = true;
            break;
        }
    }
    return result;
}

bool not_in(vector<candidate> ck, candidate c) {
    bool result = true;
    for(auto &elem : ck) {
        if (elem.itemset == c.itemset){
            result = false;
            break;
        }
    }
    return result;
}

vector<candidate> candidate_gen(vector<candidate> frequent_set){
    vector<candidate> ck;
    for(auto f1 = frequent_set.begin(); f1 != frequent_set.end(); f1++){
        for (auto f2 = f1 + 1; f2 != frequent_set.end(); f2++) {
            set<int> diff;
            set_difference(f2->itemset.begin(), f2->itemset.end(), f1->itemset.begin(), f1->itemset.end(), inserter(diff, diff.end()));
            if (diff.size() == 1) {
                candidate c;
                c.count = 0;
                c.frequency = 0.0;
                set_union(f1->itemset.begin(), f1->itemset.end(), diff.begin(), diff.end(), inserter(c.itemset, c.itemset.end()));
                bool result = true;
                for(auto &excluded : c.itemset) {
                    result = subset_check(c.itemset, excluded, frequent_set);
                    if (result == false) {
                        break;
                    }
                }
                if(result == true) {
                    if (not_in(ck, c))
                        ck.push_back(c);
                }
            }
        }
    }
    return ck;
}

void apriori(const string infilename, const string outfilename, double support_threshold){

    unordered_map<int, double> C1;

    // read input and generate C1
    ifstream inputFile(infilename);
    if (!inputFile) {
        cout << "Unable to open file";
        exit(1); // terminate with error
    }
    int item;
    long absolute_count = 0;
    string str;
    while(getline(inputFile, str)) {
        absolute_count++;
        istringstream integers(str);
        while(integers >> item) {
            if(C1[item] > 0) {
                C1[item] = C1[item] + 1;
            }
            else{
                C1[item] = 1;
            }
        }
    }

    // final result
    vector<vector<candidate>> frequent_sets;

    // generate F1
    vector<candidate> F1;
    for(auto it = C1.begin(); it != C1.end(); it++) {
        double frequency = it->second * 1.0 / absolute_count;
        if (frequency >= support_threshold) {
            candidate c;
            c.itemset.insert(it->first);
            c.count = it->second;
            c.frequency = frequency;
            F1.emplace_back(c);
        }
    }
    // push F1 to frequent sets
    frequent_sets.push_back(F1);

    // run algorithm
    while(true){
        vector<candidate> ck= candidate_gen(frequent_sets[frequent_sets.size() -1]);
        inputFile.clear();
        inputFile.seekg(0, inputFile.beg);

        while(getline(inputFile, str)) {
            istringstream integers(str);
            set<int> transaction;
            int x;
            while(integers >> x) {
                transaction.insert(x);
            }

            for(auto &cand : ck) {
                if(includes(transaction.begin(), transaction.end(), cand.itemset.begin(), cand.itemset.end())) {
                    cand.count++;
                }
            }
        }
        vector<candidate> frequent_set;

        for(auto &cand: ck) {
            cand.frequency = cand.count * 1.0 / absolute_count;
            if (cand.frequency >= support_threshold) {
                frequent_set.push_back(cand);
            }
        }

        if(frequent_set.size() == 0){
            break;
        }
        else{
           // cout << frequent_sets.size() << "\n";
            frequent_sets.push_back(frequent_set);
        }

    }
    ofstream outputFile(outfilename);
    for(auto &frequent_set : frequent_sets) {
        for(auto &cand : frequent_set) {
            for(auto &item : cand.itemset) {
                outputFile << item << " ";
            }
            outputFile << "\n";
        }
    }
    outputFile.close();
    inputFile.close();

}


