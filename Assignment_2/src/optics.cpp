#include "dbscan.h"
#include "ds.h"
using namespace std;

// reference :  https://en.wikipedia.org/wiki/OPTICS_algorithm 
#define boost ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);
#define pif pair<int,float>
#define pfi pair<float, int>

float point_collection[POINTS][DIM];
vector<pif> orderedPoints;
vector<float> reachabilityDist;
vector<bool> isProcessed;
float epsilon;
int minNumOfPts;
int dimension, instances;

void readInput(char* fname){
	float pt;
	string line;
  	ifstream myfile(fname); 
  	int i, j;
  	i = 0;
    if (myfile.is_open()){
		stringstream ss;
		while(getline(myfile, line)){
			istringstream iss(line);
			j = 0;
			while(iss>>pt){
				point_collection[i][j++] = pt;
			}
			dimension = j;
			i++;
		}	
    }
    instances = i;
    myfile.close();
	return;
}

float calculateDist(int idx1, int idx2){
	float ans = 0;
	for (int i=0;i< dimension; i++){
		ans += (point_collection[idx1][i] - point_collection[idx2][i])*(point_collection[idx1][i] - point_collection[idx2][i]);
	}
	return sqrt(ans);
}

// void getNbr(int root, vector<int> &nbr){
// 	int n = point_collection.size();
// 	for (int i=0; i< n; i++){
// 		if (calculateDist(i, root) <= epsilon) nbr.pb(i);
// 	}
// }

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
	int n = instances;

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
			query_ds(i,nbr);
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
					query_ds(node, nbr1);
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
	readInput(argv[1]);
	epsilon = atof(argv[2]);
	minNumOfPts = atoi(argv[3]);
	init_ds(dimension,instances,epsilon);
	optics();
	output(argv[4]);
	return 0;
}