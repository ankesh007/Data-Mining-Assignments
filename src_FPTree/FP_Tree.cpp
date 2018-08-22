#include "FP_Tree.h"

double CUT_OFF_PERCENT=0;
int TOTAL_TRANSACTIONS=0;
int REQUIRED_SUPPORT_COUNT=0;
int TOTAL_ITEMS=0;
int TOTAL_NODES=0;

map<int,int> item_idx;
vector<int> idx_item;

vvi frequent_patterns;//final Answer

vi path_count; // Keeps count of Number of paths through this item
vi node_to_item;
vector<map<int,int> > track_children_items;  // Keeps track of all children of node - maps item to Node Number
vector<int> parent_node;
vvi header_table;

int idx=0;
vi prefix_frequent_itemset;

void calculate_support()
{
	double count= CUT_OFF_PERCENT*TOTAL_TRANSACTIONS;
	count/=100;
	REQUIRED_SUPPORT_COUNT=ceil(count);
}

/*
	Used for sorting <item,count> pair in decreasing order of count
*/
bool COMPARATOR(pii a,pii b)
{
	if(a.y!=b.y)
		return a.y>b.y;
	return a.x<b.x;
}

/*
	Used for sorting itemsets
*/
bool COMPARATOR2(int a,int b)
{
	return a<b;
}

void growFP_Tree(vi &itemset)
{
	int cur_node=0;

	for(auto itr:itemset)
	{
		int next_node=-1;

		if(track_children_items[cur_node].find(itr)==track_children_items[cur_node].end())
		{
			track_children_items[cur_node][itr]=TOTAL_NODES;//Create a new child of Parent
			parent_node.pb(cur_node);//Assign parent to child
			header_table[itr].pb(TOTAL_NODES);//Update header Pointers
			node_to_item.pb(itr);//Reverse Mapping
			next_node=TOTAL_NODES;
			TOTAL_NODES++;
			track_children_items.resize(TOTAL_NODES);
			path_count.pb(0);
		}
		else
			next_node=track_children_items[cur_node][itr];
		
		cur_node=next_node;
		path_count[cur_node]++;
	}
}

void printTree(int root)
{
	//cout<<root<<" "<<path_count[root]<<" "<<node_to_item[root]<<endl;
	for(auto itr:track_children_items[root])
	{
		//cout<<itr.x<<" "<<itr.y<<endl;
		printTree(itr.y);
	}
}

void constructFP_Tree(char *filename)
{
	string line;
	vpii count_Transactions;
	vi local_idx_item;
	map<int,int> local_item_idx;
	int temp_count=-1;

/*
	First Pass over dataset
	Setup Frequency table for items
	Required to order the items in itemset
*/

	ifstream read_input;
	read_input.open(filename);
    string tokens;

	while(getline(read_input,line))
	{
		TOTAL_TRANSACTIONS++;

		stringstream check1(line);
	    while(getline(check1, tokens, ' '))
	    {
	    	if(tokens.length()==0)
	    		continue;
	    	int item_num=atoi(tokens.c_str());
	    	if(local_item_idx.find(item_num)==local_item_idx.end())
	    	{
	    		temp_count++;
	    		local_item_idx[item_num]=temp_count;
	    		local_idx_item.pb(item_num);
	    		count_Transactions.pb({temp_count,1});
	    	}
	    	else
	    	{
	    		count_Transactions[local_item_idx[item_num]].y++;
	    	}
	    }
    }
	read_input.close();
	calculate_support();
	#ifdef DEBUG
		//cout<<"First Pass Over"<<endl;
 	#endif

/*
	Filtering Items below Support Threshold
	Giving them idxes based on their count
	0-> Highest Count
*/
	sort(count_Transactions.begin(),count_Transactions.end(),COMPARATOR);
	for(auto itr:count_Transactions)
	{
		if(itr.y<REQUIRED_SUPPORT_COUNT)
			break;
		idx_item.pb(local_idx_item[itr.x]);
		item_idx[local_idx_item[itr.x]]=TOTAL_ITEMS;
		TOTAL_ITEMS++;
	}
	#ifdef DEBUG
		//cout<<"Filtering Over"<<endl;
 	#endif


/*
	Setup Data Structures for FP_Tree
	Initialise Header Table
	Make a Root Node
*/	
	header_table.resize(TOTAL_ITEMS);
	parent_node.pb(-1);
	track_children_items.resize(1);
	node_to_item.pb(-1);
	path_count.pb(0);
	TOTAL_NODES++;
/*
	Perform second pass over the data
	Grow FP_Tree simultaneously
*/
	read_input.open(filename);
	vi itemset;

	while(getline(read_input,line))
	{
		stringstream check1(line);
		itemset.resize(0);

	    while(getline(check1, tokens, ' '))
	    {
	    	if(tokens.length()==0)
	    		continue;
			int item_num=atoi(tokens.c_str());

	    	if(item_idx.find(item_num)==item_idx.end())
	    		continue;
	    	itemset.pb(item_idx[item_num]);
	    }
	    if(itemset.size()==0)
	    	continue;
	    sort(itemset.begin(),itemset.end(),COMPARATOR2);

    #ifdef DEBUG
		//cout<<"Begin grow"<<endl;
	#endif
	    growFP_Tree(itemset);
    #ifdef DEBUG
		//cout<<"End Grow"<<endl;
	#endif
    }

	read_input.close();
	#ifdef DEBUG
		//cout<<"Second pass over"<<endl;
 	#endif
}

void augmentFP_set()
{
	vi frequent_item;
	int x=frequent_patterns.size();
	frequent_patterns.pb(frequent_item);
	frequent_patterns[x].resize(idx);
	for(int i=0;i<idx;i++)
		frequent_patterns[x][i]=prefix_frequent_itemset[i];	
}

map<int,int> global_counter; //maps modified item number to their count in conditional tree
map<int,int> new_header_helper;// stores filtered nodes
map<int,int> reset_helper;// an auxillary map - can be removed potentially

void reset_path(int node,vpii &storeFP_counts)
{
	int temp=path_count[node];
	node=parent_node[node];

	while(node)
	{
		if(reset_helper.find(node)==reset_helper.end())
		{
			reset_helper[node]=1;
			storeFP_counts.pb({node,path_count[node]});
			path_count[node]=temp;
		}
		else
		{
			path_count[node]+=temp;
		}
		global_counter[node_to_item[node]]+=temp;
		node=parent_node[node];
	}
}

void createHeaderTable(int node,vvi &newHeaderTable,vpii &storeFP_parents)
{
	node=parent_node[node];
	while(node)
	{
		if(new_header_helper.find(node_to_item[node])!=new_header_helper.end())
		{
			int x=new_header_helper[node_to_item[node]];
			if(reset_helper.find(node)==reset_helper.end())
			{
				reset_helper[node]=1;
				newHeaderTable[x].pb(node);
				int temp=parent_node[node];
				while(temp && new_header_helper.find(node_to_item[temp])==new_header_helper.end())
				{
					temp=parent_node[temp];
				}
				if(temp!=parent_node[node])
				{
					storeFP_parents.pb({node,parent_node[node]});
					parent_node[node]=temp;
				}
			}
		}
		node=parent_node[node];
	}
}

void dfs_mineFP(vi &conditional_leaves)
{
	augmentFP_set();

	vpii storeFP_counts;
	vpii storeFP_parents;

	global_counter.clear();
	new_header_helper.clear();
	reset_helper.clear();

	for(auto itr:conditional_leaves)
	{
		reset_path(itr,storeFP_counts);
	}

	int newHeaderCount=0;
	vi temp_nodes;//Stores item values

	for(auto itr:global_counter)
	{
		if(itr.y<REQUIRED_SUPPORT_COUNT)
			continue;
		new_header_helper[itr.x]=newHeaderCount;
		newHeaderCount++;
		temp_nodes.pb(itr.x);
	}
	vvi newHeaderTable;
	newHeaderTable.resize(newHeaderCount);

	reset_helper.clear();
	for(auto itr:conditional_leaves)
	{
		createHeaderTable(itr,newHeaderTable,storeFP_parents);
	}
	int x=newHeaderTable.size();
	for(int i=x-1;i>=0;i--)
	{
		prefix_frequent_itemset[idx]=temp_nodes[i];
		idx++;
		dfs_mineFP(newHeaderTable[i]);
		idx--;
	}

	for(auto itr:storeFP_counts)
	{
		path_count[itr.x]=itr.y;
	}
	for(auto itr:storeFP_parents)
	{
		parent_node[itr.x]=itr.y;
	}
}

void mine_FrequentPatterns()
{
	#ifdef DEBUG
		//cout<<"Hey"<<endl;
 	#endif
	prefix_frequent_itemset.resize(TOTAL_ITEMS);

	for(int i=TOTAL_ITEMS-1;i>=0;i--)
	{
	#ifdef DEBUG
		//cout<<i<<endl;
 	#endif
		prefix_frequent_itemset[idx]=i;
		idx++;
		dfs_mineFP(header_table[i]);
		idx--;
	}
}

void print_FrequentPatterns(char *filename)
{
	ofstream write_output;
	write_output.open(filename);
	for(auto itr:frequent_patterns)
	{
		for(auto itr2:itr)
		{
			write_output<<idx_item[itr2]<<" ";
		}
		write_output<<"\n";
	}
	write_output.close();
}

// void sumTree()
// {
// 	for(auto itr:header_table)
// 	{
// 		int sum=0;
// 		for(auto itr2:itr)
// 			sum+=path_count[itr2];
// 		//cout<<sum<<endl;
// 	}
// }

int main(int argc,char **argv)
{
	CUT_OFF_PERCENT=atof(argv[2]);
	constructFP_Tree(argv[1]);
#ifdef SIMPLE_DEBUG
	//cout<<"Constructed FP Tree"<<endl;
#endif
	mine_FrequentPatterns();
#ifdef SIMPLE_DEBUG
	//cout<<"Mined FP"<<endl;
#endif
	print_FrequentPatterns(argv[3]);
#ifdef SIMPLE_DEBUG
	//cout<<"End"<<endl;
#endif
	return 0;
}