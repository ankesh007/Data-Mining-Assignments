#include <bits/stdc++.h>
#define si set<int>

using namespace std;

typedef struct node
{
	int item; 
	int count; 
	map<int, node *> children;
	node *parent;
}node;


map<int,int> all_items;
int num_transactions;
double min_sup_trans;
map<int,int> name_change;
map<int,int> inv_name_change;
map<int, vector<node*> > global_chain;
vector<si> frequentItemSets;
ofstream myfile;
string output_file;

void debug_set(si& sett){
	for(si::iterator it=sett.begin();it!=sett.end();++it){
		myfile << *it << " ";
	}
	myfile << "\n";
}

void debug_map(map<int,int> mapp){
	for(map<int,int>::iterator it=mapp.begin(); it!=mapp.end(); ++it){
		cout << it->first << " " << it->second<<", ";
	}
	cout << endl;
}

void debug_tree(node* curr_node){
	cout << curr_node->item << " " << curr_node->count<<endl;
	for(map<int,node*>::iterator it=curr_node->children.begin(); it!=curr_node->children.end(); ++it){
		debug_tree(it->second);
	}
}

node* make_node(int item, node* parent){
	node *new_node = new node;
	new_node->item = item;
	new_node->count = 0;
	new_node->parent = parent;
	return new_node;
}

void first_pass(string file_name, double supp_thresh){
	ifstream in(file_name);
	while(in) {
		string line;
    	getline(in,line);
    	istringstream words(line);
    	for(int item; words >> item; ){
    		if(all_items.count(item)>0)
    			all_items[item] +=1;
    		else
    			all_items[item] = 1;
    	}
    	num_transactions+=1;
  	}
  	min_sup_trans = supp_thresh * num_transactions;
  	in.close();
  	vector <pair<int,int> > sorted_items;  	
	for (map<int,int>::iterator it=all_items.begin(); it!=all_items.end(); ++it){
		if(it->second >= min_sup_trans){
			sorted_items.push_back(make_pair(it->second,it->first));
		}
	}
	sort(sorted_items.begin(),sorted_items.end(),greater<pair<int, int> >());
	for(int i=0;i<sorted_items.size();i++){
		name_change[sorted_items[i].second] = (i+1);
		inv_name_change[(i+1)] = sorted_items[i].second;
	}
}

void add_transaction(node* root, si& transaction, map<int, vector<node*> >& linear_chain, int factor){
	node *curr_node = root;
	for(si::iterator it=transaction.begin();it!=transaction.end();++it){
		if(curr_node->children.count(*it)==0){
			curr_node->children[*it] = make_node(*it, curr_node);
			linear_chain[*it].push_back(curr_node->children[*it]);
		}
		curr_node->children[*it]->count +=factor;
		curr_node = curr_node->children[*it];
	}
}

void make_tree(string file_name, node* root){
	ifstream in(file_name);
	while(in) {
		string line;
    	getline(in,line);
    	istringstream words(line);
    	si transaction;
    	for(int item; words >> item; ){
    		if(name_change.count(item)>0)
    			transaction.insert(name_change[item]);
    	}
		add_transaction(root, transaction, global_chain, 1);    	
  	}
  	in.close();
}


void fp_growth(node* root, map<int, vector<node*> >& header_table, si exist_pattern){
	for(map<int, vector<node*> >::reverse_iterator it=header_table.rbegin();it!=header_table.rend();++it){
		int new_elem = it->first;
		vector<node*> my_list = header_table[new_elem];

		exist_pattern.insert(inv_name_change[new_elem]);
		frequentItemSets.push_back(exist_pattern);
		debug_set(exist_pattern);
		
		map<int, vector<node*> > conditional_header_table;
		node *cond_root = make_node(0, NULL);
		map<int, int> cond_elem_count;
		for(int i=0;i<my_list.size();i++){
			node *curr_node = my_list[i];
			while(curr_node->parent){
				curr_node = curr_node->parent;
				if(curr_node->item!=0){
					int item_no = curr_node->item;
					if(cond_elem_count.count(item_no)==0){
						cond_elem_count[item_no] = my_list[i]->count;
					}
					else{
						cond_elem_count[item_no] += my_list[i]->count;
					}
				}			
			}
		}

		for(int i=0;i<my_list.size();i++){
			si elems;
			node *curr_node = my_list[i];
			while(curr_node->parent){
				curr_node = curr_node->parent;
				if(curr_node->item!=0){
					int item_no = curr_node->item;
					if(cond_elem_count[item_no]>=min_sup_trans){
						elems.insert(item_no);
					}
				}			
			}
			add_transaction(cond_root, elems, conditional_header_table, my_list[i]->count);
		}
		fp_growth(cond_root, conditional_header_table, exist_pattern);
		exist_pattern.erase(inv_name_change[new_elem]);
	}
}






int main(int argc,char* argv[]){
	string file_name = argv[1];
	double supp_thresh = atof(argv[2])/100.0;
	output_file = string(argv[3]) + ".txt";
	myfile.open(output_file);
	first_pass(file_name, supp_thresh);
	node *root = make_node(0, NULL);
	make_tree(file_name, root);
	si item_set;
	fp_growth(root, global_chain, item_set);
	cout << frequentItemSets.size() <<endl;
	myfile.close();
}