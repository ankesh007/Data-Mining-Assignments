#ifndef FP_HANDLER_H
#define FP_HANDLER_H

#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <list>
#include <stack>
using namespace std;

#include "ReadTransaction.h"
struct FPNode
{
    int m_id;
    int m_count;
    bool m_is_root;
    FPNode* m_parent;
    list<FPNode*> m_children;
    FPNode* m_auxillary;

    FPNode();
    FPNode(const FPNode* original);
};

struct FPTree
{
    FPNode* m_root;
    unordered_map<int,list<FPNode*> > m_tree_index;

    FPTree();
    ~FPTree(); // FPTree destruction will auto destruct all FPNodes in the tree
    void printTree();
};

class FPHandler
{
private:
    string m_input_filename, m_output_filename;
    double m_min_support;
    int m_total_transactions, m_total_ids;
    FPTree* m_fptree;
    
    void addBranchToTree( vector<int>& branch );
    void generateMainFpTree();
    void fillProjectedTree(FPTree* proj_tree, list<FPNode*>& node_list);
    void fpGrowth(FPTree* fptree, vector<vector<int>> & freq_set);

public:
    FPHandler(string input_file, string output_file, double min_support);
    void generateFrequentItemSet();
    ~FPHandler();
};

/////////////////////// Implementation ///////////////////////////
// FPNode
inline FPNode::FPNode():
    m_id(0),
    m_count(0),
    m_is_root(false),
    m_parent(nullptr),
    m_auxillary(nullptr)
{
    m_children.clear();
}

inline FPNode::FPNode(const FPNode* original):
    m_id(original->m_id),
    m_count(original->m_count),
    m_is_root(original->m_is_root),
    m_parent(original->m_parent),
    m_auxillary(original->m_auxillary)
{
    m_children.clear();
}

/////////////////////// Implementation ///////////////////////////
// FPTree
inline FPTree::FPTree()
{
    m_root = new FPNode();
    m_root->m_is_root = true; // Signifies null node
    m_tree_index.clear();
}
inline FPTree::~FPTree() // FPTree destruction will auto destruct all FPNodes in the tree
{
    delete m_root;
    for (auto el = m_tree_index.begin(); el != m_tree_index.end(); ++el)
    {
        for (auto node : el->second)
        {
            delete node;
        }
        el->second.clear();
    }
}

inline void FPTree::printTree()
{
    cerr<<"----------------Printing tree -------------------\n";
    for (auto& iter : m_tree_index)
    {
        cerr<<"id : "<<iter.first;
        auto& index_list = iter.second;
        cerr<<" check id : "<<(*index_list.begin())->m_id;
        cerr<<" count : "<<(*index_list.begin())->m_count;
        cerr<<" index length : "<<index_list.size()<<endl;
        auto it = index_list.begin();
        while(++it != index_list.end())
        {
            cerr<<(*it)->m_id<<" "<<(*it)->m_parent->m_id<<" c : "<<(*it)->m_count<<endl;
        }
    }
}

/////////////////////// Implementation ///////////////////////////
// FPHandler

inline FPHandler::FPHandler(string input_file, string output_file, double min_support):
    m_input_filename(input_file),
    m_output_filename(output_file),
    m_min_support(min_support),
    m_total_transactions(0),
    m_total_ids(0)
{
}

inline void FPHandler::generateFrequentItemSet()
{
    generateMainFpTree();
    vector<vector<int> > freq_set;
    fpGrowth(m_fptree, freq_set);

    WriteFrequentItems* write_helper = new WriteFrequentItems(m_output_filename);
    write_helper->printItems(freq_set);
    delete write_helper;

}

inline FPHandler::~FPHandler()
{
    delete m_fptree;
}

inline void FPHandler::addBranchToTree( vector<int>& branch )
{
    auto parent = m_fptree->m_root;
    size_t index = 0;
    while (index < branch.size())
    {
        bool found = false;
        for(auto child : parent->m_children)
        {
            if(child->m_id == branch[index]) //Match, increment count and continue
            {
                child->m_count++;
                found = true;
                parent = child;
                break;
            }
        }
        if(!found)
        {
            FPNode* new_list_child = new FPNode();
            new_list_child->m_parent = parent;
            new_list_child->m_id = branch[index];
            new_list_child->m_count = 1;
            auto& tree_index = m_fptree->m_tree_index;
            if (tree_index.find(branch[index]) == tree_index.end())
            {
                FPNode* indexnode = new FPNode();
                indexnode->m_id = branch[index];
                list<FPNode*> indexlist;
                indexlist.push_back(indexnode);
                tree_index.emplace(branch[index], indexlist);
            }
            tree_index[branch[index]].push_back(new_list_child);
            auto indexhead = tree_index[branch[index]].begin();
            (*indexhead)->m_count++;
            parent->m_children.push_back(new_list_child);
            parent = new_list_child;
        }
        index++;
    }
}

inline void FPHandler::generateMainFpTree()
{
    unordered_map<int,int> id_to_freq;
    ReadTransaction* read_helper = new ReadTransaction(m_input_filename);
    vector<int> transaction;
    while(read_helper->nextTransaction(transaction) > 0)
    {
        m_total_transactions++;
        for(auto id : transaction)
        {
            if(id_to_freq.find(id) != id_to_freq.end())
            {
                ++id_to_freq[id];
            }
            else
            {
                id_to_freq.emplace(id,1);
                m_total_ids++;
            }
        }
        transaction.clear();
    }

    function<bool(int left, int right)> compare_freq = 
        [ &id_to_freq ] (int left, int right) -> bool
        {
            int leftf = id_to_freq[left];
            int rightf = id_to_freq[right];
            return ((leftf > rightf) || ((leftf == rightf) && (left < right)));
        };

    m_fptree = new FPTree();
    read_helper->resetHead(); // Read transactions for the second time from disk
    while(read_helper->nextTransaction(transaction) > 0)
    {
        vector<int> branch;
        for ( auto id : transaction )
        {
            if ((id_to_freq[id]/((double) m_total_transactions)) >= m_min_support)
                branch.push_back(id);
        }

        sort(branch.begin(), branch.end(), compare_freq);
        addBranchToTree(branch);
        transaction.clear();
    }
    
    // Update index node counts in m_fptree
    auto& tree_index = m_fptree->m_tree_index;
    for (auto iter : tree_index)
    {
        auto& total_count = (*iter.second.begin())->m_count;
        total_count = 0;
        auto node_iter = iter.second.begin();
        while(++node_iter != iter.second.end())
        {
            total_count += (*node_iter)->m_count;
        }
    }
    delete read_helper;
}

inline void FPHandler::fillProjectedTree(FPTree* proj_tree, list<FPNode*>& node_list)
{
    auto& tree_index = proj_tree->m_tree_index;
    auto iter = node_list.begin();
    // Skip index node
    while(++iter != node_list.end()) // Process and copy each branch till top
    {
        auto node = *iter;
        int original_count = node->m_count;
        while(node)
        {
            if(!node->m_parent)
            {
                cerr<<"[ERROR] FPHandler.h : fillProjectedTree() - Non root node has no parent!";
                exit(0);
            }
            else if(node->m_parent->m_is_root)
            {
                if(node->m_auxillary)
                {
                    node->m_auxillary->m_parent = proj_tree->m_root;
                }
                node = nullptr;
                continue;
            }

            if(node->m_parent->m_auxillary) // Already copied
            {
                if(node->m_auxillary)
                {
                    node->m_auxillary->m_parent = node->m_parent->m_auxillary;
                }
                node->m_parent->m_auxillary->m_count += original_count;
                node = node->m_parent;
            }
            else
            {
                FPNode* parent_copy = new FPNode(node->m_parent);
                parent_copy->m_auxillary = node->m_parent;
                node->m_parent->m_auxillary = parent_copy;
                if(node->m_auxillary) // Self has a copy
                {
                    node->m_auxillary->m_parent = parent_copy;
                }
                parent_copy->m_count = original_count;
                // Add parent copy to projected tree index
                if (tree_index.find(parent_copy->m_id) != tree_index.end())
                {
                    tree_index[parent_copy->m_id].push_back(parent_copy);
                }
                else // Add index node too
                {
                    FPNode* index_node = new FPNode();
                    index_node->m_id = parent_copy->m_id;
                    list<FPNode*> index_list;
                    index_list.push_back(index_node);
                    index_list.push_back(parent_copy);
                    tree_index.emplace(parent_copy->m_id, index_list);
                }
                node = node->m_parent;
            }
        }

    }

    // Update projected tree index nodes
    for (auto iter : tree_index)
    {
        auto& total_count = (*iter.second.begin())->m_count;
        total_count = 0;
        auto node_iter = iter.second.begin();
        while(++node_iter != iter.second.end())
        {
            total_count += (*node_iter)->m_count;
        }
    }

    // Remove auxillary linkings from all nodes
    iter = node_list.begin();
    // Skip index node
    while(++iter != node_list.end())
    {
        auto node = *iter;
        while(node)
        {
            if(node->m_is_root)
            {
                node = nullptr;
                continue;
            }
            else
            {
                if(node->m_auxillary)
                    node->m_auxillary->m_auxillary = nullptr;
                node->m_auxillary = nullptr;
                node = node->m_parent;
            }
        }
    }
}

// Recursive DO NOT INLINE
void FPHandler::fpGrowth(FPTree* fptree, vector<vector<int>> & freq_set)
{
    if(fptree->m_tree_index.empty()) // Base Case
    {
        return;
    }

    // Recursively call fpGrowth with projected tree
    // For each element in tree index
    for (auto iter : fptree->m_tree_index)
    {
        int id = iter.first;
        auto& node_list = iter.second;
        int count = (*node_list.begin())->m_count;

        if ((((double)count)/m_total_transactions) < m_min_support)
        {
            continue; // Skip because insufficient support
        }

        // New projected tree
        FPTree* proj_tree = new FPTree();
        fillProjectedTree(proj_tree, node_list);
        vector<vector<int>> new_set;
        fpGrowth(proj_tree, new_set);
        for( auto& pat : new_set )
        {
            pat.push_back(id);
            freq_set.push_back(pat);
        }
        // Push singleton id
        freq_set.push_back(vector<int> (1, id));
        delete proj_tree;
    }
}

#endif
