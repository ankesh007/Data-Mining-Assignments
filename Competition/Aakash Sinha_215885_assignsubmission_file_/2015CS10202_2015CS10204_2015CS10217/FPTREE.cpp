#include<bits/stdc++.h>
using namespace std;
#define ll long long
#pragma optimize("O3")
#define prime 1000003
#define mod 1000000007
#pragma comment(linker, "/stack:2000000000")
char fp[]="-fptree",ap[]="-apriori";
vector<int> frequentitems;
unordered_map<int,int> itemidx;
unordered_map<int,int> fstocur;
vector<pair<int,int> > tree;
vector<unordered_map<int,int> > nxtpntr;
vector<vector<int> > nxtidx;
vector<int> rightptr;
unordered_map<int,int> lastidx,fstidx;
vector<vector<int> > frequentsets;
vector<int> parent;
double mincount;
vector< vector<pair<int,int> > > leaves;
int mx=0;
inline int readInt() 
{
	char c;
	int n=0;
    c=getchar_unlocked();
    if(feof(stdin))
    	return -2;
    if(c=='\n'||c=='\r')
    {
    	if(c=='\r')
    		c=getchar_unlocked();
    	return -1;
    }
    while (!('0' <= c && c <= '9')) 
        c = getchar_unlocked();
    while('0'<=c&&c<='9') 
    {
        n=n*10+c-'0';
        c=getchar_unlocked();
    }
    return n;
}
inline void writeInt(int a)
{
    register char c;
    char snum[5];
    int i=0;
    do
    {
        snum[i++]=a%10+48;
        a=a/10;
    }
    while(a!=0);
    --i;
    while(i>=0)
        putchar_unlocked(snum[i--]);
    putchar_unlocked(' ');
}
int comp(vector<int> &v1,vector<int> &v2)
{
	if(v1.size()==v2.size())
	{
		int great=false;
		for(int i=0;i<v1.size();i++)
			if(v1[i]<v2[i])
			{
				great=true;break;
			}
			else if(v1[i]>v2[i])
				break;
		return great;
	}
	return v1.size()<v2.size();
}
void dfs(int i)
{
	leaves[itemidx[tree[i].first]].push_back({tree[i].second,i});
	for(auto j:nxtidx[i])
		dfs(j);
}
void mine_frequent(vector<pair<int,int> > &items,int idx,vector<int> &itemset)
{
	int cnt=0;
	vector<pair<int,int> > next;
	unordered_map<int,int> parent_counts;
	unordered_map<int,int> totcnt;
	for(auto i:items)
	{
		if(tree[i.second].first==frequentitems[idx])
		{
			cnt+=i.first;
			int j=parent[i.second];
			while(j!=-1)
				parent_counts[j]+=i.first,totcnt[tree[j].first]+=i.first,j=parent[j];
		}
	}
	if((double)cnt*100.0<mincount||idx<0)
		return;
	itemset.push_back(frequentitems[idx]);
	frequentsets.push_back(itemset);
	for(auto i:parent_counts)
		if(totcnt[tree[i.first].first]*100.0>=mincount)
			next.push_back({i.second,i.first});
	for(int i=idx-1;i>=0;i--)
			mine_frequent(next,i,itemset);
	if(!itemset.empty())
		itemset.pop_back();
}
vector<vector<int> > nxt;
vector<vector<vector<int> > > itemsets;
vector<vector<int> > gennext,tempvec;
unordered_map<int,int> mp1;
vector<set<int> > vectorset;
vector<int> allcounts,tempcounts;
int get_hash(vector<int> &v)
{
	ll cur=0,num=1;
	for(ll i=0;i<v.size();i++)
	{
		cur=cur+v[i]*num;
		cur%=mod;
		num=(num*prime)%mod;
	}
	return cur;
}
int get_diff(vector<int> &a,vector<int> &b)
{
	unordered_set<int> u;
	assert(a.size()==b.size());
	for(int i=0;i<a.size();i++)
		u.insert(a[i]),u.insert(b[i]);
	return (u.size()==a.size()+1);
}
vector<int> uni(vector<int> &v1,vector<int> &v2)
{
	set<int> s;
	for(int i=0;i<v1.size();i++)
		s.insert(v1[i]),s.insert(v2[i]);
	vector<int> ans;
	for(auto j:s)
		ans.push_back(j);
	return ans;
}
void candidate(vector<vector< int> > &prev)
{
	nxt.clear();
	unordered_set<int> hashes,hashes_new;
	int n=prev.size();
	for(int i=0;i<n;i++)
		hashes.insert(get_hash(prev[i]));
	for(int i=0;i<n;i++)
	{
		for(int j=i+1;j<n;j++)
		{
			bool poss=true;
			vector<int> c;
			if(get_diff(prev[i],prev[j]))
			{
				vector<int> temp=uni(prev[i],prev[j]);
				c=temp;
			}
			int h=get_hash(c);
			if(c.empty()||hashes_new.find(h)!=hashes_new.end())
				continue;
			for(int k=0;k<c.size();k++)
			{
				vector<int> temp(c.begin(),c.end());
				temp.erase(temp.begin()+k);
				poss&=(hashes.find(get_hash(temp))!=hashes.end());
			}
			if(poss)
				nxt.push_back(c),hashes_new.insert(h);
		}
	}
}
int main(int argc,char* argv[])
{
	char* inputfile=argv[1];
	double x=atof(argv[2]);
	char* outfile=argv[4];
	if(strcmp(argv[3],fp)==0)
	{
		unordered_map<int,int> mp;
		int n=0;
		int tot=0;
		bool poss=false;
		for(int i=0;i<2;i++)
		{
			freopen(inputfile,"r",stdin);
			if(i>0)
				mincount=(double)tot*x;
			poss=false;
			while(!poss)
			{
				int num;
				vector<pair<int,int> > items;
				tot+=(i==0);
				while(1)
				{
					num=readInt();
					poss|=(num==-2);
					if(num<0)
						break;
					if(i==0)
						mp[num]++;
					else
						items.push_back({mp[num],num});
				}
				if(i==0)
					continue;
				sort(items.begin(),items.end());
				reverse(items.begin(),items.end());
				int curidx=-1;
				for(int j=0;j<items.size();j++)
				{
					int curitem=items[j].second;
					if((double)mp[curitem]*100.0<mincount)
						break;
					if(j==0)
					{
						if(fstocur[curitem]==-1)
						{
							tree.push_back({curitem,1});
							parent.push_back(-1);
							curidx=tree.size()-1;
							fstocur[curitem]=curidx;
							unordered_map<int,int> temp;
							nxtpntr.push_back(temp);
							vector<int> v;
							nxtidx.push_back(v);
						}
						else
						{
							curidx=fstocur[curitem];
							tree[curidx].second++;
						}
					}
					else
					{
						if(nxtpntr[curidx].find(curitem)==nxtpntr[curidx].end())
						{
							tree.push_back({curitem,1});
							nxtpntr[curidx][curitem]=tree.size()-1;
							nxtidx[curidx].push_back(tree.size()-1);
							parent.push_back(curidx);
							curidx=tree.size()-1;
							unordered_map<int,int> temp;
							vector<int> v;
							nxtpntr.push_back(temp);
							nxtidx.push_back(v);
						}
						else 
						{
							assert(nxtpntr[curidx].find(curitem)!=nxtpntr[curidx].end());
							curidx=nxtpntr[curidx][curitem];
							tree[curidx].second++;
							assert(curidx<tree.size());
						}
					}
				}
			}
			if(i==0)
				tot--;
			fclose(stdin);
			if(i==0)
			{
				set<pair<int,int> > s;
				for(auto j:mp)
					s.insert({j.second,j.first}),n++;
				for(auto j:s)
					frequentitems.push_back(j.second);
				reverse(frequentitems.begin(),frequentitems.end());
				for(int j=0;j<frequentitems.size();j++)
					itemidx[frequentitems[j]]=j,fstocur[frequentitems[j]]=-1;
			}
		}
		vector<int> temp(tree.size(),-1);
		rightptr=temp;
		vector<int> ans;
		leaves.resize(n);
		for(int i=0;i<n;i++)
			if(fstocur[frequentitems[i]]!=-1)
				dfs(fstocur[frequentitems[i]]);
		for(int i=n-1;i>=0;i--)
			ans.clear(),mine_frequent(leaves[i],i,ans);
		vector<vector<int> > v1;
		for(auto i:frequentsets)
		{
			vector<int> t;
			for(auto j:i)
				t.push_back(j);
			sort(t.begin(),t.end());
			v1.push_back(t);
		}
		// sort(v1.begin(),v1.end(),comp);
		freopen(outfile,"w",stdout);
		for(auto i:v1)
		{
			for(auto j:i)
				writeInt(j);
			printf("\n");
		}
	}
	else
	{
		string line;
		int itr=0;
		int cnt=0;
		while(1)
		{
			ifstream inp(inputfile);
			vectorset.clear();
			vectorset.resize(gennext.size());
			allcounts.clear();
			allcounts.resize(gennext.size());
			for(int i=0;i<gennext.size();i++)
			{
				for(auto j:gennext[i])
					vectorset[i].insert(j);
				allcounts[i]=0;
			}
			while(getline(inp,line))
			{
				tempcounts.clear();
				tempcounts.resize(gennext.size());
				for(int i=0;i<gennext.size();i++)
					tempcounts[i]=gennext[i].size();
				istringstream strm(line);
				int num;
				while(strm>>num)
				{
					if(itr==0)
						mp1[num]++;
					else
					{
						for(int i=0;i<gennext.size();i++)
						{
							if(vectorset[i].find(num)!=vectorset[i].end())
								tempcounts[i]--;
						}
					}
				}
				for(int i=0;i<gennext.size();i++)
					if(tempcounts[i]==0)
						allcounts[i]++;
				if(itr==0)
					cnt++;
			}
			if(itr==0)
			{
				vector<vector<int> > temp;
				for(auto i:mp1)
				{
					if((double)i.second*100.0>=x*(double)cnt)
					{
						vector<int> item;
						item.push_back(i.first);
						temp.push_back(item);
					}
				}
				itemsets.push_back(temp);
				candidate(temp);
				gennext=nxt;
			}
			else
			{
				tempvec.clear();
				for(int i=0;i<gennext.size();i++)
					if((double)allcounts[i]*100.0>=x*(double)cnt)
						tempvec.push_back(gennext[i]);
				itemsets.push_back(tempvec);
				candidate(tempvec);
				gennext=nxt;
				if(gennext.empty())
					break;
			}
			itr++;
			inp.close();
		}
		freopen(outfile,"w",stdout);
		vector<vector<int> > v1;
		for(auto j:itemsets)
		{
			for(auto i:j)
				v1.push_back(i);
		}
		sort(v1.begin(),v1.end(),comp);
		for(auto i:v1)
		{
			for(auto j:i)
				cout<<j<<' ';
			cout<<endl;
		}
	}
}