/* 
 * File:   EVO.h
 * Author: njin
 *
 * Created on October 10, 2009, 7:30 PM
 */

#ifndef _EVO_H
#define	_EVO_H
#include "graph.h"
#include "pattern.h"
#include "candidate_list.h"
#include "pattern_index.h"
#include "feature.h"
#include <vector>
#include <string>
#include <fstream>
#include <iostream>
#include <map>
using namespace std;

extern int positive_graph_count; //number of positive graphs
extern int negative_graph_count; //number of negative graphs
extern int iter_num;    //number of iterations
extern double bg_freq_threshold;    //negative frequency threshold; not used
extern int max_size;    //maximal sizes of patterns
extern double pfreq_threshold;  //positive frequency threshold; not used
extern string NF_SUFFIX;
extern string EF_SUFFIX;
extern int test_samples;
//class: EVO
/*implementation of the discriminative subgraph mining algorithm GAIA*/
class EVO {
public:
    vector<graph*> graphs;  //input graphs
    string fprefix; //file name prefix
    pattern_index pat_idx;  //pattern index for existing patterns
	
    /*it's possible that a graph doesn't have a feature or has an empty
     feature because it's required that a feature has positive frequency higher
     than negative frequency*/
    vector<feature*> graph_features; // the feature pattern for each graph
    float features_min_score;   //the minimal feature score found so far
    vector<candidate_list*> candidate_lists;    //candidate list for each positive graph
    vector<bool> feature_updated;   //whether a feature has been updated
    map<string, pattern*> edges;    //pattern index for edges;
                                    //because codes for edges are not calculated


    float norm_acc(int mis_pos, int mis_neg);   //normalized accuracy
    void feature_selection(vector<feature*>& features);	/*select features to optimize 
														 normalized accuracy*/
    
	//output resulting feature subgraph patterns to a file
    void fout_feature_matrix(vector<int>& code, int size, ofstream& fout);
    void fout_feature_matrix(int gid, ofstream& fout);
    void fout_feature_matrix(feature* f, ofstream& fout);
    void fout_basic_result();
    void fout_features();
    void fout_training_matrix();
	
    bool read_graphs(); /*read the input graphs; 
						 assumes the gids in both input files are in increasing order*/
    
    bool distribute_pattern(pattern* p); //pattern candidate migration
										 /*returns whether the pattern survived,
                                          it also updates features*/
    
	/*insert an edge into the edge index*/
	void insert_edge(int gid, int n1, int n2, int l1, int l2, int le);
    
    void init_edges();  //initialize edge index
						/*codes for edges are not calculated because a pattern will
                         never grow into an edge*/
	
    void pick_one_grow(int gid);    /*pick one subgraph pattern from the
									 candidate list of positive graph whose ID is gid
									 and extend this subgraph pattern*/
	
    void evolution();   //one call of evolution is one iteration
	
    EVO(string fprefix);	/*given the graph files with the prefix given in the parameter,
							 mine discriminative subgraph patterns based on the algorithm GAIA*/
    ~EVO();
#ifdef PRE_EXTENSION_PRUNING
    bool has_potential_pre(pattern* p);  /*if the score of a pattern p is no 
     * greater than the scores of p's supporting graphs' features, there's no need to
     * extend p*/
#endif
};

#endif	/* _EVO_H */

