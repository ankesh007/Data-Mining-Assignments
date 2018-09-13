#include <bits/stdc++.h>
using namespace std;

map<int,int> counts;

char line[1000000];
int total_transactions = 0;

bool compare(int a,int b){
	map<int,int>::iterator it1,it2;
	it1 = counts.find(a);
	it2 = counts.find(b);
	if(it1->second == it2->second) return (it1->first < it2->first);
	return (it1->second < it2->second);
}
bool compare_out(vector<int>& a,vector<int>& b){
	sort(a.begin(),a.end());
	sort(b.begin(),b.end());
	return (a.size()<b.size())||(a.size()==b.size()&&a<b);
}
void fill_counts(char* filename){
	FILE* in_file = fopen(filename, "r");
	while ( fgets( line, 1000000, in_file ) != NULL ){
		istringstream iss(line); 
	    int a;
	    while (iss >> a)
	    {
	    	if(counts.find(a)==counts.end()) counts[a]=0;
	    	counts[a]++;
	    }
	    total_transactions++;
	}
	fclose(in_file);
}

class Node{
public:
	int id;
	int count;
	Node *parent;
	unordered_map<int, Node* > children;
	Node(int a){count = 1; id = a; parent = NULL;}
	Node(int a, int b){count = b; id = a; parent = NULL;}
};

void printtree(Node *root){

	queue<Node*> myq;
	myq.push(root);
	myq.push(NULL);

	while(!myq.empty()){
		Node *temp = myq.front();
		myq.pop();
		if(temp==NULL){
			cout << "\n";
			if(!myq.empty()) myq.push(NULL);
		}
		else{
			cout << temp->id << "(" << temp->count << "," << ((temp->parent!=NULL)?temp->parent->id:-2) << ")\t";
			for (auto it : temp->children)
			{
				myq.push(it.second);
			}
		}
	}
	cout << endl;
}

void printlist(unordered_map<int, vector<Node*> > &mylist){
	for (auto it : mylist)
	{
		cout << "list of " <<  it.first << " :: ";
		for (auto ele : it.second)
		{
			cout << ele->id << "(" << ele->count << "," << ((ele->parent!=NULL)?ele->parent->id:-2) << ")\t";
		}
		cout << endl;
	}
}

void insert_fptree(vector<int> &arr, Node *root, int ind, unordered_map<int, vector<Node*> > &mylist, int &basecount){

	while(ind > -1){
		unordered_map<int, Node*>::iterator it = root->children.find(arr[ind]);
		Node *child;
		if(it == root->children.end()){
			child = new Node(arr[ind], basecount);
			child->parent = root;
			root->children[arr[ind]] = child;
			mylist[arr[ind]].push_back(child);
		}
		else{
			child = it->second;
			child->count+=basecount;
		}
		root = child;
		ind--;
	}
}

void deleteall(Node *root){
	for (auto it : root->children)
	{
		deleteall(it.second);
	}
	root->children.clear();
	delete root;
	return;
}

void fptree_growth(unordered_map<int, vector<Node*> > &mylist, vector<int> &sorted_items, int ind, vector< vector<int> > &res, int& threshold){
	vector<Node*> templist = mylist[sorted_items[ind]];

	unordered_map<int,int> tempcountmap;
	for (auto ele : templist)
	{
		int basecount = ele->count;
		Node* tempnode = ele->parent;
		while(tempnode != NULL)
		{
			tempcountmap[tempnode->id] += basecount;
			tempnode = tempnode->parent;
		}
	}

	Node *root = new Node(-1);
	unordered_map<int, vector<Node*> > newlist;
	vector<int> rr;
	for (auto ele : templist)
	{
		Node* tempnode = ele->parent;
		while(tempnode != NULL){
			if(tempcountmap[tempnode->id] >= threshold && tempnode->id != -1) rr.push_back(tempnode->id);
			tempnode = tempnode->parent;
		}
		if(rr.size() == 0) continue;
		insert_fptree(rr, root, rr.size()-1, newlist, ele->count);
		rr.clear();
	}

	//printtree(root);
	//printlist(newlist);

	res.push_back(vector<int>(1,sorted_items[ind]));
	for (int i = ind-1; i > -1; --i)
	{
		while(i > -1 && newlist[sorted_items[i]].size()==0) i--;
		if(i==-1) continue;
		
		vector< vector<int> > tempres;
		fptree_growth(newlist, sorted_items, i, tempres, threshold);
	
		for (auto it : tempres)
		{
			it.push_back(sorted_items[ind]);
			//res.push_back(vector<int>(it.begin(), it.end()));
			res.push_back(it);
		}
		tempres.clear();

		vector<Node*> templist = newlist[sorted_items[i]];
		for (auto it : templist)
		{
			Node* temp = it;
			it->parent->children.erase(sorted_items[i]);
			delete temp;
		}
		templist.clear();
		newlist.erase(sorted_items[i]);
	}

	tempcountmap.clear();
	newlist.clear();
	deleteall(root);
	return;
}


void sort_and_insert(char* filename, int threshold, Node *root, unordered_map<int, vector<Node*> > &newlist){
	vector<int> trans;
	FILE* in_file = fopen(filename, "r");
	while ( fgets( line, 1000000, in_file ) != NULL ){
	    istringstream iss(line); 
	    int a;
	    trans.clear();
	    while (iss >> a)
	    {
	    	if(counts[a]>=threshold) trans.push_back(a);
	    }	
	    sort(trans.begin(),trans.end(),compare);
	    int temp = 1;
		insert_fptree(trans, root, trans.size()-1, newlist, temp);
	}
	fclose(in_file);
}

void run(char *filename, int threshold, bool flag){

	Node *root = new Node(-1);
	unordered_map<int, vector<Node*> > newlist;

	fill_counts(filename);
	threshold = ceil(((double)(total_transactions*threshold))/100);
	sort_and_insert(filename, threshold, root, newlist);

	vector< pair<int,int> > tempitems;
	for (auto it : counts)
	{
		tempitems.push_back(make_pair(it.second, it.first));
	}
	counts.clear();
	sort(tempitems.begin(), tempitems.end(), greater< pair<int,int> >());

	vector<int> items;
	for (int i = 0; i < tempitems.size(); ++i)
	{
		if(tempitems[i].first >= threshold) items.push_back(tempitems[i].second);
	}

	vector< vector<int> > res;
	
	for (int i = items.size()-1; i > -1; --i)
	{
		vector< vector<int> > tempres;
		fptree_growth(newlist, items, i, tempres, threshold);

		for (auto it : tempres)
		{
			res.push_back(it);
		}
		tempres.clear();

		vector<Node*> templist = newlist[items[i]];
		for (auto it : templist)
		{
			Node* temp = it;
			it->parent->children.erase(items[i]);
			delete temp;
		}
		templist.clear();
		newlist.erase(items[i]);
	}
	if(!flag){
		sort(res.begin(),res.end(),compare_out);
		for (int i = 0; i < res.size(); ++i)
		{
			for (int j = 0; j < res[i].size(); ++j)
			{
				cout << res[i][j] << " ";
			}
			cout << '\n';
		}
	}
}

int main(int argc, char *argv[]){
	if(argc != 3){
		cout << "Not enough arguments\n";
	}

	char *filename = argv[1];
	int threshold = atoi(argv[2]);

	if(threshold > 100)			//generate runtime data
	{
    	int x [] = {1,5,10,15,25,90};
    	for (int i =0; i<6; i++)
    	{
			clock_t begin = clock();

			run(filename, x[i], 1);

			clock_t end = clock();
			double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			cout << elapsed_secs << endl;
    	}
	}
	else run(filename, threshold, 0);
	return 0;
}