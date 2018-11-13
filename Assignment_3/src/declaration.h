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
extern int max_size = 1000;
extern double pfreq_threshold = 0;
extern int test_samples = 0;
extern vector<graph *> graph_database;
extern map<string, pattern *> pattern_code;

float get_score(int pos_graph,int neg_graph);
void update_pattern_score(pattern *p);
#endif	