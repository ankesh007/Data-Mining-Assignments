#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>
#include <ctime>
#include <unordered_map>
#include <assert.h>
#include <algorithm>
#include <ostream>
#include <iterator>
#include <map>
#include <set>
#include <deque>
#include <queue>
#include <sstream>

using namespace std;

/* Number of transactions in transaction file */
int numTransactions;
/* Minimum support for a itemset to be frequent */
double minSup;

ofstream outc;
string outf;
bool debug = false;
bool plot;

struct Node{
    double item;
    int count;

    Node* parent;
    Node* next;
    map<int,struct Node*> children;

    Node():item(-1),count(0){
    };

    Node(int i,int c):item(i),count(c){
        next = nullptr;
        parent = nullptr;
    };
};

class FPTree{
    private:
        Node root;
        unordered_map<int,int> flist;
        vector<int> ilist;
        set<int> is;
        unordered_map<int,Node*> headerTable;
        bool flist_done;

    public:
        void printTree();

        //Constructor.
        FPTree();
        Node* getRoot();
        int getCount();

        //Add a transaction and modify the FPTree.
        void AddTrans(Node* root,vector<int> &trans,int k);

        //FPGrow function.Takes the filename to grow the fp tree rooted at root.
        void FPGrow(string filename);

        FPTree getConditionalTree(int item);

        void genItemSets(vector<int> &prior);
};

unordered_map<int,int> glist;

void Process(vector<int> s){
    for(auto &c:s){
        outc << c << " ";
    }
    outc << endl;
}

FPTree::FPTree(){
    root=Node();
    flist_done=false;
}

void FPTree::printTree(){
    queue<Node> q;
    q.push(root);
    while (!q.empty()) {
        auto top = q.front();
        q.pop();
        if (top.item == -2) {
            cout << endl;
            continue;
        }
        else if (top.item == -1)
            cout << top.item << ":" << top.count << endl;
        else
            cout << top.item << ":" << top.count << "\t";
        for (auto it: top.children) {
            q.push(*it.second);
        }
        Node temp = Node();
        temp.item = -2;
        q.push(temp);
    }
}

void FPTree::AddTrans(Node* root,vector<int> &trans,int k) {
    root->count+=k;
    if(trans.empty())
        return;
    auto i = trans.back();

    if(!flist_done){
        flist[i]+=k;
        if(is.find(i)==is.end()){
            ilist.push_back(i);
            is.insert(i);
        }
    }
    if(root->children.find(i)==root->children.end()){
        root->children[i] = new Node(i,0);     
        root->children[i]->parent = root;
        if(headerTable[i]==nullptr){
            headerTable[i]=root->children[i];
        } else {
            Node* hptr = headerTable[i];
            while(hptr->next!=nullptr)
                hptr = hptr->next;
            hptr->next=root->children[i];
        }
    }

    trans.pop_back();
    AddTrans(root->children[i],trans,k);
}

void FPTree::FPGrow(string filename){
    string line;
    ifstream input(filename, ios::in);
    int number;

    //Init the flist and ilist.
    flist_done = true;
    if (input.is_open()) {
        while (getline(input, line)) {
            numTransactions++;
            istringstream iss(line);
            while (iss >> number) {
                flist[number]++;
                if(is.find(number)==is.end()){
                    ilist.push_back(number);
                    is.insert(number);
                }
            }
        }
        input.close();
    }
    ifstream input2(filename, ios::in);
    glist = (flist);
    if(input2.is_open()) {
        while (getline(input2, line)) {
            istringstream iss(line);
            vector<int> trans;
            while (iss >> number) {
                if((flist[number])>=(minSup*numTransactions)/100)
                    trans.push_back(number);
            }
            if(trans.empty())
                continue;
            sort(trans.begin(),trans.end(),[](const int& i1, const int& i2) {
                if(glist[i1]!=glist[i2]){
                    return glist[i1]<glist[i2];
                }
                return i1<i2;
            });
            if(debug){
                // cout<<"trans"<<endl;
                for(auto&t:trans)
                    cout<<t<<" ";
                cout<<endl;
            }
            AddTrans(&root,trans,1);
        }
    }
}

Node* FPTree::getRoot(){
    return &root;
}

FPTree FPTree::getConditionalTree(int item){
    FPTree CondTree = FPTree();
    Node* nextptr = headerTable[item];
    while(nextptr!=nullptr){
        vector<int> branch;
        Node* headptr = nextptr;
        int k=nextptr->count;
        headptr=headptr->parent;
        while(headptr->item!=-1){
            branch.push_back(headptr->item);
            headptr=headptr->parent;            
        }
        if(debug){
            // cout<<"Branch\n";
            for(auto&b:branch)
                cout<<b<<" ";
            cout<<endl;
        }
        CondTree.AddTrans(CondTree.getRoot(),branch,k);
        nextptr=nextptr->next;
    }
    return CondTree;
}

int FPTree::getCount() {
    return root.count;
}

void FPTree::genItemSets(vector<int> &prior){
    set<int> proccesed;
    sort(ilist.begin(),ilist.end(),[this](const int& i1, const int& i2) {
        if(flist[i1]!=flist[i2]){
            return flist[i1]<flist[i2];
        }
        return i1<i2;
    });
    for(const auto &iter: ilist) {
        if(proccesed.find(iter)==proccesed.end()){
            if((flist[iter])<(minSup*numTransactions)/100){
                continue;
            }
            FPTree CondTree = getConditionalTree(iter);
            if(debug)
                cout<<iter<<":"<<CondTree.getCount()<<endl;
                
            prior.push_back(iter);
            Process(prior);
            CondTree.genItemSets(prior);
            prior.pop_back();
        }
        proccesed.insert(iter);
    }
}

void fptreeGo(string filename, string outputFileName, double minimumSupport){
    numTransactions = 0;
    outf = outputFileName;
    minSup = minimumSupport;
    outc.open(outf);

    clock_t begin;
    if (plot)
        begin = clock();
    
    FPTree tree = FPTree();
    tree.FPGrow(filename);
    //tree.printTree();
    vector<int> prior;
    tree.genItemSets(prior);

    outc.close();
    if (plot) {
        clock_t end = clock();
        double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
        // cout << "Execution time is: " << elapsed_secs << endl;
        cout << elapsed_secs << endl;
    }
}

int main(int argc, char** argv) {
    ios_base::sync_with_stdio(false); cin.tie(NULL);
    string transactionFileName(argv[1]);
    double minimumSupport= atof(argv[2]);    
    string outputFileName(argv[3]);
    outputFileName += ".txt";
    plot = atoi(argv[4]) == 1 ? true : false;
    fptreeGo(transactionFileName, outputFileName, minimumSupport);
    return 0;
}
