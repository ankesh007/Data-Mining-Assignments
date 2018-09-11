#include <iostream>
#include <fstream>
#include <sstream>
#include <ctime>
#include <iterator>
#include <map>
#include <list>
#include <vector>
#include <algorithm>


using namespace std;


void print_vec(vector <int> v)
{
	for (auto i: v)
		cout<<i<<' ';
	cout<<endl;
}
bool check_containment(vector<int> itemset1 , vector<int> itemset2)
{
	int i=0,j=0;
	if (itemset1.size()>itemset2.size())
		return false;

	while(i<itemset1.size() && j<itemset2.size())
	{
		if(itemset1[i]==itemset2[j])
		{
			i++;
			j++;
		}
		else if(itemset1[i]>itemset2[j])
			j++;
		else
			return false;
	}
	return i == itemset1.size();
}


list <vector<int>> generate_candidates(list <vector<int>> Fk_minus_1_items)
{
	list <vector<int>> Fk;
	// list <int> frequent_subsets (Fk.size(),0);
	int k = Fk_minus_1_items.front().size() + 1;
	int N = Fk_minus_1_items.size();
	list <vector <int>>::iterator it1,it2;
	for(it1 = Fk_minus_1_items.begin();it1 != Fk_minus_1_items.end(); ++it1)
	{
		it2 = it1;
		++it2;
		for(; it2!= Fk_minus_1_items.end();++it2)
		{
			vector<int> vec1=*it1 , vec2 = *it2;
			// cout<<"Vectors for merging are : "<<endl;
			// print_vec(vec1);
			// print_vec(vec2);
			// cout<<endl<<endl;
			int i;
			for( i = 0;i<k-2;i++)
				if(vec1[i]!=vec2[i])
					break;
			if(i < k-2)					//the pair doesn't have k-2 items in common
				continue;
			int bigger = (vec1[k-2]>vec2[k-2])?vec1[k-2]:vec2[k-2];
			int smaller = vec1[k-2]+vec2[k-2] - bigger;
			vector <int> newitemset(vec1.begin(),vec1.end()-1);

			newitemset.emplace_back(smaller);
			newitemset.emplace_back(bigger);
			// cout<<"New itemset generated is"<<endl;
			// print_vec(newitemset);
			//count how many subsets are frequent
			int freq_subsets = 0;
			for (list<vector<int>>::iterator it3 = Fk_minus_1_items.begin();it3!=Fk_minus_1_items.end();++it3)
				if(check_containment(*it3,newitemset))
					freq_subsets++;
			if(freq_subsets == k)
				{
					Fk.push_back(newitemset);
					// cout<<"Pushed candidate :"<<endl;
					// print_vec(Fk.back());
				}
		}
	}
	return Fk;
}


void print_itemset(list <vector<int>> Fk,char* outfilename)
{
	ofstream outfile;
    outfile.open (outfilename,ios::app);
	for(auto it : Fk)
	{
		vector<int> itemset = it;
		for(auto i : itemset)
			outfile<< i <<' ';
		outfile<<endl;
	}
	outfile.close();
}

void mine_frequent_itemset(char* infilename,int supportpercent,char* outfilename)
{
	
  	//Read the file to do first pass over data
	map <int,int> itemcounts ;
	int transaction_count = 0;

	FILE* in_file = fopen(infilename, "r");

	char line[1000000];
	vector <int> transaction;
  	while (fgets( line, 1000000, in_file ) != NULL)
	{
		istringstream iss(line);
		transaction.clear();
		int a;
		while(iss>>a)
		{
			transaction.push_back(a);
			if (itemcounts.find(a) == itemcounts.end())
				itemcounts[a] = 1;
			else
				itemcounts[a] = itemcounts[a] + 1;
		}
		sort(transaction.begin(),transaction.end());
		transaction_count ++;
		
	}
	fclose(in_file);
	// cout<<"Completed first pass!"<<endl;
	double support = (supportpercent/100.0)*transaction_count;
	//filter out infrequent data items
	for (auto it = itemcounts.begin(); it!=itemcounts.end(); )
		if(it->second<support)
			itemcounts.erase(it++);
		else
			++it;
	// cout<<"trimmed infrequent items"<<endl;
	//Store frequent item sets of size 1
	list <vector<int>> Fk_minus_1_items;
	// list <int> Fk_minus_1_counts;
	for (auto it = itemcounts.begin(); it!=itemcounts.end();++it )
		{
			vector <int> newvec;
			newvec.emplace_back(it->first);
			Fk_minus_1_items.push_back(newvec);
			// Fk_minus_1_counts.push_back(it->second);
		}
	// cout<<"Generated frequent items of size 1"<<endl;
	//generate frequent itemsets of size k+1
	print_itemset(Fk_minus_1_items,outfilename);
	while(Fk_minus_1_items.size()>1)
	{
		list <vector <int>> Fk_items = generate_candidates(Fk_minus_1_items);		//To Do: implement generate_candidates
		if(Fk_items.size()<1)
			break;
		// cout<<"Generated candidates of size : "<<(Fk_items.front()).size()<<endl;
		// for(auto it : Fk_items)
		// 	print_vec(it);
		list <int> Fk_counts (Fk_items.size(),0);


		in_file = fopen(infilename, "r");

		list<vector<int> >::iterator items;
		list<int>::iterator counts;
		while (fgets( line, 1000000, in_file ) != NULL )	//Going through each transaction
		{
			istringstream iss(line);
			transaction.clear();
			int a;
			while(iss>>a)
			{
				if(itemcounts[a]>=support)
					transaction.push_back(a);
			}
			sort(transaction.begin(),transaction.end());
			// set<int> transaction_set(transaction.begin(),transaction.end());
			items = Fk_items.begin();
			counts = Fk_counts.begin();
			for (;items!=Fk_items.end();++items,++counts)		//for all candidate itemsets
			{
				if(check_containment(*items,transaction))		//To Do: implement check_containment
					*counts = *counts + 1;
			}


			
		}
		fclose(in_file);
		//remove infrequent itemsets

		items = Fk_items.begin();
		counts = Fk_counts.begin();
		for (;items!=Fk_items.end();)
		{
			if(*counts<support)
			{
				Fk_items.erase(items++);
				Fk_counts.erase(counts++);
			}
			else
			{
				++items;
				++counts;
			}
		}
		// cout<<"Remaining candidates "<<endl;
		// for(auto it : Fk_items)
		// 	print_vec(it);
		//print items in Fk_minus_1_items before overwriting
				//To Do: implement print_itemset
		Fk_minus_1_items = Fk_items;
		print_itemset(Fk_minus_1_items,outfilename);
		// cout<<"Size of fk-1 : "<<Fk_minus_1_items.size()<<endl;
	}
}

int main(int argc,char* argv[])
{
	int supportpercent = atoi(argv[1]);
	if (supportpercent <= 100 )		//normal execution of code
	{		
		ofstream outfile;
	    outfile.open (argv[3]);
	    outfile.close();
		mine_frequent_itemset(argv[2],supportpercent,argv[3]);
		return 0;
	}
	else			//generate runtime data
	{
		ofstream outfile;
    	outfile.open ("apriori_runtime.txt");
    	int x [] = {1,5,10,15,25,90};
    	for (int i =0; i<6; i++)
    	{
			clock_t begin = clock();

			mine_frequent_itemset(argv[2],x[i],argv[3]);

			clock_t end = clock();
			double elapsed_secs = double(end - begin) / CLOCKS_PER_SEC;
			outfile << elapsed_secs << endl;
    	}
    	outfile.close();	
	}
	
}