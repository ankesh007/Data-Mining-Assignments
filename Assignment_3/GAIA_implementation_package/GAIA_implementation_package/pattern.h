/* 
 * File:   pattern.h
 * Author: njin
 *
 * Created on October 10, 2009, 7:05 PM
 */

#ifndef _PATTERN_H
#define	_PATTERN_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>
#include "common.h"
#include "pattern_index.h"
#include "graph.h"
using namespace std;

extern int pos_num;		//global variable: number of positive graphs
extern int neg_num;		//global variable: number of negative graphs

typedef vector<short> occ;	//stores one occurrence of one subgraph pattern; 
							//graph id is stored separately

typedef struct
{
    /*embeddings is a two dimensional matrix, par_em_gid and par_occ_id
     are indices in the embeddings of the parent pattern*/
    short par_em_gid;
    int par_occ_id;
    short source;   //one node in the extension edge
    short drain;    //the other node in the extension edge
}ext_occ;   //extension occurrences

//class: extension
/*only stores the information of the extension edge but not the base subgraph pattern*/
class extension
{
public:
    bool is_internal_ext;   //if both nodes in the extension are already in the graph
    vector<short> pgids;    //supporting positive graphs
    vector<short> ngids;    //supporting negative graphs
    vector<ext_occ*> ext_occs;  //occurrences of this extension
    ~extension();
};

//class: pattern
/*stores the information of a subgraph pattern;
 methods to extend, calculate the code of, calculate the score of and output 
 the adjacency matrix of a subgraph pattern*/
class pattern {
public:
    short** matrix;     //adjacency matrix
    int size;           //number of nodes
    int edge_size;      //number of edges
    float score_precise;    //accurate score
    int score_binned;   //discretized score
    vector<short> code; //code of this pattern
    vector<pair<short, vector<occ*>* > > embeddings;    //occurrences:
														//.first is gid, .second is occurrence
    vector<short> pgids;	//IDs of the positive supporting graphs
    vector<short> ngids;	//IDs of the negative supporting graphs
    vector<bool> dead_node; //nodes that don't have new extensions
    vector<bool> is_alive;  //nodes that have new extensions
    bool has_potential;     //can still extend into larger patterns
#ifdef MOMENTUM
    int momentum;			//amount of momentum left
#endif
    map<int, extension* > extensions;	//all possible the extensions


    pattern();			//constructor
    ~pattern();			//destructor
    vector<short> gen_code();   //generate the code of this pattern
    bool check_embeddings(vector<graph*>& graphs); //check if the embeddings of the pattern are correct
    void get_score();   //calculate score
    void cout_matrix(); //output matrix
    void cout_code();   //output code
	
    //generate a new pattern based on the extension
    //pattern_index is the index of all existing patterns
    pattern* gen_new_pattern(extension& ext, int code, pattern_index& pat_idx, vector<graph*>& graphs);
    
	//find all possible extensions
    void collect_ext(short gid, short par_em_gid, occ& occ1, short occ1_id, vector<graph*>& graphs);
    
	//generate new subgraph patterns based on all possible extensions
	//returns the vector of new subgraph patterns
    vector<pattern*>* extend(vector<graph*>& graphs, pattern_index& pat_idx); //this function prunes patterns that decrease in score
};

#endif	/* _PATTERN_H */

