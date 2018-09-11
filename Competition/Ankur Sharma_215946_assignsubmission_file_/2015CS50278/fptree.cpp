#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <fstream>
#include <sstream>
#include <algorithm>

using namespace std;

int num_transactions;
float minsup;
map<int, int> global_order; //for ties
ofstream ofile;
map<int, int>* global_count_map;

class Node
{
public:
	int item;
	int count;
	vector<Node*> children;
	Node* parent; //add if needed
	
	Node()
	{
		item = -1;
	}
	Node(int id, Node* par, int c)
	{
		item = id;
		count = c;
		parent = par;
	}

};


bool sortFunction(const pair<int,int> first, const pair<int,int> second);
bool filterFunction(const pair<int, int> elem);
void createFPTree(vector<pair<int,int> > transactions, Node* tree, map<int, vector<Node*> >* headerTable);
void FPGrowth(vector<Node*> node_list, vector<int> candidate_set);
void printTree(Node node, int level);

bool sortFunction(const pair<int,int> first, const pair<int,int> second)
{
	return (first.second > second.second);
	//random comment to increase 1 line
}

bool newSortFunction(const pair<int,int> first, const pair<int,int> second)
{
	if((*global_count_map)[first.first] > (*global_count_map)[second.first])
		return true;
	else if((*global_count_map)[first.first] == (*global_count_map)[second.first]) //breaking ties
	{
		if(global_order[first.first] < global_order[second.first])
			return true;
	}
	return false;
}

bool filterFunction(const pair<int, int> elem) //change this acc to num_transactions??????????
{
	return ( ((elem.second * 1.0)/ num_transactions)  < minsup);
	// return ((float)((elem.second * 1.0) / num_transactions)  < minsup);
}

bool filterByMap(const pair<int, int> elem)
{
	return ( ((*global_count_map)[elem.first]*1.0)/num_transactions < minsup);
}


void printTree(Node node, int level)
{
	vector<Node*>::iterator it;
	for(it = node.children.begin(); it != node.children.end(); it++)
	{
		cout << "ON LEVEL " << level << endl;
		cout << (**it).item << ":" << (**it).count << endl;
		printTree(**it, level+1);
	}
	cout << endl;
}

void printItem(vector<int> items)
{
	for(int i = 0; i < items.size(); i++)
		ofile << items[i] << " ";
	ofile << endl;
}


int main(int argc, char* argv[])
{	
	if(argc !=4)
	{
		cout << "Please enter 3 arguments" << endl;
		exit(1);
	}

	string input_file, output_file;
	ifstream inFile;
	map<int,int> count_map;

	input_file = argv[1];
	minsup = (atoi(argv[2])*1.0) / 100;
	output_file = string(argv[3]) + ".txt";

	inFile.open(input_file);
	if(!inFile)
	{
		cout << "File doesn't exist" << endl;
		exit(1);
	}
	
	//first DB scan for getting 'flist' order
	// cout << "first db scan\n";
	num_transactions = 0;
	string line;
	while(getline(inFile, line))
	{
		num_transactions++;
		istringstream iss(line);
		for(string s; iss >> s; )
			count_map[stoi(s)]++;
	}
	inFile.close();

	vector<pair<int,int> > flist(count_map.begin(), count_map.end()); //copying map elements to vector
	flist.erase(remove_if(flist.begin(), flist.end(), &filterFunction ) , flist.end()); //removing those less than minsup
	sort(flist.begin(),flist.end(),&sortFunction); //sort by support

	//print flist
	// vector<pair<int,int> >::iterator it2;
	// for(it2 = flist.begin(); it2 != flist.end(); it2++)
	// {
	// 	cout << it2->first << ": " << it2->second << endl;
	// }

	//can this be improved??
	// cout << "GLOBAL ORDER\n";
	for(int i = 0; i < flist.size(); i++)
		global_order[flist[i].first] = i;

	global_count_map = &count_map;

	map<int,vector<Node*> > headerTable; //header table
	Node tree; //FP Tree

	//parse input and call fp tree
	// cout << "second db scan\n";
	inFile.open(input_file);
	int check = 1;
	while(getline(inFile, line))
	{
		// cout << "adding transaction " << check << endl;
		vector<pair<int,int> > transaction_items;
		istringstream iss(line);
		for(string s; iss >> s; )
		{
			int item = stoi(s);
			if( (count_map[item]*1.0 / num_transactions) >= minsup) //only add if sup > minsup
				transaction_items.push_back( make_pair(item, 1));
		}

		//sort by frequency in count_map
		// sort(transaction_items.begin(), transaction_items.end(), Local(count_map)); //
		sort(transaction_items.begin(), transaction_items.end(), &newSortFunction); //
		createFPTree(transaction_items, &tree, &headerTable);
		check++;
	}

	// cout << "TREE CREATED!!!" << endl;
	// printTree(tree, 1);
	ofile.open(output_file);
	for(int i = flist.size()-1; i >= 0; i--)
		FPGrowth(headerTable[flist[i].first], vector<int>());
	ofile.close();

}

void createFPTree(vector<pair<int, int> > transactions, Node* tree, map<int, vector<Node*> >* headerTable)
{
	Node* curNode = tree;
	vector<pair<int,int> >::iterator it;
	for(it = transactions.begin(); it != transactions.end(); it++)
	{
		vector<Node*>::iterator it1;
		bool found = false;
		for(it1 = (*curNode).children.begin(); it1 != (*curNode).children.end(); it1++)
		{
			if((**it1).item == (*it).first)
			{
				found = true;
				break;
			}
		}

		if(found) //increase count, change cur_node to this node -> no change to be made in headertable?
		{
			// cout << "Found\n";
			(**it1).count += (*it).second;
			curNode = *it1;
		}

		if(!found) //create a new child for curnode, change cur_node to the new node
		{
			Node* new_node = new Node( (*it).first, curNode, (*it).second );
			(*curNode).children.push_back(new_node);
			(*headerTable)[(*it).first].push_back(new_node);
			curNode = new_node;
		}
	}
}

void FPGrowth(vector<Node*> node_list, vector<int> candidate_set )
{
	int count = 0;

	for(int i = 0; i < node_list.size(); i++)
		count += node_list[i]->count;


	if( (count*1.0/num_transactions) >= minsup) //threshold for now --> change later
	{
		candidate_set.push_back(node_list[0]->item);
		printItem(candidate_set);
		map<int, int> item_count;
		
		// vector<vector<pair<int, int> > > all_transactions(node_list.size());
		//create a new flist (first scan) and store transactions
		for(int i = 0; i < node_list.size(); i++)
		{
			//keep going to parent till -1
			Node* next_item = node_list[i]->parent;
			while(next_item->item != -1)
			{
				item_count[next_item->item] += node_list[i]->count;
				// all_transactions[i].push_back(make_pair(next_item->item, node_list[i]->count) );
				next_item = next_item->parent;
			}
		}

		global_count_map = &item_count;

		vector<pair<int,int> > new_flist(item_count.begin(), item_count.end()); //copying map elements to vector
		new_flist.erase(remove_if(new_flist.begin(), new_flist.end(), &filterFunction ) , new_flist.end()); //removing those less than minsup
		sort(new_flist.begin(),new_flist.end(), &newSortFunction); //sort by support

		//print new flist
		// cout << "new flist " << endl;
		// for(int i = 0; i < new_flist.size(); i++)
		// {
		// 	cout << new_flist[i].first << ": " << new_flist[i].second << endl;
		// }

		//create new tree (second scan)
		Node new_tree;
		map<int, vector<Node*> > new_headerTable; //header table

		// for(int i = 0; i < node_list.size(); i++)
		// {
		// 	if(all_transactions[i].size() == 0) //is this nec?
		// 		continue;
		// 	vector<pair<int, int> > transactions = all_transactions[i];
		// 	transactions.erase(remove_if(transactions.begin(), transactions.end(), &filterByMap ) , transactions.end()); //removing those less than minsup
		// 	sort(transactions.begin(), transactions.end(), &newSortFunction);
		// 	createFPTree(transactions, &new_tree, &new_headerTable);
		// }

		for(int i = 0; i < node_list.size(); i++)
		{
			vector<pair<int, int> > transactions;
			Node* next_item = node_list[i]->parent;
			while(next_item->item != -1)
			{
				if( (item_count[next_item->item]*1.0/num_transactions) >= minsup)
				{
					transactions.push_back( make_pair(next_item->item, node_list[i]->count));
				}
				next_item = next_item->parent;
			}
			sort(transactions.begin(), transactions.end(), &newSortFunction);
			
			// cout << "printing next transaction\n";
			// for(int j = 0; j < transactions.size(); j++)
			// {
			// 	cout << transactions[j].first << ": " << transactions[j].second << ": " << global_order[transactions[j].first] << endl ;
			// }

			createFPTree(transactions, &new_tree, &new_headerTable);
		}



		// printTree(new_tree, 1);
		for(int i = new_flist.size()-1 ; i >= 0; i--)
		{
			FPGrowth(new_headerTable[new_flist[i].first],candidate_set);
		}

	}
}