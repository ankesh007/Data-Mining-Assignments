#include "fp_tree.h"

void buildCondTree(vector<FPptr>& pointer,vector<int> item,int status){
    int i,count=0;
    map<int ,int > count_items_in_cond_fp_tree;
    map<int ,vector<FPptr >> ptrs;
    vector<int> temp_vec;
    temp_vec=item;
    vector<pair<FPptr,int>> vector_of_pairs;
    vector<pair<FPptr,int>> vop;
    map<int,int>::iterator it;
    map<int ,vector<FPptr>>::iterator it_ptr;
    FPptr node_ptr;
    for(i=0;i<pointer.size();++i){
        if(status)
            count=pointer[i]->count;
        else{
            vop=considered_count_map[item];
            for(int k=0;k<vop.size();++k){
                if(vop[k].first==pointer[i]){
                    count=vop[k].second;
                    break;
                }
            }
        }
        node_ptr=pointer[i]->parent;
        while(node_ptr->parent){
            temp_vec.push_back(node_ptr->data);
            if(count_items_in_cond_fp_tree.find(node_ptr->data)==count_items_in_cond_fp_tree.end()){
                count_items_in_cond_fp_tree.insert(pair<int,int>(node_ptr->data,count));
                node_ptr->considered_count=count;
                ptrs[node_ptr->data].push_back(node_ptr);
                considered_count_map[temp_vec].push_back(pair<struct FPNode*,int>(node_ptr,node_ptr->considered_count));
            }
            else{
                count_items_in_cond_fp_tree.find(node_ptr->data)->second+=count;
                if(find((ptrs.find(node_ptr->data)->second).begin(),(ptrs.find(node_ptr->data)->second).end(),node_ptr)!=(ptrs.find(node_ptr->data)->second).end()){
                    node_ptr->considered_count+=count;
                    vector_of_pairs=considered_count_map[temp_vec];
                    for(int k=0;k<vector_of_pairs.size();++k){
                        if(vector_of_pairs[k].first==node_ptr) {
                            vector_of_pairs[k].second = node_ptr->considered_count;
                            break;
                        }
                    }
                    considered_count_map[temp_vec]=vector_of_pairs;
                }
                else{
                    ptrs[node_ptr->data].push_back(node_ptr);
                    node_ptr->considered_count=count;
                    considered_count_map[temp_vec].push_back(pair<struct FPNode*,int>(node_ptr,node_ptr->considered_count));
                }
            }
            node_ptr=node_ptr->parent;
            temp_vec.erase(temp_vec.end()-1);
        }
    }
    it=count_items_in_cond_fp_tree.begin();
    it_ptr=ptrs.begin();
    while(it!=count_items_in_cond_fp_tree.end()){
        if(it->second>=thresh) {
            item.push_back(it->first);
            freq_itemset_vec.push_back(item);
            vec.push_back(item);
            vec_ptrs.push_back(it_ptr->second);
            item.erase(item.end()-1);
        }
        ++it;
        ++it_ptr;
    }
}
