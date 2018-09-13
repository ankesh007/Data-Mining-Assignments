#include <iostream>
#include <vector>
#include <algorithm> 
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <time.h>
#include <chrono>
using namespace std;
unordered_map <int,int> item_count;
unordered_map<int,struct node*> header_table;

struct node *root;
bool pair_count_comparator(pair <int,int> a,pair <int,int> b){ 
	if(a.second>b.second)
		return true;
	else if(a.second<b.second)
		return false;
	else
		return a.first>b.first;
}
bool string_count_comparator(int a,int b){ 
	if(item_count[a]>item_count[b])
		return true;
	else if(item_count[a]<item_count[b])
		return false;
	else
		return (a>b);
}
int min_support;
struct node{
	int item;
	int prefix_count;
	unordered_map<int,struct node*> child;
	struct node* header_next;
	struct node* parent;
	int index;
};
void add_to_header_table(int s,struct node* element)
{
	struct node *p=header_table[s];
	if(p==NULL){		
		header_table[s] = element;
		return;
	}
	while(p->header_next!=NULL)
	{
		p=p->header_next;
	}
	p->header_next=element;
}

void ascend_tree(struct node* node,vector<int> &vect,vector<pair<struct node*, int> > &prefixes){
	struct node *iter=node;
	int previous_count=iter->prefix_count;
	int up=iter->item;
	iter=iter->parent;
	while(iter->item!=-1){
		if(vect[iter->index]==0){
			vect[iter->index]=1;
			pair<struct node*, int> p;
			p.first=iter;
			p.second=iter->prefix_count;
			prefixes.push_back(p);
			iter->prefix_count=previous_count;
		}
		else{
			iter->prefix_count+=previous_count;
		}
		// previous_count=iter->prefix_count;
		iter=iter->parent;
	}
}

int get_count(int item,vector<int> &vect)
{
	struct node *node_p=header_table[item];
	int count=0;
	while(node_p!=NULL){
		if(vect[node_p->index]==1)
		{
			count+=node_p->prefix_count;
		}
		node_p=node_p->header_next;
	}
	return count;
}

void draw_conditional_tree(vector<int> &p,vector<int> &useful_item_vector,int check_useful,vector<vector <int> > &result,vector<int> &vect)
{
	// to be optimized
	int last_node=p[p.size()-1];
	vector<int> valid(vect.size());
	vector< pair<struct node*, int> > prefixes; 
	for(int i=0;i<valid.size();i++){
		valid[i]=0;
	}
	struct node *node_p = header_table[last_node];
	vector<pair <int,int> > item_vector;
	while(node_p!=NULL)
	{
		if(vect[node_p->index]==1){
			ascend_tree(node_p,valid,prefixes);
		}
		node_p=node_p->header_next;
	}
	vector<int> useful;
	for(int i=0;i<check_useful;i++){
		int a=get_count(useful_item_vector[i],valid);
		if(a>=min_support){
			useful.push_back(useful_item_vector[i]);
		}
	}
	int i=useful.size()-1;
	while(i>=0)
	{
		vector<int> c=p;
		c.push_back(useful[i]);
		result.push_back(c);
		useful.resize(useful.size()-1);
		draw_conditional_tree(c,useful,i,result,valid);
		i--;
	}
	for(int jj=0;jj<prefixes.size();jj++)
	{
		prefixes[jj].first->prefix_count=prefixes[jj].second;
	}
}

int main(int argc, char ** argv){
	time_t timer;
	time(&timer);
	auto start = chrono::high_resolution_clock::now();
	string infilename=argv[1];
	float x=atof(argv[2]);
	string outfilename=argv[3];
	int num_transactions = 0;
	ifstream inputfile;
	inputfile.open(infilename);
	while (inputfile)
	{
	    string s;
	    if (!getline( inputfile, s )) 
			break;
		num_transactions++;
	    istringstream ss( s.c_str() );
	    while (ss){
			string s;
			if (!getline( ss, s, ' ' )) 
				break;
			int k = stoi(s);
			if(item_count.find(k)==item_count.end()){
				item_count[k]=1;
			}
			else{
				item_count[k]+=1;
			}
	    }
	}

	vector<pair <int,int> > item_vector;
	for(auto& x : item_count)
	{
		pair<int,int> p(x.first,x.second);
		item_vector.push_back(p);
	}
	min_support=x*num_transactions/100;
	sort(item_vector.begin(),item_vector.end(),pair_count_comparator);
	vector<int> useful_item_vector;
	for(int i=0;i<item_vector.size();i++){
		if(item_vector[i].second<min_support) break;
		header_table[item_vector[i].first]= NULL;
		useful_item_vector.push_back(item_vector[i].first);
	}
	root=new struct node();
	root->item=-1;

	inputfile.clear();
	inputfile.seekg(0, ios::beg);
	// fseek ( &inputfile , 0 , SEEK_SET );
	int index=0;
	while (inputfile)
	{
	    string s;
	    if (!getline( inputfile, s )) break;
	    // // cout<<
	    struct node *previous=root;
	    istringstream ss( s.c_str() );
	    vector<int> record;
	    while (ss)
	    {
	      string s;
	      if (!getline( ss, s, ' ' )) break;
		  int k = stoi(s);
	      record.push_back(k);
	    }
	    sort(record.begin(),record.end(),string_count_comparator);
	    for(int i=0;i<record.size();i++){
	    	if(item_count[record[i]]<min_support) break;
	    	if(previous->child.find(record[i])==previous->child.end()){
		    	struct node *new_node=new node;
		    	new_node->item=record[i];
		    	new_node->prefix_count=1;
		    	new_node->header_next=NULL;
		    	add_to_header_table(record[i],new_node);
		    	new_node->parent=previous;
		    	new_node->index=index++;
				previous->child[record[i]]=new_node;
			}
			else{
				previous->child[record[i]]->prefix_count+=1;
			}
			previous=previous->child[record[i]];

	    }
	}
	// Done construction
	time_t timer2;
	time(&timer2);
	double seconds2 = difftime(timer2,timer);
	// cout<<"Construction Done at - "<<seconds2<<endl;	
	vector<int> valid(index);
	for(int i=0;i<valid.size();i++){
		valid[i]=1;
	}
	vector<vector<int> > result;
	int i=useful_item_vector.size()-1;
	while(i>=0){
		vector<int> p;
		p.push_back(useful_item_vector[i]);
		result.push_back(p);
		// useful_item_vector.resize(useful_item_vector.size()-1);
		draw_conditional_tree(p,useful_item_vector,i,result,valid);
		i--;
	}
	time_t timer3;
	time(&timer3);
	double seconds1 = difftime(timer3,timer);
	// cout<<"Write Start at - "<<seconds1<<endl;	
	ofstream outputfile;
	outputfile.open(outfilename);
	for(int i1=0;i1<result.size();i1++)
	{
		for(int j=result[i1].size()-1;j>=0;j--)
		{
			outputfile<<result[i1][j]<<" ";
		}
		outputfile<<endl;
	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	float p=duration.count();
	cout<<p/1000000<<endl;
}