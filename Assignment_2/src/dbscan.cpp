#include "dbscan.h"
#include "ds.h"
#include <bits/stdc++.h>
#include <boost/geometry.hpp>
#include <boost/geometry/index/rtree.hpp>
using namespace std;

// #define POINT(X) bg::model::point<float, X, bg::cs::cartesian>
// #define BOX(X) bg::model::box<bg::model::point<float, X, bg::cs::cartesian> >
// #define VALUE(X) pair<bg::model::box<bg::model::point<float, X, bg::cs::cartesian> >, size_t>

namespace bg = boost::geometry;
namespace bgi = boost::geometry::index;

float epsilon;
int minPts;
int numPoints=0;
int dimension=0;

bool is_noise[POINTS];
int my_queue[POINTS];
int current_tail=0;//Where enqueue happens 
int current_head=0;//Where dequeue happens
float point_collection[POINTS][DIM];
bool used[POINTS];

vector<vector<int> > clusters;
int cluster_no=0;
vector<int> neighbour;

void expand(int i)
{
	neighbour.clear();
	query_ds(i,neighbour);
	
	if(((int)neighbour.size())<minPts)
	{
		is_noise[i]=true;
		return;
	}
	else
	{
		clusters.resize(cluster_no+1);
		clusters[cluster_no].pb(i);
		used[i]=true;

		for(auto itr:neighbour)
		{
			if(used[itr])
				continue;
			used[itr]=true;
			my_queue[current_tail++]=itr;
		}

		while(current_head<current_tail)
		{
			int point_id=my_queue[current_head++];
			clusters[cluster_no].pb(point_id);

			if(is_noise[point_id])
				continue;
			else
			{
				neighbour.clear();
				query_ds(point_id,neighbour);
				if(((int)neighbour.size())<minPts)
					continue;
				else
				{
					for(auto itr:neighbour)
					{
						if(!used[itr])
						{
							my_queue[current_tail++]=itr;
							used[itr]=true;
						}
					}
				}
			}
		}
		cluster_no++;
	}
}

void dbscan()
{
	for(int i=0;i<numPoints;i++)
	{
		if(!used[i])
		{
			expand(i);
		}
	}
}

void parseInput(char *filename)
{
	ifstream read_input;
	read_input.open(filename);
    string tokens,line;
    int temp_dim=0;

	while(getline(read_input,line))
	{
		stringstream check1(line);
		temp_dim=0;

	    while(getline(check1, tokens, ' '))
	    {
	    	float val=stof(tokens);
	    	point_collection[numPoints][temp_dim]=val;
	    	++temp_dim;
	    }
		numPoints++;
	}
	dimension=temp_dim;
	read_input.close();
}

void writeOutput(const char *filename)
{
	ofstream write_output;
	write_output.open(filename);

	int counter=0;
	for(auto itr:clusters)
	{
		write_output<<"#"<<counter<<"\n";
		for(auto itr2:itr)
			write_output<<itr2<<"\n";
		counter++;
	}

	write_output<<"#outlier"<<"\n";
	for(int i=0;i<numPoints;i++)
	{
		if(!used[i])
			cout<<i<<"\n";
	}
	write_output.close();
}

int main(int argc,char **argv)
{
	minPts=atoi(argv[1]);
	epsilon=atof(argv[2]);

	parseInput(argv[3]);
	// cout<<dimension<< " "<<epsilon<<" "<<numPoints<<endl;
	// return 0;
	init_ds(dimension,numPoints,epsilon);
	dbscan();
	// cout<<cluster_no<<endl;
	if(argc==5)
		writeOutput(argv[4]);
	else
		writeOutput("kmeans.txt");

	return 0;
}