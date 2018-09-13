#ifndef DEB_FP_FP_TREE_H
#define DEB_FP_FP_TREE_H
#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <map>
#include <set>
#include <algorithm>
#include <ctime>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <cstring>
#include <cmath>

using  namespace std;
struct FPNode{
    int data;
    int count;
    int considered_count;
    struct FPNode* parent;
    vector<pair<int,struct FPNode*>> child_pointers;
};
typedef struct FPNode* FPptr;

extern unordered_map<int ,int > freq_map;        //Stores frequency count of items in 1st read of database
extern int thresh;                                //Stores MinSupport count required
extern vector<pair<int, int>> freq_pair;         //Stores all frequency count from freq_map in form of vector pair
extern vector<pair<int, int>> freq_sorted;       //Stores Item ids in increasing order according to their frequency
extern vector<int> item_corr_order;              //Stores Item ids having frequency>minSup in decreasing order by frequency
extern unordered_map<int, int> item_map;         //Stores Item Ids mapped to their rank by decreasing frequency for quick access
extern vector<vector<int>> freq_itemset_vec;     /***Stores all frequent Itemsets and later print that***/
extern map< vector<int>,vector< pair < FPptr,int > > > considered_count_map;//Stores minimum count of itemsets in Tree
extern vector<vector<FPptr>> vec_ptrs;
extern vector<vector<int>> vec;

/******FUNCTIONS********/
int trans_freq_count(string&);
void freq_sort(int);
void build_FP_Tree(string&,vector<vector<FPptr>>&);
FPptr BuildTree(FPptr node,int&,int&,vector<vector<FPptr>>& );
void buildCondTree(vector<FPptr>& pointer,vector<int> ,int);

#endif //DEB_FP_FP_TREE_H
