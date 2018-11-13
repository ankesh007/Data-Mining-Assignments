/* 
 * File:   graph.h
 * Author: njin
 *
 * Created on October 10, 2009, 6:58 PM
 */

#ifndef _GRAPH_H
#define	_GRAPH_H

#include <string>
#include <vector>
using namespace std;

//class: graph
/*stores the information of a input graph, including: string information
 about this graph, all the nodes and all the edges*/
class graph {
public:
    string info;    //stores string information about this graph, not used for subgraph mining
    vector<short> nodes;    //list of nodes and their labels in a graph
    vector<vector<pair<short, short> >*> adjList; /*adjacency list of each node; for each pair:
												   first=nid, second=edge label*/
    ~graph();	//destructor
};

#endif	/* _GRAPH_H */

