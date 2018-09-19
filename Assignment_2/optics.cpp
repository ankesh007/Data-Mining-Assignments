#include <bits/stdc++.h>
using namespace std;

// reference :  https://en.wikipedia.org/wiki/OPTICS_algorithm 

#define boost ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
#define pif pair<int,float>
#define mp make_pair
#define pb push_back
#define pfi pair<float, int>

vector< vector<float> > dataPoints;
vector<pif> orderedPoints;
vector<float> reachabilityDist;
vector<bool> isProcessed;
float eps;
int minNumOfPts;

void readInput(char* fname){
	float pt;
	string line;
  	ifstream myfile(fname); 
    if (myfile.is_open()){
		stringstream ss;
		vector<float> temp;
		while(getline(myfile, line))
		{
			istringstream iss(line);
			while(iss>>pt){
				temp.push_back(pt);
			}
			dataPoints.push_back(temp);
			temp.clear();
		}    	
    }
    myfile.close();
	return;
}

float calculateDist(int idx1, int idx2){
	float ans = 0;
	int d = dataPoints[0].size();
	for (int i=0;i< d; i++){
		ans += (dataPoints[idx1][i] - dataPoints[idx2][i])*(dataPoints[idx1][i] - dataPoints[idx2][i]);
	}
	return sqrt(ans);
}

void getNbr(int root, vector<int> &nbr){
	int n = dataPoints.size();
	for (int i=0; i< n; i++){
		if (calculateDist(i, root) <= eps) nbr.pb(i);
	}
}

// returns -1 if the root point cannot be a core point.
float getCoreDist(int root, vector<int> &nbr){
	if (nbr.size() < minNumOfPts) return -1;
	else {
		priority_queue<pfi> pq;
		int n = nbr.size();
		for(int i=0; i< minNumOfPts; i++) pq.push(mp(calculateDist(root, nbr[i]), nbr[i]));
		for (int i = minNumOfPts; i< n; i++){
			float d1 = calculateDist(root, nbr[i]);
			float d2 = pq.top().first;
			if (d1 < d2){
				pq.pop();
				pq.push(mp(d1, nbr[i]));
			}
		}
		return pq.top().first;
	}
}

void updateReachability(vector<int> &nbr, int root, priority_queue<pfi, vector<pfi>, greater<pfi> > &pq){
	float coreDist = getCoreDist(root, nbr);
	int n = nbr.size();
	for (int i=0; i< n; i++){
		int node = nbr[i];
		if (!isProcessed[node]){
			float currReachDist = max(coreDist, calculateDist(node, root));
			reachabilityDist[node] = (reachabilityDist[node]==-1)? currReachDist: min(reachabilityDist[node], currReachDist);
			pq.push(mp(reachabilityDist[node], node));
		}
	}
}

void optics(){
	int n = dataPoints.size();

	orderedPoints.clear();
	reachabilityDist.clear();
	isProcessed.clear();

	reachabilityDist.resize(n, -1); // -1 -> not defined yet
	isProcessed.resize(n, false);

	for (int i=0; i< n; i++){
		if (!isProcessed[i]){
			isProcessed[i] = true;
			orderedPoints.pb(mp(i,reachabilityDist[i]));
			
			vector <int> nbr;
			getNbr(i,nbr);
			if (nbr.size() >= minNumOfPts){
				priority_queue <pfi, vector<pfi>, greater<pfi> > pq;
				updateReachability(nbr, i, pq);
				while (!pq.empty()){
					int node = pq.top().second;
					pq.pop();
					if (isProcessed[node]) continue;
					isProcessed[node] = true;
					orderedPoints.pb(mp(node, reachabilityDist[node]));
					
					vector<int> nbr1;
					getNbr(node, nbr1);
					if (nbr1.size() >= minNumOfPts){
						updateReachability(nbr1, node, pq);
					}
				}
			}
		}
	}
}

void output(char* fname){
	ofstream myfile;
	myfile.open(fname);
	for (int i=0; i < orderedPoints.size(); i++){
		myfile << orderedPoints[i].second << endl;
	}
	myfile.close();
}

int main(int argc, char *argv[]){
	boost
	dataPoints.clear();
	readInput(argv[1]);
	eps = atof(argv[2]);
	minNumOfPts = atoi(argv[3]);
	optics();
	output(argv[4]);
	return 0;
}