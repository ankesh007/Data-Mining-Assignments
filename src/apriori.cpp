#include <bits/stdc++.h>
using namespace std;

#define boost ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
#define pvii pair<vector<int>, int>
#define vvi vector<vector<int> > 
#define pb push_back
#define mp make_pair

bool differByOne(vector<int> &list1, vector<int> &list2){
	int k = list1.size();
	for (int i=0; i< k-1; i++) if (list1[i] != list2[i]) return false;
	return true;
}

void candidateGen(vvi &frequent, vector<pvii> &candidate){
	if (frequent.size()==0) return;
	int k = frequent[0].size();
	int n = frequent.size();

	for (int i= 0; i< n; i++){
		for (int j = i+1; j < n; j++){
			if (differByOne(frequent[i], frequent[j])){
				// add in the candidate solution.
				vector<int> newList = frequent[i];
				newList.push_back(frequent[j][k-1]);
				candidate.pb(mp(newList,0));
			}
			else break; // because sorted list
		}
	}	
}

void selectAndSaveFrequent(vector<pvii> &candidate, int threshold,vector<vector<int> > &frequent, string &outName){
	ofstream outdata;
	outdata.open(outName);

	for (vector<pvii> :: iterator it = candidate.begin(); it != candidate.end(); it ++){
		if ((it->second) >= threshold){
			frequent.push_back(it->first);
			for (vector<int> :: iterator it1 = (it->first).begin(); it1 != (it->first).end(); it1++){
				outdata << *it1 << " ";
			}
			outdata << endl;
		}
	}
	outdata.close()
}

void updateCount(string &inpFile, vector<pvii> &candidate){
	string line;
	int item;
	ifstream myfile(inpName.c_str()); 
	int n = candidate.size();

	if (myfile.is_open()){
		while(getline(myfile, line)){
		  istringstream iss(line);
	 	  set<int> currSet;
		  while(iss>>item) currSet.insert(item);
		  
		  for (int i=0; i < n; i++){
		  	bool isSubset = true;
		  	for (vector<int> :: iterator it = candidate[i].first.begin(); it != candidate[i].first.end() && isSubset; it++){
		  		if (currSet.find((*it)) == currSet.end()) isSubset = false;
		  	}
		  	if (isSubset) candidate[i].second++;
		  }
		}
	}
	myfile.close();
}

// returns the value of the threshold count
int init_pass(string &inpName, vector<pvii> &candidate,double k){
	map<int,int> singleton;
	string line;
	int item;
	int totalTransactions = 0;

	ifstream myfile(inpName.c_str()); 
	if (myfile.is_open()){
		while(getline(myfile, line)){
			if (line != "") totalTransactions++;
			istringstream iss(line);
			while(iss>>item){
				if (singleton.find(item)== singleton.end()) singleton[item] = 0;
				singleton[item]++;
			} 
		}
	}
	myfile.close();
	
	for (map<int,int> :: iterator it = singleton.begin(); it != singleton.end(); it++){
		vector<int> vec(1,it->first);
		candidate.pb(mp(v, it->second));
	}
	return ceil((k/100.0)*totalTransactions);
}

void apriori(string &inpName, string &outName, double k){
	vector<pvii> candidate;
	int threshold = init_pass(inpFp, candidate, k);

	vvi frequent;
	selectAndSaveFrequent(candidate, threshold, frequent, outName);
	while(!frequent.isEmpty()){
		candidate.clear();
		candidateGen(frequent, candidate);
		updateCount(inpFile,candidate);
		frequent.clear();
		selectAndSaveFrequent(candidate,frequent,outName);
	}
}

int main(){
	boost
	double k;
	cin >> k;
	apriori("inp", "out", k);
	return 0;
}