#include "KdTree.h"

priority_queue<kd_tree_node*, vector<kd_tree_node*>, maxheapComparator> max_heap;
priority_queue<pair<double,int>, vector<pair<double,int> >, maxheapComparator2> naive_max_heap;
//Returns L^2 distance between 2 dataPoints
double getDistance(vd &a,vd &b)
{
	double dist=0,t;
	for(int i=0;i<DIMENSIONS;i++)
	{
		t = a[i] - b[i] ;
		dist += t * t;
	}
	return (dist);
}

double lowerbound(vd &mi, vd &ma, vd &a)
{
	double dist = 0;
	for (int i = 0; i < DIMENSIONS; ++i)
	{
		double t1=a[i]-mi[i],t2=a[i]-ma[i];
		t1=t1*t1;
		t2=t2*t2;
		double t=min(t1,t2);
		if(mi[i]<a[i] && a[i]<ma[i])
		t=0;		
		dist += t;
	}
	return (dist);
}

double getDistance_seq(vd &a, vd &b)
{
	double dist = 0;
	for (int i = 0; i < DIMENSIONS; i++)
		dist += (a[i] - b[i]) * (a[i] - b[i]);
	return sqrt(dist);
}

// returns true if a<b
bool lexioCompare(vd &a,vd &b)
{
	bool flag=true;
	for(int i=0;i<DIMENSIONS;i++)
	{
		if(a[i]<b[i])
			break;
		if(a[i]>b[i])
		{
			flag=false;
			break;
		}
	}
	return flag;
}

// Stores the result in global max_heap -> To access actual datapoints, pop nodes from heap and use node->Datapoint
void knn(kd_tree_node *root, vector<double> &q, int k)
{
	priority_queue<kd_tree_node*, vector<kd_tree_node*>, minheapComparator> min_heap;
	root->lowerbound = lowerbound(root->minRect, root->maxRect, q);
	// max_heap.clear();

	min_heap.push(root);

	while(!min_heap.empty())
	{
		// cerr<<min_heap.size()<<endl;
		kd_tree_node* r=min_heap.top();
		min_heap.pop();

		if(max_heap.size()>=k)
			if (r->lowerbound > max_heap.top()->distance)
				break;
		

		double distance = getDistance(r->Datapoint, q);
		r->distance=distance;	/*Setting distance for the node*/

		if (max_heap.size() < k)
		{
			max_heap.push(r);
		}
		else if (distance < max_heap.top()->distance)
		{
			max_heap.pop();
			max_heap.push(r);
		}
		//Added for lexiographic ordering in case of ties
		else if(distance==max_heap.top()->distance && lexioCompare(r->Datapoint,max_heap.top()->Datapoint))
		{
			max_heap.pop();
			max_heap.push(r);
		}

		kd_tree_node* left=r->lef;
		kd_tree_node* right=r->rig;

		if(left!=NULL)
		{
			left->lowerbound=lowerbound(left->minRect,left->maxRect,q);
			if (left->lowerbound <= max_heap.top()->distance)
				min_heap.push(left);
		}
		if(right!=NULL)
		{
			right->lowerbound = lowerbound(right->minRect, right->maxRect, q);
			if (right->lowerbound <= max_heap.top()->distance)
				min_heap.push(right);
		}
	}
}

// Stores the result in global naive_max_heap -> To access actual datapoints, elements from heap(node) and perform(Dataset[node.y])
void naive_knn(vector<double> &q, int k)
{
	int instances=Dataset.size();

	for(int i=0;i<instances;i++)
	{
		double dist=getDistance_seq(q,Dataset[i]);
		if(naive_max_heap.size()<k)
			naive_max_heap.push({dist,i});
		else
		{
			if(dist<naive_max_heap.top().x)
			{
				naive_max_heap.pop();
				naive_max_heap.push({dist,i});
			}
			else if(dist==naive_max_heap.top().x && lexioCompare(Dataset[i],Dataset[naive_max_heap.top().y]))
			{
				naive_max_heap.pop();
				naive_max_heap.push({dist,i});				
			}
		}
	}	
}