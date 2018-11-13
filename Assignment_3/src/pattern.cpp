#include <bits/stdc++.h>
#include "pattern.h"
#include "declaration.h"

extern double pfreq_threshold;
extern map<vector<int>, pattern *> pattern_code;

extension::~extension()
{
    for (int i = 0; i < this->ext_occs.size(); i++)
        delete ext_occs[i];
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
    for (auto itr:embeddings)
    {
        for (int j = 0; j < (itr.second)->size(); j++)
            delete itr.second->at(j);
        delete itr.second;
    }
}

vector<pattern*>* pattern::extend(vector<graph*>& graphs)
{
    is_alive.resize(size);
    vector<pattern*>* extended_patterns = new vector<pattern*>;
    cout<<"Here"<<endl;

    for (int i = 0; i < this->embeddings.size(); i++)
    {
        int gid = embeddings[i].first;
        vector<occ*>& voccs = *(embeddings[i].second);
        for (int j = 0; j < voccs.size(); j++)
            this->collect_ext(gid, i, *(voccs[j]), j, graphs);
    }
    map<int, extension*>::iterator mit;
    for (mit = extensions.begin(); mit != extensions.end(); mit++)
    {
        pattern* p = gen_new_pattern(*(mit->second), mit->first, graphs);
        delete mit->second;
        if (p != NULL)
        {
            extended_patterns->push_back(p);
        }
    }
    
    return extended_patterns;
}

//generate a new pattern based on the extension
//pattern_index is the index of all existing patterns
pattern* pattern::gen_new_pattern(extension& ext, int code, vector<graph*>& graphs)
{
    int source = EXTSOURCE(code);
    int drain = EXTDRAIN(code);
    int elabel = EXTEDGE(code);

    pattern* res = new pattern;
    res->pgids = ext.pgids;
    res->ngids = ext.ngids;

    if (ext.is_internal_ext)
        res->size = this->size;
    else
        res->size = this->size + 1;
    res->edge_size = this->edge_size + 1;
    
    update_pattern_score(res);
    if (res->score_precise < this->score_precise)
    {
        res->size = 0;
        delete res;
        return NULL;
    }

    if (res->score_precise/(float)(res->edge_size) <
            this->score_precise / (float)(edge_size))
    if (res->score_precise < this->score_precise)
        res->momentum = this->momentum - M_LOSS;
    else
        res->momentum = this->momentum + M_GAIN;
    if (res->momentum <= 0)
    {
        res->size = 0;
        delete res;
        return NULL;
    }
    for (int i = 0; i < size; i++)
            dead_node[i] = !is_alive[i];
    res->dead_node = this->dead_node;
    if (res->size > this->size)
        res->dead_node.push_back(false);

    //do the following only if the score increases

    //generating the matrix
    res->matrix = new int*[res->size];
    for (int i = 0; i < res->size; i++)
        res->matrix[i] = new int[res->size];
    for (int i = 0; i < this->size; i++)
        for (int j = 0; j < this->size; j++)
            res->matrix[i][j] = this->matrix[i][j];
    if (ext.is_internal_ext)
    {
        res->matrix[source][drain] = elabel;
        res->matrix[drain][source] = elabel;
    }
    else
    {
        int last = res->size - 1;
        for (int i = 0; i < res->size; i++)
        {
            res->matrix[last][i] = 0;
            res->matrix[i][last] = 0;
        }
        res->matrix[source][last] = elabel;
        res->matrix[last][source] = elabel;
        res->matrix[last][last] = drain;
    }

    //check the code after all occurrences in the first graph are found
    //continue generating the embeddings only if the pattern hasn't be visited before
    for (int i = 0; i < ext.ext_occs.size(); i++)
    {
        ext_occ* eop = ext.ext_occs[i];
        int gid = embeddings[eop->par_em_gid].first;
        int poid = eop->par_occ_id;
        int nid2 = eop->drain;
        vector<occ*>* voccs;
        if (res->embeddings.size() == 0
                || res->embeddings[res->embeddings.size()-1].first < gid)
        {
            if (res->embeddings.size() == 1)
            {
                res->gen_code();
                if (pattern_code.find(res->code)!=pattern_code.end())
                {
                    res->size = 0;
                    delete res;
                    return NULL;
                } 
                pattern_code[res->code]=0;
            }
            voccs = new vector<occ*>;
            res->embeddings.push_back(pair<int,vector<occ*>*>(gid, voccs));
        }
        //new pattern's current occs is voccs
        else
            voccs = res->embeddings[res->embeddings.size()-1].second;
        occ* nocc = new occ;
        voccs->push_back(nocc);
        occ* oocc = (*(embeddings[eop->par_em_gid].second))[poid];
        for (int k = 0; k < oocc->size(); k++)
            nocc->push_back((*oocc)[k]);
        if (!ext.is_internal_ext)
        {
            nocc->push_back(nid2);
            #ifdef CHECK_EMBEDDING
            if (graphs[gid]->labels[nid2] != drain)
                cout<<"embedding error in gen_new_pattern"<<endl;
            #endif
        }
    }

    res->gen_code();
    return res;
}

//find the subscript of a given id in a given occurrence
int find_in_occ(int id, occ& occ1)
{
    for (int i = 0; i < occ1.size(); i++)
        if (id == occ1[i])
            return i;
    return -1;
}

//find all possible extensions
void pattern::collect_ext(int gid, int par_em_gid, occ& occ1, int occ1_id, vector<graph*>& graphs)
{
    bool is_pos = gid < positive_graph_count;
    bool is_internal;
    map<int, extension*>::iterator mit;
    int nid1, nid2;
    int edge_label, drain_node;
    pair<int, int> pair_info;

    for (int i = 0; i < occ1.size(); i++)
    {
        if (dead_node[i])
        {
            continue;
        }
        nid1 = occ1[i];
        vector<pair<int, int>> &vadj = (graphs[gid]->adjList)[nid1];

        for (int j = 0; j < vadj.size(); j++)
        {
            pair_info = vadj[j];
            nid2 = pair_info.x;
            edge_label = pair_info.second;
            drain_node = find_in_occ(nid2, occ1);
            //if node2 is already in the pattern, drain_node=node id in the pattern
            is_internal = drain_node >= 0;
            if (is_internal && matrix[i][drain_node] != 0)
            {
                continue;
            }


            if (!is_internal)    //if node2 is not in the pattern, drain_node = node label
            {
                drain_node = graphs[gid]->labels[nid2];
            }
            int code = EXTCODE(is_internal, i, edge_label, drain_node);
            mit = this->extensions.find(code);
            if (mit == extensions.end())
            {
                if (!is_pos)
                    continue;   //this is an extension only found in negative graphs
                extension* ext_p = new extension;
                ext_p->is_internal_ext = is_internal;
                extensions[code] = ext_p;
                mit = extensions.find(code);
            }
            if (is_pos && (mit->second->pgids.size()==0 
                    || gid > mit->second->pgids[mit->second->pgids.size()-1]))
                mit->second->pgids.push_back(gid);
            if (!is_pos && (mit->second->ngids.size()==0
                    || gid > mit->second->ngids[mit->second->ngids.size()-1]))
                mit->second->ngids.push_back(gid);
            ext_occ* eop = new ext_occ;
            eop->par_em_gid = par_em_gid;
            eop->par_occ_id = occ1_id;
            eop->source = nid1;
            eop->drain = nid2;
            
            mit->second->ext_occs.push_back(eop);
            #ifdef CHECK_EMBEDDING
            drain_node = EXTDRAIN(mit->first);
            if (!is_internal && graphs[embeddings[eop->par_em_gid].first]->labels[eop->drain] != drain_node)
                cout<<"error in collecting extension occurrences"<<endl;
            #endif
            #ifdef DEAD_NODE
            is_alive[i] = true;
            #endif
        }
    }
}

//compare two occurrences lexicographically
//v1 and v2 are supposed to be sorted already;
//v1 and v2 must have the same size
bool lt_occ(occ& v1, occ& v2)
{
    for (int i = 0; i < v1.size(); i++)
        if (v1[i] < v2[i])
            return true;
    return false;
}

//generate the code of this pattern
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

//look for a certain edge extension in a vector of edge extensions
/*only the edge information needs to be checked; the node label
 of nid2 is checked when checking the matrix entry [nid2][nid2];
 edge label cannot be zero here*/
bool find_edge(int nid2, int elabel, vector<pair<int, int> >& vadj)
{
    for (int i = 0; i < vadj.size(); i++)
        if (vadj[i].first == nid2 && vadj[i].second == elabel)
            return true;
    return false;
}

//check if the embeddings of the pattern are correct
bool pattern::check_embeddings(vector<graph*>& graphs)
{
    for (int i = 0; i < embeddings.size(); i++)
    {
        int gid = embeddings[i].first;
        vector<occ*>& vocc = *(embeddings[i].second);
        for (int j = 0; j < vocc.size(); j++)
        {
            occ& occ1 = *(vocc[j]);
            for (int x = 0; x < size; x++)
                for (int y = 0; y <= x; y++)
                {
                    if (y == x && matrix[x][y] != graphs[gid]->labels[occ1[x]])
                        return false;
                    if (y != x && matrix[x][y] == 0)
                        continue;
                    if (x != y && matrix[x][y] != 0)
                    {
                        if (!find_edge(occ1[y], matrix[x][y], (graphs[gid]->adjList[occ1[x]])))
                            return false;
                    }
                }
        }
    }
    return true;
}

