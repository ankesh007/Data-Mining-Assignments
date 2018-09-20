#ifndef KDTREE_H
#define KDTREE_H

#include <bits/stdc++.h>
#include "dbscan.h"

typedef vector<float> vf;

//Node Structure of KD Tree
struct kd_tree_node
{
	int Datapoint;
	int split_dim;
};

extern vector<vector<int> > DimSortedDataset; // Precomputed Once -> Stores sorted Pointers to Original Dataset
extern int DIMENSIONS; // To be set by Parser Function
extern int instances; // To be set by Parser Function

extern float eps;//Queries all points in eps radius Epsilon of query point  
extern float eps_2;//eps^2

kd_tree_node *setup_KdTree(int dimensions,int inst,float epsilon);
kd_tree_node *make_tree(vector<vector<int> > &DimSortedDataset,int dimCut);
void spatial_query_knn(kd_tree_node *node,int index,vector<int> &neighbour);//Assumes points are stored in global array

struct CustomComparator
{
	CustomComparator(int parameter)
	{
		this->parameter=parameter;
	}
	bool operator()(int i,int j)
	{
		return point_collection[i][parameter]<point_collection[j][parameter];
	}
	int parameter;
};

#endif