#include <bits/stdc++.h>
using namespace std;

#define boost ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
#define pvii pair<vector<int>, int>
#define vvi vector<vector<int> > 
#define pb push_back
#define mp make_pair

ofstream outdata;

bool differByOne(vector<int> &list1, vector<int> &list2){
	int k = list1.size();
	for (int i=0; i< k-1; i++) if (list1[i] != list2[i]) return false;
	return true;
}

int toComp(vector<int> &v1, vector<int> &v2){
	int n = v1.size();
	for (int i=0; i< n; i++){
		if (v1[i] != v2[i]) return (v1[i]-v2[i]);
	}
	return 0;
}

bool binSearch(int start, int end, vector<int> &toCheck, vvi &frequent){
	if (start >= end){
		if (toComp(frequent[start], toCheck)==0) return true;
		else return false;
	}
	int mid = (start+end)/2;
	int midVal = toComp(frequent[mid],toCheck);
	if (midVal == 0) return true;
	else if (midVal < 0) return binSearch(mid+1, end, toCheck, frequent);
	else return binSearch(start, mid-1, toCheck, frequent); 
}

void skipOneElement(vector<int> &arr, vector<int> &ans, int idx){
	for (int i=0; i< arr.size(); i++){
		if (i!= idx) ans.push_back(arr[i]);
	}
}

void candidateGen(vvi &frequent, vector<pvii> &candidate){
	if (frequent.size()==0) return;
	int k = frequent[0].size();
	int n = frequent.size();

	for (int i= 0; i< n; i++){
		for (int j = i+1; j < n; j++){
			if (differByOne(frequent[i], frequent[j])){
				// Add in the candidate solution
				vector<int> newList = frequent[i];
				newList.push_back(frequent[j][k-1]);
				// prune the ineligible sets	
				
				bool isEligible = true;
				for (int x = 0; (x < newList.size()-2) && isEligible; x++){
					vector<int> toCheck;
					skipOneElement(newList,toCheck,x);
					if (!binSearch(0,frequent.size()-1,toCheck,frequent)) isEligible = false;
				}
				if (isEligible) candidate.pb(mp(newList,0));
			}
			else break; // because sorted list
		}
	}
}

void selectAndSaveFrequent(vector<pvii> &candidate, int threshold,vector<vector<int> > &frequent){
	for (vector<pvii> :: iterator it = candidate.begin(); it != candidate.end(); it ++){
		if ((it->second) >= threshold){
			frequent.push_back(it->first);
			for (vector<int> :: iterator it1 = (it->first).begin(); it1 != (it->first).end(); it1++){
				outdata << (*it1) << " ";
			}
			outdata << endl;
		}
	}
}

void updateCount(string &inpName, vector<pvii> &candidate){
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
				if (singleton.find(item) == singleton.end()) singleton[item] = 0;
				singleton[item]++;
			} 
		}
	}
	myfile.close();
	cout << singleton.size() << " " << totalTransactions << endl;
	for (map<int,int> :: iterator it = singleton.begin(); it != singleton.end(); it++){
		vector<int> vec(1,it->first);
		candidate.pb(mp(vec, it->second));
	}
	return ceil((k/100.0)*totalTransactions);
}

void apriori(string &inpName, double k){
	vector<pvii> candidate;
	int threshold = init_pass(inpName, candidate, k);

	vvi frequent;
	selectAndSaveFrequent(candidate, threshold, frequent);
	int temp;
	while(frequent.size()!=0){
		cout << temp++ << " " << frequent.size() << endl;
		candidate.clear();
		candidateGen(frequent, candidate);
		updateCount(inpName,candidate);
		frequent.clear();
		selectAndSaveFrequent(candidate,threshold,frequent);
	}
}

int main(int argc, char* argv[]){
	boost
	clock_t t1, t2;
	t1 = clock();
	string s1(argv[1]);
	string s2(argv[2]);
	double f = atof(argv[3]);
	outdata.open(s2);
	apriori(s1,f);
	outdata.close();
	t2 = clock();
	double time_taken = ((double) (t2 - t1)) / CLOCKS_PER_SEC;

	ofstream outfile;
	if (atoi(argv[4])==1)
	{
		outfile.open("time_apriori");
	}
	else{
		outfile.open("time_apriori", ios_base::app);
	}
	outfile<<time_taken<<endl;
	outfile.close();
	return 0;
}
