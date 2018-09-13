#include<bits/stdc++.h>
#include<iostream>
using namespace std;

void candidate_generation(vector<set<int> > &frequent_last, vector<set<int> > &candidate_itemsets);
void write_file(vector<set<int> > total_items, string &out);
int Apriori(string in, double threshold, string out);

double tot_time = 0;
clock_t cg_st, cg_stop;

int main(int argc, char **argv) {
    string in_file = argv[1];
    double thresh = (stoi(argv[2])/100.0);
    string out_file = argv[3];
    Apriori(in_file,thresh,out_file);
}

inline int fast_atoi( const char * st )
{   int val = 0;
    while( *st )
        val = val*10 + (*st++ - '0');
    return val;
}

int Apriori(string in, double threshold, string out) {
    double time_taken;
    string filename = "./" + in;
    //double threshold = 0.01;
    long ll = 0;
    string outfile_name = "./" + out;
    clock_t start = clock();

    vector<vector<int>> new_transactions;
    unordered_map<int, int> initial_frequency;
    unordered_map<int, int>::iterator it_map;

    //1st time reading for frequency count
    int transaction_count = 0;
    FILE *fp;
    const char *c = filename.c_str();
    char buffer[16], str[1048576];
    int i, it = 0;
    fp = fopen(c, "r");
    while (fgets(str, 1048576, fp) != nullptr) {
        i = 0;
        transaction_count++;
        while (str[i] != '\n') {
            while (isdigit(str[i])) {
                buffer[it++] = str[i++];
            }
            initial_frequency[fast_atoi(buffer)]++;
            i++;
            memset(buffer, 0, 16);
            it = 0;
        }
    }
    fclose(fp);

    //file processing done. Transaction database created. Initial frequencies are calculated.
    time_taken = (clock() - start) / (double) CLOCKS_PER_SEC;
    //cout << "\nTotal Time taken for 1st read is: " << time_taken << " seconds." << "\n";
    //cout << "\nMap has " << initial_frequency.size() << " buckets\n";

    int min_count = ceil(threshold * transaction_count);
    it_map = initial_frequency.begin();
    auto temp = initial_frequency.begin();
    vector<set<int> > frequent_itemsets;

    //Deleting items with support less than minsup
    while (it_map != initial_frequency.end()) {
        if ((it_map->second) < min_count) {
            temp = it_map;
            it_map++;
            initial_frequency.erase(temp);
        } else {
            set<int> temp_set{it_map->first};
            frequent_itemsets.push_back(temp_set);
            it_map++;
        }
    }
    //         1-length frequent itemset is created.


    int F = frequent_itemsets.size();
    vector<set<int> > total_frequent_itemsets = frequent_itemsets;

/**************  Apriori Algorithm implementation******************/
    vector<set<int> > candidate_items;
    clock_t fq_st, fq_stop;
    double fq_diff = 0, ffq_diff = 0;
    unordered_set<int> items;

    int item_set_number = 0;
    while (F != 0) {
        // candidate generation.
        candidate_items.clear();
        cg_st = clock();
        candidate_generation(frequent_itemsets, candidate_items);
        cg_stop = clock();
        double diff = (cg_stop - cg_st) / (double) CLOCKS_PER_SEC;;
        //cout<<"Intermediate Time taken for Candidate Generation is:"<<diff<<" seconds."<< endl;
        tot_time += diff;
        // vector for keeping count of each itemset in candidate itemset.
        vector<int> f_count(candidate_items.size());
        if (candidate_items.empty()) {  //Algorithm Finished
            break;
        } else {
            int k;
            it = 0;
            fp = fopen(c, "r");
            while (fgets(str, 1048576, fp) != nullptr) {
                items.clear();
                k = 0;
                while (str[k] != '\n') {
                    while (isdigit(str[k])) {
                        buffer[it++] = str[k++];
                    }
                    items.insert(fast_atoi(buffer));
                    k++;
                    memset(buffer, 0, 16);
                    it = 0;
                }
                item_set_number = 0;
                fq_st = clock();
                for (const auto &j : candidate_items) {
                    if (!any_of(j.begin(), j.end(), [&items](int i) { return items.count(i) == 0; })) {
                        f_count[item_set_number] += 1;
                    }
                    ll++;
                    item_set_number++;
                }
                ffq_diff = (clock() - fq_st) / (double) CLOCKS_PER_SEC;
                fq_diff += ffq_diff;
            }
         fclose(fp);

        // Pruning infrequent candidates from candidates_items.
        int indexer = 0;
        auto itr_candidate = candidate_items.begin();
        while (itr_candidate != candidate_items.end()) {
            if (f_count[indexer] < min_count) {
                itr_candidate = candidate_items.erase(itr_candidate);
                indexer++;
            } else {
                itr_candidate++;
                indexer++;
            }
        }

        // now candidates_items contains only the frequent itemsets.
        frequent_itemsets.clear();
        total_frequent_itemsets.insert(total_frequent_itemsets.end(), candidate_items.begin(),
                                       candidate_items.end());
        frequent_itemsets = candidate_items;
    }

    F = frequent_itemsets.size();
}

// Writing frequent items to outfile.
    write_file(total_frequent_itemsets, outfile_name);
    time_taken = (clock() - start) / (double) CLOCKS_PER_SEC;
    std::ofstream a_out;
    a_out.open("./a_plot_time.txt",std::ofstream::app);
    a_out<<time_taken<<"\n";
    a_out.close();

    //cout << "Total Time taken for Intersection Generation is:" << fq_diff << " seconds." <<"\n";
    //cout << "Total Intersection count is: " << ll << "  iterations." << endl;
    //cout << "Total Time taken for Apriori Algorithm is:" << time_taken << " seconds." << "\n" << "\n";
    return 0;
}

void candidate_generation(vector<set<int> > &frequent_last, vector<set<int> > &candidate_itemsets) {
    auto candidate_itr = frequent_last.begin();
    auto next_itr = candidate_itr;
    set<int> temp_k1, temp_k2;
    while (candidate_itr != frequent_last.end()) {
        next_itr = candidate_itr + 1;
        while (next_itr != frequent_last.end()) {

            //create two k-1 size sets from k size sets.
            temp_k1 = *candidate_itr;
            temp_k2 = *next_itr;
            auto del_itr = temp_k1.end();
            del_itr--;
            temp_k1.erase(del_itr);
            del_itr = temp_k2.end();
            del_itr--;
            int last = *(del_itr);
            temp_k2.erase(del_itr);

            // if two k-1 size sets are equal merge them and check for subsets.
            if (temp_k1 == temp_k2) {
                temp_k1 = *candidate_itr;
                temp_k1.insert(last);
                set<int> set_ckecker;

                //k-1 size sets are merged. checking for subsets now.
                bool flag = false;
                bool in = true;
                for (int i : temp_k1) {
                    set<int> check = {i};
                    set<int> result;
                    set_difference(temp_k1.begin(), temp_k1.end(), check.begin(), check.end(),
                                   inserter(result, result.end()));
                    auto iter = frequent_last.begin();
                    while (iter != frequent_last.end()) {
                        set<int> check_set = *iter;
                        if (result == check_set) {
                            flag = true;
                            break;
                        }
                        iter++;
                    }
                    if (!flag) {
                        in = false;
                        break;
                    }
                    flag = false;
                }
                if (in)
                    candidate_itemsets.push_back(temp_k1);
            }
            next_itr++;
        }
        candidate_itr++;
    }
}


void write_file(vector<set<int> > total_items, string &out) {
    ofstream outfile(out);
    for (auto k : total_items) {
        string write_data;
        for (auto l : k) {
            write_data += to_string(l) + " ";
        }
        outfile << write_data << "\n";
    }
    outfile.close();
}
