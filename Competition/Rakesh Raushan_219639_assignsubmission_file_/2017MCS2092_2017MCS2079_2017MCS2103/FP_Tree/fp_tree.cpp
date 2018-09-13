#include "fp_tree.h"
clock_t start_main=clock();

unordered_map<int ,int > freq_map;         //Stores frequency count of items in 1st read of database
int thresh=0;                              //Stores MinSupport count required
vector<pair<int, int>> freq_sorted;        //Stores Item ids in increasing order according to their frequency
vector<int> item_corr_order;               //Stores Item ids having frequency>minSup in decreasing order by frequency
vector<vector<int>> freq_itemset_vec;      /***Stores all frequent Itemsets and later print that***/
unordered_map<int, int> item_map;          //Stores Item Ids mapped to their rank by decreasing frequency for quick access
map< vector<int>,vector< pair < FPptr ,int>>> considered_count_map;//Stores minimum count of itemsets in Tree

vector<vector<int>> vec;
vector<vector<FPptr>> vec_ptrs;
vector<int> one_sized;
void write_file(string outfile_name);


int main(int argc, char **argv){
    string in_filename = argv[1];
    string out_filename = argv[3];
    double supp_thresh = (stoi(argv[2])/100.0);

    clock_t start_trans_freq_count,start_freq_sort,start_build_FP_Tree;
    double diff_trans_freq_count,diff_freq_sort,diff_build_FP_Tree,diff_main;
    int i;
    /******** Reading the Transaction Database to find Frequency once ********/
    start_trans_freq_count=clock();
    long trans_count = trans_freq_count(in_filename);
    thresh=ceil(supp_thresh * trans_count);
    diff_trans_freq_count = (clock() - start_trans_freq_count)/ (double) CLOCKS_PER_SEC;

    /******** Sorting & Pruning the Item ids on the basis of its frequency ********/
    start_freq_sort =clock();
    freq_sort(thresh);
    freq_itemset_vec.push_back(item_corr_order);           //Inserting 1-Itemset in frequent itemset
    vector<vector<FPptr>> header_table(item_corr_order.size());
    diff_freq_sort = (clock() - start_freq_sort)/ (double) CLOCKS_PER_SEC;

    /******** Building FP Tree*********/
    start_build_FP_Tree=clock();
    build_FP_Tree(in_filename,header_table);
    diff_build_FP_Tree = (clock() - start_build_FP_Tree)/ (double) CLOCKS_PER_SEC;

    /*********Finding Frequent Itemsets***********/
    for(i=header_table.size()-1;i>=0;--i){
        one_sized.clear();
        one_sized.push_back(item_corr_order[i]);
        buildCondTree(header_table[i],one_sized,1);
        while(vec.size()>0){
            buildCondTree(vec_ptrs[0],vec[0],0);
            vec.erase(vec.begin());
            vec_ptrs.erase(vec_ptrs.begin());
        }
    }

    /*****Printing the frequent itemsets*****/
    write_file(out_filename);

    diff_main = (clock() - start_main)/ (double) CLOCKS_PER_SEC;
    //double temp=diff_main-diff_build_FP_Tree-diff_freq_sort-diff_trans_freq_count;
    //cout<<"Total Time taken for Frequency count is:  "<<diff_trans_freq_count<< endl;
    //cout<<"Total Time taken for Item ids frequency sort and pruning is:  "<<diff_freq_sort<< endl;
    //cout<<"Total Time taken for Build FPTree is:  "<<diff_build_FP_Tree<< endl;
    //cout<<"Total Time taken for Rest is:  "<<temp<<" seconds."<< endl<<endl;
    //cout<<"Total Time taken for Complete Program is:  "<<diff_main<<" seconds."<< endl<<endl;
    return 0;
}

void write_file(string outfile_name){
    ofstream outfile(outfile_name);
    string total_data ;
    for(auto i : freq_itemset_vec[0])
        total_data += to_string(i) + "\n";

    for(int j = 1; j < freq_itemset_vec.size(); j++){
       for(int k : freq_itemset_vec[j])
            total_data += to_string(k) + " ";
       total_data +="\n";
    }
    outfile<<total_data;
    outfile.close();
}
