#include "declaration.h"
#include "pattern.h"

char* string_to_chararr(string &s)
{
    int x=s.length();
    char *arr=new char[x+1];
    for(int i=0;i<x;i++)
    {
        arr[i]=s[i];
    }
    s[x]=0;
    return arr;
}

float get_score(int pos_count, int neg_count)
{
    if(pos_count<=0)
    {
        return -1;
    }
    float rplus=pos_count*1.0/positive_graph_count;
    float rneg = (neg_count+1)*1.0 / (negative_graph_count+1);

    return log(rplus/rneg);
}

int correct_size(vector<int> &ngids)
{
    int coun = 0;
    int x = ngids.size();
    for (int i = 0; i < x; i++)
        if (ngids[i] < positive_graph_count + negative_graph_count)
            coun++;
    return coun;
}

void update_pattern_score(pattern *p)
{
    p->score_precise = get_score(p->pgids.size(), correct_size(p->ngids));
    p->score_binned = max(0,(int)(p->score_precise * 10.0) + 1);
}

void update_gids(bool isPositiveGraph, int currentGid,vector<int> &pgids, vector<int> &ngids)
{
    if (isPositiveGraph)
    {
        int len = pgids.size();
        if (len == 0 || pgids[len - 1] != currentGid)
        {
            pgids.pb(currentGid);
        }
    }
    else
    {
        int len = ngids.size();
        if (len == 0 || ngids[len - 1] != currentGid)
        {
            ngids.pb(currentGid);
        }
    }
}
