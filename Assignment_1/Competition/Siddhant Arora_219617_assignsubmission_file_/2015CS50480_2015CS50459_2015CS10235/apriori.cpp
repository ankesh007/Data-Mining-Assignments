#include <iostream>
#include <vector>
#include <algorithm> 
#include <fstream>
#include <string>
#include <unordered_map>
#include <sstream>
#include <set>
#include <algorithm>
#include <chrono>

#define ll long long int
using namespace std;

int main(int argc, char ** argv)
{
	auto start = chrono::high_resolution_clock::now();
	int i,j;
	unordered_map <int,int> item_count;
	string infilename=argv[1];
	float x=atof(argv[2]);
	string outfilename=argv[3];
	int num_transactions = 0;
	// string infilename = "retail.dat";
	ifstream inputfile;
	inputfile.open(infilename);
	vector<set<int> > transact;

	while (inputfile)
	{
		set<int> cur_transact;
	    string s;
	    if (!getline( inputfile, s )) 
			break;
		num_transactions++;
	    istringstream ss( s.c_str() );
	    while (ss)
	    {
			string s;
			if (!getline( ss, s, ' ' )) 
				break;
			int k = stoi(s);
			cur_transact.insert(k);
			if(item_count.find(k)==item_count.end()){
				item_count[k]=1;
			}
			else{
				item_count[k]+=1;
			}
	    }
	    transact.push_back(cur_transact);
	}
	int min_support=x*num_transactions/100;

	vector<vector<int> > f_cur;
	vector<vector<int> > f_cur_next;

	vector<vector<int> > freq_item;

	for(auto& x : item_count)
	{
		if (x.second>=min_support)
		{
			vector<int> f1_cur;
			f1_cur.push_back(x.first);
			f_cur.push_back(f1_cur);
		}
	}

	while(f_cur.size()>0)
	{
		for(i=0;i<f_cur.size();i++)
		{
			freq_item.push_back(f_cur[i]);
		}

		f_cur_next.resize(0);

		int n = f_cur.size();

		for(i=0;i<n;i++)
		{
			for(j=i+1;j<n;j++)
			{
				set<int> cur_cur;

				for(int kk=0;kk<f_cur[i].size();kk++)
				{
					cur_cur.insert(f_cur[i][kk]);
					cur_cur.insert(f_cur[j][kk]);
				}

				int sz = f_cur[i].size();

				if(cur_cur.size() == (sz + 1))
				{
					int my_count = 0;

					vector<int> cur_cur_1;

					for(auto& x: cur_cur)
					{
						cur_cur_1.push_back(x);
					}

					for(int kk=0;kk<transact.size();kk++)
					{
						bool pres = true;

						for(int jj = 0;jj<cur_cur_1.size();jj++)
						{
							int xx = cur_cur_1[jj];
							if(transact[kk].find(xx) == transact[kk].end())
							{
								pres = false;
								break;
							}
						}

						if(pres)
							my_count ++;
					}

					if (my_count>=min_support)
					{
						sort(cur_cur_1.begin(),cur_cur_1.end());
						f_cur_next.push_back(cur_cur_1);
					}
				}
			}
		}

		f_cur.resize(0);

		for(i=0;i<f_cur_next.size();i++)
		{
			bool diff = true;
			for(j=0;j<i;j++)
			{
				bool stopped = false;
				for(int kk = 0;kk<f_cur_next[i].size();kk++)
				{
					if (f_cur_next[i][kk]!=f_cur_next[j][kk])
					{
						stopped = true;
						break;
					}
				}
				if(stopped == false)
				{
					diff = false;
					break;
				}
			}
			if(diff)
				f_cur.push_back(f_cur_next[i]);
		}

	}

	ofstream outputfile;
	outputfile.open(outfilename);
	for(i=0;i<freq_item.size();i++)
	{
		for(int j=0;j<freq_item[i].size();j++)
		{
			outputfile<<freq_item[i][j]<<" ";
		}
		outputfile<<"\n";
	}
	auto stop = chrono::high_resolution_clock::now();
	auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
	float p=duration.count();
	cout<<p/1000000<<endl;

	return 0;
}