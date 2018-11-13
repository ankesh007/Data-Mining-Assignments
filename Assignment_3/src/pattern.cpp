/* 
 * File:   pattern.cpp
 * Author: njin
 * 
 * Created on October 10, 2009, 7:05 PM
 */

#include <map>

#include "pattern.h"

extern double pfreq_threshold;	//global variable: positive frequency threshold

//destructor of class extension
extension::~extension()
{
    for (int i = 0; i < this->ext_occs.size(); i++)
        delete ext_occs[i];
}

//output to screen the adjacency matrix of the subgraph pattern
void pattern::cout_matrix()
{
    for (int i = 0; i < this->size; i++)
    {
        for (int j = 0; j < size; j++)
            cout << matrix[i][j] << " ";
        cout<<endl;
    }
}

//output to screen the code of the subgraph pattern
void pattern::cout_code()
{
    int cur = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j <= i; j++)
            cout<<code[cur++]<<" ";
        cout<<endl;
    }
}

//constructor of class pattern
pattern::pattern()
{
#ifdef MOMENTUM
    this->momentum = 0;
#endif
    this->code.clear();
    this->score_precise = 0.0;
    this->score_binned = 0;
    size = 0;
    has_potential = true;
}

//destructor of class pattern
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

//generate new subgraph patterns based on all possible extensions
//returns the vector of new subgraph patterns
vector<pattern*>* pattern::extend(vector<graph*>& graphs, pattern_index& pat_idx)
{
    #ifdef DEAD_NODE
    for (int i = 0; i < size; i++)
        is_alive.push_back(false);
    #endif
    vector<pattern*>* res = new vector<pattern*>;
    for (int i = 0; i < this->embeddings.size(); i++)
    {
        short gid = embeddings[i].first;
        vector<occ*>& voccs = *(embeddings[i].second);
        for (int j = 0; j < voccs.size(); j++)
            this->collect_ext(gid, i, *(voccs[j]), j, graphs);
    }
    map<int, extension*>::iterator mit;
    for (mit = extensions.begin(); mit != extensions.end(); mit++)
    {
        pattern* p = gen_new_pattern(*(mit->second), mit->first, pat_idx, graphs);
        delete mit->second;
        if (p != NULL)
        {
            //checking the correctness of the embeddings of p
            #ifdef CHECK_EMBEDDING
            if (!p->check_embeddings(graphs))
                cout<<"embedding error!"<<p->size<<endl;
            #endif
            //end of checking
            res->push_back(p);
        }
    }
    
    return res;
}

//generate a new pattern based on the extension
//pattern_index is the index of all existing patterns
pattern* pattern::gen_new_pattern(extension& ext, int code,
        pattern_index& pat_idx, vector<graph*>& graphs)
{
    int source = EXTSOURCE(code);
    int drain = EXTDRAIN(code);
    int elabel = EXTEDGE(code);

    pattern* res = new pattern;
    res->pgids = ext.pgids;
    res->ngids = ext.ngids;
    #ifdef MIN_FREQUENCY_EXTENSION
    float pfreq = (float)(res->pgids.size()) / (float)pos_num;
    if (pfreq < pfreq_threshold)
    {
        res->size = 0;
        delete res;
        return NULL;
    }
    #endif
    if (ext.is_internal_ext)
        res->size = this->size;
    else
        res->size = this->size + 1;
    res->edge_size = this->edge_size + 1;
    
    res->get_score();
#ifdef TOTAL_SCORE
    if (res->score_precise < this->score_precise)
#endif
#ifdef UNIT_SCORE
    if (res->score_precise*res->edge_size
            < this->score_precise*this->edge_size)
#endif
    {
        res->size = 0;
        delete res;
        return NULL;
    }

#ifdef MOMENTUM
    #ifdef TOTAL_SCORE
    if (res->score_precise/(float)(res->edge_size) <
            this->score_precise / (float)(edge_size))
    #endif
    #ifdef UNIT_SCORE
    if (res->score_precise < this->score_precise)
    #endif
        res->momentum = this->momentum - M_LOSS;
    else
        res->momentum = this->momentum + M_GAIN;
    if (res->momentum <= 0)
    {
        res->size = 0;
        delete res;
        return NULL;
    }
#endif
#ifndef MOMENTUM
    #ifdef TOTAL_SCORE
    if (res->score_precise/(float)(res->edge_size) <
            this->score_precise / (float)(edge_size))
    #endif
    #ifdef UNIT_SCORE
    if (res->score_precise < this->score_precise)
    #endif
    {
        res->size = 0;
        delete res;
        return NULL;
    }
#endif

    #ifdef DEAD_NODE
    for (int i = 0; i < size; i++)
            dead_node[i] = !is_alive[i];
    res->dead_node = this->dead_node;
    if (res->size > this->size)
        res->dead_node.push_back(false);
    #endif

    //do the following only if the score increases

    //generating the matrix
    res->matrix = new short*[res->size];
    for (int i = 0; i < res->size; i++)
        res->matrix[i] = new short[res->size];
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
        short gid = embeddings[eop->par_em_gid].first;
        short poid = eop->par_occ_id;
        short nid2 = eop->drain;
        vector<occ*>* voccs;
        if (res->embeddings.size() == 0
                || res->embeddings[res->embeddings.size()-1].first < gid)
        {
            if (res->embeddings.size() == 1)
            {
                res->gen_code();
                if (pat_idx.find(res->code))
                {
                    res->size = 0;
                    delete res;
                    return NULL;
                }
                pat_idx.insert(res->code);
            }
            voccs = new vector<occ*>;
            res->embeddings.push_back(pair<short,vector<occ*>*>(gid, voccs));
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
            if (graphs[gid]->nodes[nid2] != drain)
                cout<<"embedding error in gen_new_pattern"<<endl;
            #endif
        }
    }

    res->gen_code();
    return res;
}

//find the subscript of a given id in a given occurrence
int find_in_occ(short id, occ& occ1)
{
    for (int i = 0; i < occ1.size(); i++)
        if (id == occ1[i])
            return i;
    return -1;
}

//find all possible extensions
void pattern::collect_ext(short gid, short par_em_gid, occ& occ1, short occ1_id, vector<graph*>& graphs)
{
    bool is_pos = gid < pos_num;
    bool is_internal;
    map<int, extension*>::iterator mit;
    short nid1, nid2;
    short edge_label, drain_node;
    pair<short, short> pair_info;
    vector<pair<short, short> >* vadj;

    for (int i = 0; i < occ1.size(); i++)
    {
        #ifdef RANDOM_NODE_EXTENSION
        if (rand()%2 == 0)
            continue;
        #endif
        #ifdef DEAD_NODE
        if (dead_node[i])
        {
            continue;
        }
        #endif
        nid1 = occ1[i];
        vadj = (graphs[gid]->adjList)[nid1];
        for (int j = 0; j < vadj->size(); j++)
        {
            pair_info = (*vadj)[j];
            nid2 = pair_info.first;
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
                drain_node = graphs[gid]->nodes[nid2];
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
            if (!is_internal && graphs[embeddings[eop->par_em_gid].first]->nodes[eop->drain] != drain_node)
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
vector<short> pattern::gen_code()
{
    if (!this->code.empty())
        return code;
    occ min_occ;
    occ tmp;
    map<short, short> gnid2pnid;
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
    vector<short> n2o;
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

int correct_size(vector<short> &ngids)
{
    int coun=0;
    int x=ngids.size();
    for(int i=0;i<x;i++)
        if(ngids[i]<pos_num+neg_num)
            coun++;
    return coun;
}

//calculate score
void pattern::get_score()
{
    this->score_precise = calc_score(pgids.size(), correct_size(ngids), edge_size);
#ifdef TOTAL_SCORE
    #ifdef LOG_RATIO
    this->score_binned = (int)(score_precise*10.0) + 1;
    #endif
    #ifdef LOG_ODDS_RATIO
    this->score_binned = (int)(score_precise) + 1;
    #endif
    #ifdef RATIO
    this->score_binned = (int)(score_precise) + 1;
    #endif
    #ifdef GTEST
    this->score_binned = (int)(score_precise*10.0) + 1;
    #endif
#endif
#ifdef UNIT_SCORE
    #ifdef LOG_RATIO
    this->score_binned = (int)(score_precise*10.0) + 1;
    #endif
    #ifdef LOG_ODDS_RATIO
    this->score_binned = (int)(score_precise) + 1;  
    #endif
    #ifdef RATIO
    this->score_binned = (int)(score_precise) + 1;
    #endif
    #ifdef GTEST
    this->score_binned = (int)(score_precise*10.0) + 1;
    #endif
#endif
    if (score_binned < 0)
        score_binned = 0;
}

//look for a certain edge extension in a vector of edge extensions
/*only the edge information needs to be checked; the node label
 of nid2 is checked when checking the matrix entry [nid2][nid2];
 edge label cannot be zero here*/
bool find_edge(short nid2, short elabel, vector<pair<short, short> >& vadj)
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
        short gid = embeddings[i].first;
        vector<occ*>& vocc = *(embeddings[i].second);
        for (int j = 0; j < vocc.size(); j++)
        {
            occ& occ1 = *(vocc[j]);
            for (int x = 0; x < size; x++)
                for (int y = 0; y <= x; y++)
                {
                    if (y == x && matrix[x][y] != graphs[gid]->nodes[occ1[x]])
                        return false;
                    if (y != x && matrix[x][y] == 0)
                        continue;
                    if (x != y && matrix[x][y] != 0)
                    {
                        if (!find_edge(occ1[y], matrix[x][y], *(graphs[gid]->adjList[occ1[x]])))
                            return false;
                    }
                }
        }
    }
    return true;
}

