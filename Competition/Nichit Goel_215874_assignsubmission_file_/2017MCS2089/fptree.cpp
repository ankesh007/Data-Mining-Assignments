#include<bits/stdc++.h>
#include<ext/pb_ds/assoc_container.hpp>
using namespace std;
using namespace __gnu_pbds;
#define MAX_LINE_LENGTH 1048576


/** Node in FP Tree **/
struct Node {
	int id, cnt, parent;
	gp_hash_table<int, int> children;
};

/** FP Tree structure **/
struct FPTree {
	vector<Node> nodes;
	gp_hash_table<int, vector<int> > header;
	gp_hash_table<int, int> freq;
	vector<pair<int,int> > flist;
};

FILE *infile, *outfile;
int n, support;
static char itemset[MAX_LINE_LENGTH];
static char outline[MAX_LINE_LENGTH];

/** Utilities **/
clock_t sttime;
void startt() {
    sttime=clock();
}

double runtime() {
    return double(clock()-sttime)/CLOCKS_PER_SEC;
}

bool mark;
inline bool getnextint(char *fline, int &x, int &in) {
	in = 0;
	mark = false;
	while (fline[x]) {
		if (fline[x] >= '0' && fline[x] <= '9') {
			in *= 10; in += fline[x]-'0';
			mark = true;
		}
		else {
			if (mark) return true;
		}
		x++;
	}
	return false;
}

inline void putnextint(char *pline, int &x, int in) {
	char ti[10];
	char *p = ti;
	do {
		*p++ = char(in % 10) + '0'; in /= 10;
	} while (in > 0);
	do {
		pline[x++] = *--p;
	} while (p != ti);
	pline[x++] = ' ';
}

/** Debugging print out **/
void dfs(FPTree &tree, int at) {
	printf(" At %d->%d (id = %d) = %d\n",tree.nodes[at].parent,at,tree.nodes[at].id,tree.nodes[at].cnt);
	for (auto it=tree.nodes[at].children.begin(); it!=tree.nodes[at].children.end(); it++) {
		dfs(tree, it->second);
	}
}

void debug_fp(FPTree &tree, gp_hash_table<int, null_type> freqset) {
	printf("Entering fp_growth:\n");
	dfs(tree, 0);
	for (int i=0; i<tree.flist.size(); i++) {
		int id = tree.flist[i].first;
		printf("ID %d:",id);
		for (int j=0; j<tree.header[id].size(); j++) {
			printf(" %d",tree.header[id][j]);
		}
		printf("\n");
	}
	printf("Frequent set = ");
	for (auto it=freqset.begin(); it!=freqset.end(); it++) {
		printf("%d ",(*it));
	}
	printf("\n\n");
}

/** Main function **/
bool freqcmp(pair<int,int> &item_a, pair<int,int> &item_b) {
	if (item_a.second != item_b.second) return item_a.second > item_b.second;
	return item_a.first < item_b.first;
}

void add_root(FPTree &tree) {
	Node root;
	root.id = -1; root.parent = -1; root.cnt = 0;
	tree.nodes.push_back(root);
}

void insert_tree(FPTree &tree, vector<int> &line, int mul) {
	int at = 0;
	for (int i=0; i<line.size(); i++) {
		int cur_item = line[i], next;
		//printf("  %d: %d -> %d\n",i,at,cur_item);
		if (tree.nodes[at].children.find(cur_item) == tree.nodes[at].children.end()) {
			next = tree.nodes.size();
			tree.nodes[at].children[cur_item] = next;
			Node new_child;
			new_child.id = cur_item;
			new_child.cnt = 0;
			new_child.parent = at;
			tree.nodes.push_back(new_child);
			tree.header[cur_item].push_back(next);
		}
		else {
			next = tree.nodes[at].children[cur_item];
		}

		//printf(" -- %d\n",next);
		tree.nodes[next].cnt += mul;

		at = next;
	}
}

void fp_growth(FPTree &tree, gp_hash_table<int, null_type> &freqset) {

	//debug_fp(tree, freqset);
	if (freqset.size() > 0) {
		int x = 0;
		for (auto it=freqset.begin(); it!=freqset.end(); it++) {
			putnextint(outline, x, (*it));
		}
		outline[x++] = '\n';
		outline[x++] = '\0';
		fputs(outline, outfile);
	}

	// Empty tree
	if (tree.nodes.size() <= 1) {
		//printf("EMPTY TREE\n");
		return;
	}

	bool ispath = true;
	for (int i=0; i<tree.nodes.size(); i++) {
		if (tree.nodes[i].children.size() > 1) {
			ispath = false;
			break;
		}
	}
	if (ispath) {
		// Just print all combinations of the path elements
		int k = tree.flist.size();
		//if (k>3) printf("Yo this is a path %d\n",k);
		for (int i=1; i<(1<<k); i++) {
			int x = 0;
			for (auto it=freqset.begin(); it!=freqset.end(); it++) {
				putnextint(outline, x, (*it));
			}
			for (int j=0; j<k; j++) {
				if (i & (1<<j)) {
					putnextint(outline, x, tree.flist[j].first);
				}
			}
			outline[x++] = '\n';
			outline[x++] = '\0';
			fputs(outline, outfile);
		}
	}
	else {
		vector<int> itemline;
		for (int i=tree.flist.size()-1; i>=0; i--) {

			int id = tree.flist[i].first;
			freqset.insert(id);
			FPTree condtree;
			add_root(condtree);

			// Count frequency of each item
			for (int j=0; j<tree.header[id].size(); j++) {
				int at = tree.header[id][j];
				int atid = tree.nodes[at].id;
				int mul = tree.nodes[at].cnt;
				while (at != 0) {
					if (condtree.freq.find(atid) == condtree.freq.end()) {
						condtree.freq[atid] = mul;
					}
					else {
						condtree.freq[atid] += mul;
					}
					at = tree.nodes[at].parent;
					atid = tree.nodes[at].id;
				}
			}

			// Construct flist in condition tree
			for (int j=0; j<tree.flist.size(); j++) {
				int fid = tree.flist[j].first;
				int freqcnt = condtree.freq[fid];
				if (fid != id && freqcnt >= support) {
					condtree.flist.push_back(make_pair(fid, freqcnt));
				}
			}

			// Construct condition tree
			for (int j=0; j<tree.header[id].size(); j++) {
				int at = tree.header[id][j];
				int atid = tree.nodes[at].id;
				int mul = tree.nodes[at].cnt;
				//printf("leaf %d: %d\n",at,mul);
				itemline.clear();
				while (at != 0) {
					if (condtree.freq[atid] >= support && at != tree.header[id][j]) {
						itemline.push_back(atid);
					}
					at = tree.nodes[at].parent;
					atid = tree.nodes[at].id;
				}
				reverse(itemline.begin(), itemline.end());
				insert_tree(condtree, itemline, mul);
			}

			// Main recursive call
			fp_growth(condtree, freqset);

			freqset.erase(id);

		}
	}
}

int main(int argc, char* argv[]) {
	if (argc < 4) {
		printf("Usage: ./a.out <X> <inputfile> <outputfile>\n");
		return 0;
	}
	double X;
	sscanf(argv[1], "%lf", &X);
	infile = fopen(argv[2],"r");
	outfile = fopen(argv[3],"w");

	startt();

	// Init FP tree
	FPTree tree;
	add_root(tree);
	//tree.freq.reserve(1<<2);
	//tree.freq.max_load_factor(0.25);

	// First pass
	n = 0;
	while (fgets(itemset, MAX_LINE_LENGTH, infile) != NULL) {
		int x = 0,item = 0;
		while (getnextint(itemset, x, item)) {
			if (tree.freq.find(item) == tree.freq.end()) {
				tree.freq[item] = 0;
			}
			tree.freq[item]++;
		}
		n++;
	}
	support = (int)ceil(n * X / 100);
	printf("Total: %d\n",n);
	printf("Threshold: %d\n",support);

	for (auto it=tree.freq.begin(); it!=tree.freq.end(); it++) {
		if ((*it).second >= support) {
			tree.flist.push_back(make_pair((*it).first, (*it).second));
		}
	}
	sort(tree.flist.begin(), tree.flist.end(), freqcmp);

	/*
	for (int i=0; i<tree.flist.size(); i++) {
		printf("%d ",tree.flist[i].first);
	}
	printf("\n");
	*/

	printf("First pass time elapsed = %.4f\n",runtime());
	if(tree.flist.size()==0)	return 0;

	// Second pass
	rewind(infile);

	vector<pair<int,int> > itemlinepair;
	vector<int> itemline;
	while (fgets(itemset, MAX_LINE_LENGTH, infile) != NULL) {
		int x = 0,item = 0,freqcnt;
		itemlinepair.clear();
		while (getnextint(itemset, x, item)) {
			freqcnt = tree.freq[item];
			if (freqcnt >= support) {
				itemlinepair.push_back(make_pair(item, freqcnt));
			}
		}
		sort(itemlinepair.begin(), itemlinepair.end(), freqcmp);
		itemline.resize(itemlinepair.size());
		for (int i=0; i<itemlinepair.size(); i++) {
			itemline[i] = itemlinepair[i].first;
		}
		insert_tree(tree, itemline, 1);
	}

	printf("Second pass time elapsed = %.4f\n",runtime());

	// Call FP growth
	gp_hash_table<int, null_type> emptyset;
	fp_growth(tree, emptyset);

	printf("Total time elapsed = %.4f\n",runtime());

	fclose(infile);
	fclose(outfile);
	return 0;
}
