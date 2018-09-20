#include <bits/stdc++.h>
using namespace std;

#define boost ios_base::sync_with_stdio(false);cin.tie(0);cout.tie(0);

vector< vector<float> > dataPoints;
vector< vector<float> > k_points;
vector<int> assignments;
int total_points, point_dim, num_cluster;
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

void initialize_clusters(int k){
	set<int> random_indices;
	while(int(random_indices.size())!=k){
		int num = rand()%total_points;
		random_indices.insert(num);
	}
	set<int>::iterator it;
	for (it = random_indices.begin(); it != random_indices.end(); it++)
	{
		k_points.push_back(dataPoints[*it]);
	}
	int i;
	for (i = 0; i < total_points; i++)
	{
		assignments[i] = -1;
	}
	return;
}

float distance(vector<float>& p1, vector<float>& p2){
	int i = 0;
	float dis = 0;
	for (i = 0; i < point_dim; i++)
	{
		dis = dis + (p1[i] - p2[i])*(p1[i] - p2[i]);
	}
	dis = sqrt(dis);
	return dis;
}

void kmeans(){
	int i,j, min_clust,l,m,flag=1;
	float min_dis, dist;
	vector<int> num_in_cluster(num_cluster,0); 
	while(flag==1){
		flag = 0;
		for (l = 0; l < num_cluster; l++)
		{
			num_in_cluster[l] = 0;
		}
		for (i = 0; i < total_points; i++)
		{
			min_dis = FLT_MAX; 
			min_clust = 0;
			for (j = 0; j < num_cluster; j++)
			{
				dist = distance(k_points[j],dataPoints[i]);
				if (dist<min_dis)
				{
					min_dis = dist;
					min_clust = j;
				}
			}
			if (assignments[i]!=min_clust)
			{
				flag = 1;
			}
			assignments[i] = min_clust;
			num_in_cluster[min_clust]++;
		}
		for (l = 0; l < num_cluster; l++)
		{
			for (m = 0; m < point_dim; m++)
			{
				k_points[l][m] = 0.0;
			}
		}
		for (i = 0; i < total_points; i++)
		{
			for (l = 0; l < point_dim; l++)
			{
				k_points[assignments[i]][l] = k_points[assignments[i]][l] + dataPoints[i][l]/num_in_cluster[assignments[i]];
			}
		}
	}
	return;
}

void print_results(){
	ofstream myfile;
	myfile.open("kmeans.txt");
	int l,i;
	vector< vector<int> > ass(num_cluster, vector<int>());

	for (i = 0; i < total_points; i++)
	{
		ass[assignments[i]].push_back(i);
	}

	for (l = 0; l < num_cluster; l++)
	{
		cout<<"clusters"<<num_cluster<<endl;
		myfile<<"#"<<l<<endl;
		int num_pts = ass[l].size();
		for (i = 0; i < num_pts; i++)
		{
			myfile<<ass[l][i]<<endl;
		}
	}
	myfile.close();
}

int main(int argc, char *argv[])
{
	boost
	readInput(argv[1]);
	num_cluster = atoi(argv[2]);
	total_points = dataPoints.size();
	if (total_points>0)
	{
		point_dim = dataPoints[0].size();
		assignments.resize(total_points);
		initialize_clusters(num_cluster);
		kmeans();
	}
	print_results();
	
	return 0;
}