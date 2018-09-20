#include "KdTree.h"
#include "dbscan.h"
/*
Note that this KD-Tree implementation has been taken from:
https://github.com/ankesh007/Kd-Tree

This has been adapted to suite task of radius queries(spatial queries)
It assumes that an array --> point_collection is accessible which given an index returns a point 
This array is declared in "dbscan.h" file and defined in "dbscan.cpp" file 
Such optimizations are needed as this was class competition assignment
*/

vector<vector<int> > DimSortedDataset; // Precomputed Once -> Stores sorted Pointers to Original Dataset
int DIMENSIONS; // To be set by Parser Function
float eps;//Queries all points in eps radius eps of query point  
float eps_2;//eps^2
int instances;

kd_tree_node *setup_KdTree(int dimensions,int inst,float epsilon)
{
	DIMENSIONS=dimensions;
	instances=inst;
	eps=epsilon;
	eps_2=eps*eps;

	DimSortedDataset.resize(DIMENSIONS);

	for (int i = 0; i < DIMENSIONS; i++)
	{
		DimSortedDataset[i].resize(instances);
		for(int j=0;j<instances;j++)
		{
				DimSortedDataset[i][j]=j;
		}
		sort(DimSortedDataset[i].begin(),DimSortedDataset[i].end(),CustomComparator(i));
		// https://stackoverflow.com/questions/4066576/passing-a-parameter-to-a-comparison-function
	}
	return make_tree(DimSortedDataset,0);
}

// Receives datapoints and splitDimension and Returns Root Node
kd_tree_node *make_tree(vector<vector<int> > &DimSortedDataset,int dimCut)
{
	if(DimSortedDataset[0].size()==0)
		return NULL;
	//Empty Tree -> NULL
	
	int instances=DimSortedDataset[0].size();	/* Points this Node Represents*/
	// kd_tree_node *node=(kd_tree_node *)malloc(sizeof(kd_tree_node));
	kd_tree_node *node = new kd_tree_node;

	vector<vector<int> > lefData,rigData;
	lefData.resize(DIMENSIONS);
	rigData.resize(DIMENSIONS);

	int medianDatapoint=-1;
	double medianVal=-1;

	// Deciding Median
	medianDatapoint=DimSortedDataset[dimCut][instances/2];
	medianVal=point_collection[DimSortedDataset[dimCut][instances/2]][dimCut];

	// node->Datapoint=Dataset[medianDatapoint];	/*Setting Node Datapoint*/
	node->Datapoint=medianDatapoint;
	node->split_dim=dimCut;
	// Divinding datapoints into 2 disjoint multiset
	for(int i=0;i<DIMENSIONS;i++)
	{
		for(int j=0;j<instances;j++)
		{
			int originalIndex=DimSortedDataset[i][j];
			if(originalIndex==medianDatapoint)
				continue;
			else if(Dataset[originalIndex][dimCut]<=medianVal)
				lefData[i].pb(originalIndex);
			else
				rigData[i].pb(originalIndex);
		}
	}

	node->lef=make_tree(lefData,(dimCut+1)%DIMENSIONS);
	node->rig=make_tree(rigData,(dimCut+1)%DIMENSIONS);
	return node;
}

//Returns L^2 distance between 2 dataPoints
float getDistance(int i,int j)
{
	double dist=0,t;
	for(int i=0;i<DIMENSIONS;i++)
	{
		t = point_collection[i] - point_collection[j] ;
		dist += t * t;
	}
	return (dist);
}

void spatial_query_knn(kd_tree_node *node,int index,vector<int> &neighbour)//Assumes points are stored in global array
{
	if(node->left==NULL && node->right==NULL)
	{
		if(getDistance(node->Datapoint,index)<eps_2)
			neighbour.pb(node->Datapoint);
		return;
	}
	float split_dim_val=point_collection[node->Datapoint][node->split_dim];
	float point_val_at_same_dim=point_collection[index][node->split_dim];

	if(node->left!=NULL && (point_val_at_same_dim-eps < split_dim_val))
	{
		spatial_query_knn(node->left,index,neighbour);
	}
	if(node->right!=NULL && (point_val_at_same_dim+eps > split_dim_val))
	{
		spatial_query_knn(node->right,index,neighbour);
	}
}
