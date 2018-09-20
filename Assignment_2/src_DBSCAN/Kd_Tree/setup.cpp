#include "setup.h"
#include "KdTree.h"
#include "dbscan.h"

vector<int> points;
kd_tree_node *head;

void init_ds(int dim,int inst,float eps)
{
	if(dim>1)
	{
		head=setup_KdTree(dim,inst,eps);
	}
	else
	{
		
	}
}
void query_ds(int index,vector<int> &neighbour)
{

}

#endif