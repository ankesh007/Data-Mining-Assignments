#include "fp_tree.h"

inline int fast_atoi( const char * st )
{    int val = 0;
    while( *st )
        val = val*10 + (*st++ - '0');
    return val;
}

void build_FP_Tree(string& filename,vector<vector<FPptr>> &header_table){
    /*****Defining ROOT Node*****/
    FPptr root= new FPNode();
    root->data= -1;
    root->count=0;
    root->parent= nullptr;

    vector<int> one_transaction;            //Stores Transaction one by one to build FP Tree
    vector<int> trans_inter;                //Stores Items of one transaction having intersection with item_corr_order
    vector<pair<int,int>> vec_pair;         //Stores sorted trans_inter according to decreasing frequency
    set<int> it_set(item_corr_order.begin(),item_corr_order.end());  //To ease intersection item_corr_order as it needs sorted vectors

    FPptr ptr;
    int i;
    const char* c = filename.c_str();
    FILE *fp;
    char buffer[16],str[1048576];
    int k,it = 0,temp;
    fp = fopen(c, "r");
    if(fp==nullptr)
        cout<<"File not Found!";
    else{
        while(fgets(str,1048576,fp) != nullptr){
            ptr = root;
            one_transaction.clear();
            trans_inter.clear();
            vec_pair.clear();

            k=0;
            while(str[k]!='\n'){
                while(isdigit(str[k])){
                    buffer[it++]=str[k++];
                }
               temp=fast_atoi(buffer);
                if(it_set.count(temp))
                    trans_inter.push_back(temp);
                k++;
                memset(buffer,0,16);
                it=0;
            }
           for (k = 0; k < trans_inter.size(); k++)
                vec_pair.emplace_back(item_map[trans_inter[k]], trans_inter[k]);

            sort(vec_pair.begin(), vec_pair.end());
            /*****Building FPTree*****/
           for (i = 0; i < vec_pair.size(); i++)
                ptr = BuildTree(ptr, vec_pair[i].second, vec_pair[i].first, header_table);
        }
    }
    fclose(fp);
}



