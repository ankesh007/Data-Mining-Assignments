#ifndef DECLARATIONS_H
#define	DECLARATIONS_H

#include <bits/stdc++.h>
#include "graph.h"
#include "pattern.h"

using namespace std;

#define x first
#define y second
#define mp make_pair
#define pb push_back
char *string_to_chararr(string &s);

extern int positive_graph_count;
extern int negative_graph_count;
extern int candidate_size;
extern int iter_num;
extern int max_size;
extern double pfreq_threshold;
extern int test_samples;
extern vector<graph *> graph_database;
extern map<vector<int>, pattern *> pattern_code;
extern map<string, pattern *> edge_code;

float get_score(int pos_graph,int neg_graph);
void update_pattern_score(pattern *p);
void update_gids(bool isPositiveGraph, int currentGid, vector<int> &pgids, vector<int> &ngids);

// void fout_feature_matrix(vector<int> &code, int size, ofstream &fout);
// void fout_feature_matrix(int gid, ofstream &fout);
// void fout_feature_matrix(feature *f, ofstream &fout);

#endif	
