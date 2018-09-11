#include "fp_tree.h"

FPptr BuildTree(FPptr node,int &item_to_insert,int &header_index,vector<vector<FPptr>>& header_table) {
     for (auto it :(node->child_pointers))
           if (it.first == item_to_insert) {
               it.second-> count += 1;
               return (it.second);
           }

    /*****Inserting New node if item not found in path*****/
    FPptr new_node = new FPNode();
    new_node->parent = node;
    new_node->count = 1;
    new_node->considered_count = 0;
    new_node->data = item_to_insert;
    (node->child_pointers).emplace_back(item_to_insert, new_node);
    header_table[header_index].push_back(new_node);
    return new_node;
}

