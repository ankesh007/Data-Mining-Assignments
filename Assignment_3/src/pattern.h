#ifndef _PATTERN_H
#define	_PATTERN_H

#include <vector>
#include <string>
#include <algorithm>
#include <iostream>
#include <map>
#include "common.h"
#include "graph.h"
using namespace std;

extern int positive_graph_count;		
extern int negative_graph_count;		

typedef vector<int> occ;	
							

class pattern {
public:
    int** matrix;     
    int size;           
    int edge_size;      
    float score_precise;    
    int score_binned;   
    vector<int> code; 
    vector<pair<int, vector<occ*>* > > embeddings;    
														
    vector<int> pgids;	
    vector<int> ngids;	
    vector<bool> dead_node; 
    vector<bool> is_alive;  
    bool has_potential;     
    int momentum;			


    pattern();			
    vector<int> gen_code();   
    vector<pattern *> *filter_patterns(vector<pattern *> *patterns);
    vector<pattern *> *extend_pattern();
};

#endif	

