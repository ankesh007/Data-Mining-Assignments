#include <map>

#include "pattern.h"
#include "declaration.h"

extern double pfreq_threshold;	
extern vector<graph *> graph_database;
extern map<vector<int>, pattern *> pattern_code;

int get_source_from_hash(int hash)
{
    return ((int)((hash) >> SOURCE_SHIFT) & SOURCE_MASK);
}
int get_edge_from_hash(int hash)
{
    return ((int)((hash) >> EDGE_SHIFT) & EDGE_MASK);
}
int get_drain_from_hash(int hash)
{
    return (((int)((hash))) & DRAIN_MASK);
}

int get_hash_from_extension(int u,int x,int y,int z)
{
    return ((((int)(u)) << TYPE_SHIFT) | (((int)(x)) << SOURCE_SHIFT) | ((int)(y) << EDGE_SHIFT) | ((int)(z)));
}

pattern::pattern()
{
    this->momentum = 0;
    this->code.clear();
    this->score_precise = 0.0;
    this->score_binned = 0;
    size = 0;
    has_potential = true;
}

pattern::~pattern()
{
    for (int i = 0; i < size; i++)
        delete[] matrix[i];
    if (size != 0)
        delete[] matrix;
    for (int i = 0; i < embeddings.size(); i++)
    {
        for (int j = 0; j < (embeddings[i].second)->size(); j++)
            delete embeddings[i].second->at(j);
        delete embeddings[i].second;
    }
}

int find_in_occ(int id, occ& occ1)
{
    auto itr=find(occ1.begin(),occ1.end(),id);
    if(itr==occ1.end())
        return -1;
    return itr-occ1.begin();
}


bool lt_occ(occ& v1, occ& v2)
{
    for (int i = 0; i < v1.size(); i++)
        if (v1[i] < v2[i])
            return true;
    return false;
};

vector<int> pattern::gen_code()
{
    if (!this->code.empty())
        return code;
    occ min_occ;
    occ tmp;
    map<int, int> gnid2pnid;
    for (int i = 0; i < this->size; i++)
        min_occ.push_back(SHRT_MAX);
    vector<occ*>::iterator vit;
    vector<occ*>* vp = this->embeddings[0].second;
    for (vit = vp->begin(); vit != vp->end(); vit++)
    {
        tmp = *(*vit);
        sort(tmp.begin(), tmp.end());
        if (lt_occ(tmp, min_occ))
        {
            min_occ = tmp;
            gnid2pnid.clear();

            for (int i = 0; i < (*(*vit)).size(); i++)
                gnid2pnid[(*(*vit))[i]] = i;
        }
    }
    vector<int> n2o;
    for (int i = 0; i < min_occ.size(); i++)
        n2o.push_back(gnid2pnid[min_occ[i]]);
    int cur = 0;
    for (int i = 0; i < min_occ.size(); i++)
    {
        for (int j = 0; j < i; j++)
        {
            code.push_back (matrix[n2o[i]][n2o[j]]);
        }
        code.push_back(matrix[n2o[i]][n2o[i]]);
    }

    return code;
}


bool find_edge(int nid2, int elabel, vector<pair<int, int> >& vadj)
{
    for (int i = 0; i < vadj.size(); i++)
        if (vadj[i].first == nid2 && vadj[i].second == elabel)
            return true;
    return false;
}

vector<pattern *> *pattern::filter_patterns(vector<pattern *> *patterns)
{
    vector<pattern *> *filtered_patterns = new vector<pattern *>;

    for (auto itr =patterns->begin(); itr != patterns->end(); itr++)
    {
        pattern *temp = *itr;
        update_pattern_score(temp);

        if ((temp)->score_precise < this->score_precise)
        {
            (temp)->size = 0;
            delete temp;
            continue;
        }

        if (temp->score_precise / (float)(temp->edge_size) <
            this->score_precise / (float)(edge_size))
            (temp)->momentum = this->momentum - M_LOSS;
        else
            temp->momentum = this->momentum + M_GAIN;
        if (temp->momentum <= 0)
        {
            temp->size = 0;
            delete temp;
            continue;
        }
        for (int i = 0; i < size; i++)
            dead_node[i] = !is_alive[i];
        temp->dead_node = this->dead_node;
        if (temp->size > this->size)
            temp->dead_node.push_back(false);

        temp->gen_code();
        if (pattern_code.find(temp->code) != pattern_code.end())
        {
            temp->size = 0;
            delete temp;
            continue;
        }

        filtered_patterns->pb(temp);
    }
    return filtered_patterns;
}

// returns all possible patterns which can be extended in a single step from the original pattern
vector<pattern *> *pattern::extend_pattern()
{
    // vector<bool> dead_node(pat->size, false);
    is_alive.clear();
    is_alive.resize(size, false);

    vector<pattern *> *patterns_produced = new vector<pattern *>;
    map<int, int> extCodes;
    int embed_iterator = -1;
    for (auto itr : embeddings)
    {
        embed_iterator++;
        int gid = itr.x;

        int occurence_iterator = -1;
        vector<occ*>& voccs2=*(itr.y); 
        for (auto itr1 : voccs2)
        {
            ++occurence_iterator;
            occ *occurence = itr1;

            for (int i = 0; i < occurence->size(); i++)
            {
                if (dead_node[i])
                    continue;

                int uNode = occurence->at(i);
                int uPos = i;
                // cout << uNode << "****" << graph_database[gid]->adjList.size()<<endl;
                vector<pair<int, int>> &u_adj = (graph_database[gid]->adjList)[uNode];
                // cout<<u_adj.size()<<"size"<<endl;
                for (int j = 0; j < u_adj.size(); j++)
                {
                    // cout<<j<<endl;
                    int vNode = u_adj[j].x;
                    int edgeLabel = u_adj[j].y;
                    int vPos = find_in_occ(vNode, *occurence);
                    bool isInternal = (vPos != -1);
                    bool isEdgeAlreadyPresent = (isInternal) && (this->matrix[uPos][vPos] != 0);

                    if (isEdgeAlreadyPresent)
                        continue;
                    if (!isInternal)
                        vPos = graph_database[gid]->labels[vNode];

                    int extCode = get_hash_from_extension(isInternal, uPos, edgeLabel, vPos);
                    bool isExtensionAlreadyProduced = (extCodes.find(extCode) != extCodes.end());

                    pattern *new_pat;

                    if (!isExtensionAlreadyProduced)
                    {
                        if(gid>=positive_graph_count)
                            continue;
                        extCodes[extCode]=patterns_produced->size();
                        new_pat = new pattern;
                        patterns_produced->pb(new_pat);

                        new_pat->size = this->size + !(isInternal);
                        new_pat->edge_size = this->edge_size + 1;

                        new_pat->matrix = new int *[new_pat->size];
                        for (int i = 0; i < new_pat->size; i++)
                            new_pat->matrix[i] = new int[new_pat->size];
                        for (int i = 0; i < this->size; i++)
                            for (int j = 0; j < this->size; j++)
                                new_pat->matrix[i][j] = this->matrix[i][j];
                        if (isInternal)
                        {
                            new_pat->matrix[uPos][vPos] = edgeLabel;
                            new_pat->matrix[vPos][uPos] = edgeLabel;
                        }
                        else
                        {
                            int last = new_pat->size - 1;
                            for (int i = 0; i < new_pat->size; i++)
                            {
                                new_pat->matrix[last][i] = 0;
                                new_pat->matrix[i][last] = 0;
                            }
                            new_pat->matrix[uPos][last] = edgeLabel;
                            new_pat->matrix[last][uPos] = edgeLabel;
                            new_pat->matrix[last][last] = vPos;
                        }
                    }

                    else
                    {
                        new_pat = patterns_produced->at(extCodes[extCode]);
                    }
                    update_gids(gid < positive_graph_count, gid, new_pat->pgids, new_pat->ngids);

                    vector<occ *> *voccs;
                     if (new_pat->embeddings.size() == 0 || new_pat->embeddings[new_pat->embeddings.size() - 1].first < gid)
                    {
                        voccs = new vector<occ *>;
                        new_pat->embeddings.push_back(pair<int, vector<occ *> *>(gid, voccs));
                    }
                    //new pattern's current occs is voccs
                    else
                        voccs = new_pat->embeddings[new_pat->embeddings.size() - 1].second;
                    occ *nocc = new occ;
                    voccs->push_back(nocc);
                    occ *oocc = (*(embeddings[embed_iterator].second))[occurence_iterator];
                    for (int k = 0; k < oocc->size(); k++)
                        nocc->push_back((*oocc)[k]);
                    if (!isInternal)
                    {
                        nocc->push_back(vNode);
                    }
                    is_alive[i] = true;
                }
            }
        }
    }
    return filter_patterns(patterns_produced);
}