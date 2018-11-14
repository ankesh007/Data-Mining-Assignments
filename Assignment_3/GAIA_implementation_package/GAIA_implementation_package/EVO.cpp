/* 
 * File:   EVO.cpp
 * Author: njin
 * 
 * Created on October 10, 2009, 7:30 PM
 */

#include <map>
#include "EVO.h"
#include <math.h>
#include <algorithm>

extern string input_path_str;	//global variable, input directory
extern string output_path_str;	//global variable, output directory

/*compare the discrimination power of two features*/
bool cmp_feature(feature* f1, feature* f2)
{
    return f1->score_precise > f2->score_precise;
}

/*calculate the normalized accuracy based on numbers of false positives and false negatives*/
float EVO::norm_acc(int mis_pos, int mis_neg)
{
    int cpos = pos_num - mis_pos;
    int cneg = graphs.size() - pos_num - mis_neg;
    float sen = (float)cpos / (float)pos_num;
    float spe = (float)cneg / (float)(graphs.size() - pos_num);
    return (sen+spe)/2;    
}

/*select feature subgraph patterns to generate classification rules based on the selected patterns;
 normalized accuracy of the classification rules is maximized for the input graphs*/
void EVO::feature_selection(vector<feature*>& features)
{
    vector<bool> has_feature;
    vector<bool> covered;
    int mis_pos = pos_num;
    int mis_neg = 0;
    int delta_mis_pos, delta_mis_neg;
    float nacc = norm_acc(mis_pos, mis_neg);
    for (int i = 0; i < pos_num; i++)
        covered.push_back(false);
    for (int i = 0; i < this->graphs.size(); i++)
        has_feature.push_back(false);
    vector<feature*> sorted_features;
    for (int i = 0; i < this->graph_features.size(); i++)
        sorted_features.push_back(graph_features[i]);

    sort(sorted_features.begin(), sorted_features.end(), cmp_feature);

    for (int i = 0; i < sorted_features.size(); i++)
    {
        if (sorted_features[i]->pgids.size() == 0)
            continue;
        delta_mis_pos = 0;
        delta_mis_neg = 0;
        for (int j = 0; j < sorted_features[i]->pgids.size(); j++)
            if (!has_feature[(sorted_features[i]->pgids)[j]])
                delta_mis_pos++;
        for (int j = 0; j < sorted_features[i]->ngids.size(); j++)
            if (!has_feature[(sorted_features[i]->ngids)[j]])
                delta_mis_neg++;
        float tmp = norm_acc(mis_pos-delta_mis_pos, mis_neg+delta_mis_neg);
        if (delta_mis_pos == 0)
            continue;
        if (tmp > nacc)
        {
            nacc = tmp;
            features.push_back(sorted_features[i]);
            mis_pos -= delta_mis_pos;
            mis_neg += delta_mis_neg;
            for (int j = 0; j < sorted_features[i]->pgids.size(); j++)
                has_feature[(sorted_features[i]->pgids)[j]] = true;
            for (int j = 0; j < sorted_features[i]->ngids.size(); j++)
                has_feature[(sorted_features[i]->ngids)[j]] = true;
        }
    }

    mis_pos = 0;
    mis_neg = 0;

    for (int i = 0; i < graphs.size(); i++)
        if (i < pos_num && !has_feature[i])
            mis_pos++;
        else if (i >= pos_num && has_feature[i])
            mis_neg++;
    cout<<"mis_pos: "<<mis_pos<<" mis_neg: "<<mis_neg<<endl;

    cout<<"normalized accuracy in training set: "<<nacc<<endl;
}

/*output selected feature subgraph patterns to a file, including the information such as 
 adjacency matrix, positive frequency and negative frequency*/
void EVO::fout_features()
{
    string fname = output_path_str + this->fprefix;
    string fname2 = fname;
    fname.append(PF_SUFFIX);
    fname2.append(FF_SUFFIX);
    ofstream out(fname.c_str());
    ofstream fout(fname2.c_str());
    vector<feature*> features;
    short pid = 0;

    feature_selection(features);

    out<<"rules: "<<features.size()<<endl;
    for (int i = 0; i < features.size(); i++)
    {
        double freq = (double)(features[i]->pgids.size())/(double)pos_num;
        double bg_freq = (double)(features[i]->ngids.size())/(double)(graphs.size()-pos_num);

        out<<"size: 1 id: ";
        if (features[i]->size != 0)
            out<<(pid++)<<" ";
        else
            for (int j = 0; j < features[i]->codes.size(); j++)
                out<<(pid++)<<" ";
        out<<"freq: "<<freq<<" bg_freq: "<<bg_freq<<endl;
    }

    pid = 0;
    for (int i = 0; i < features.size(); i++)
    {
        double freq = (double)(features[i]->pgids.size())/(double)pos_num;
        double bg_freq = (double)(features[i]->ngids.size())/(double)(graphs.size()-pos_num);
        if (features[i]->size != 0)
        {
            out<<"id: "<<(pid++)<<endl;
            out<<"adj matrix: "<<features[i]->size<<endl;
            this->fout_feature_matrix(features[i], out);
        }
        else
            for (int j = 0; j < features[i]->codes.size(); j++)
            {
                out<<"id: "<<(pid++)<<endl;
                short m_size = features[i]->codes[j]->size();
                m_size = sqrt(2*m_size-1);
                out<<"adj matrix: "<<m_size<<endl;
                this->fout_feature_matrix(*(features[i]->codes[j]), m_size, out);
            }

        out<<"neg_freq: "<<bg_freq<<endl;
        out<<"pos freq: "<<freq<<endl;
    }

    for (int i = 0; i < features.size(); i++)
    {
        fout<<"feature_size: "<<features[i]->size<<endl;
        fout<<"feature_code_size: "<<features[i]->code.size()<<endl;
        for (int j = 0; j < features[i]->code.size(); j++)
            fout<<features[i]->code[j]<<" ";
        fout<<endl;
        fout<<"feature_score: "<<features[i]->score_precise<<endl;
        fout<<"pgids_size: "<<features[i]->pgids.size()<<endl;
        for (int j = 0; j < features[i]->pgids.size(); j++)
            fout<<features[i]->pgids[j]<<" ";
        fout<<endl;
        fout<<"ngids_size: "<<features[i]->ngids.size()<<endl;
        for (int j = 0; j < features[i]->ngids.size(); j++)
            fout<<features[i]->ngids[j]<<" ";
        fout<<endl;
    }
}

/*output the svm input file for the training dataset*/
void EVO::fout_training_matrix()
{
    string fname = output_path_str + this->fprefix;
    fname.append(TF_SUFFIX);
    ofstream out(fname.c_str());
    pattern_index tmp_idx;
    vector<feature*> features;
    cout<<graphs.size()<<"****"<<graph_features.size()<<endl;
    cout<<this->graph_features.size()<<endl;

    for (int i = 0; i < this->graph_features.size(); i++)
    {
        if (graph_features[i]->code.size() == 0)
        {
            graph_features[i]->code = graph_features[i]->pgids;
        }
        if (tmp_idx.find(graph_features[i]->code))
            continue;
        else
        {
            tmp_idx.insert(graph_features[i]->code);
            features.push_back(graph_features[i]);
        }
    }

    bool** svm_matrix;
    svm_matrix = new bool*[graphs.size()];
    for (int i = 0; i < graphs.size(); i++)
    {
        svm_matrix[i] = new bool[features.size()];
        for (int j = 0; j < features.size(); j++)
            svm_matrix[i][j] = false;
    }

    for (int i = 0; i < features.size(); i++)
    {
        for (int j = 0; j < features[i]->pgids.size(); j++)
        {
            int x = (features[i]->pgids)[j];
            int y = i;
            svm_matrix[x][y] = true;
        }
        for (int j = 0; j < features[i]->ngids.size(); j++)
        {
            int x = (features[i]->ngids)[j];
            int y = i;
            svm_matrix[x][y] = true;
        }
    }

    for (int i = 0; i < graphs.size(); i++)
    {
        if (i < pos_num)
            out<<"+1 ";
        else
            out<<"-1 ";
        for (int j = 0; j < features.size(); j++)
            if (svm_matrix[i][j])
                out<<(j+1)<<":1.0 ";
        out<<endl;
    }
}

/*output the size, positive support and negative support information of the 
 resulting subgraph patterns without the adjacency matrices*/
void EVO::fout_basic_result()
{
    string fname = output_path_str + this->fprefix;
    fname.append(BF_SUFFIX);
    ofstream out(fname.c_str());

    pattern_index tmp_idx;
    for (int i = 0; i < this->graph_features.size(); i++)
    {
        if (tmp_idx.find(graph_features[i]->code))
            continue;
        else
            tmp_idx.insert(graph_features[i]->code);
        out<<"size: "<<graph_features[i]->size<<endl;
        fout_feature_matrix(i, out);
        out<<"pgids_size: "<<graph_features[i]->pgids.size()<<endl;
        for (int j = 0; j < graph_features[i]->pgids.size(); j++)
            out<<graph_features[i]->pgids[j]<<" ";
        out<<endl;
        out<<"ngids_size: "<<graph_features[i]->ngids.size()<<endl;
        for (int j = 0; j < graph_features[i]->ngids.size(); j++)
            out<<graph_features[i]->ngids[j]<<" ";
        out<<endl;
    }
}

/*output to a file the adjacency matrix of the feature subgraph pattern 
 for a certain positive graph*/
void EVO::fout_feature_matrix(short gid, ofstream& fout)
{
    vector<short>& code = this->graph_features[gid]->code;
    short size = this->graph_features[gid]->size;
    short** matrix = new short*[size];
    for (int i = 0; i < size; i++)
        matrix[i] = new short[size];
    int cur = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j <= i; j++)
            matrix[i][j] = code[cur++];
    }
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
            matrix[i][j] = matrix[j][i];

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            fout<<matrix[i][j]<<" ";
        fout<<endl;
    }
}

/*output to a file the adjacency matrix of a subgraph pattern, given its code*/
void EVO::fout_feature_matrix(vector<short>& code, short size, ofstream& fout)
{
    short** matrix = new short*[size];
    for (int i = 0; i < size; i++)
        matrix[i] = new short[size];
    int cur = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j <= i; j++)
            matrix[i][j] = code[cur++];
    }
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
            matrix[i][j] = matrix[j][i];

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            fout<<matrix[i][j]<<" ";
        fout<<endl;
    }
}

/*given the pointer to a feature, output to a file the adjacency matrix of this feature*/
void EVO::fout_feature_matrix(feature* f, ofstream& fout)
{
    vector<short>& code = f->code;
    short size = f->size;
    short** matrix = new short*[size];
    for (int i = 0; i < size; i++)
        matrix[i] = new short[size];
    int cur = 0;
    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j <= i; j++)
            matrix[i][j] = code[cur++];
    }
    for (int i = 0; i < size; i++)
        for (int j = i + 1; j < size; j++)
            matrix[i][j] = matrix[j][i];

    for (int i = 0; i < size; i++)
    {
        for (int j = 0; j < size; j++)
            fout<<matrix[i][j]<<" ";
        fout<<endl;
    }
}

/*given the graph files with the prefix given in the parameter,
 mine discriminative subgraph patterns based on the algorithm GAIA*/
EVO::EVO(string fprefix) {
    //cout<<clock()<<endl;
    this->fprefix = fprefix;
    features_min_score = 0.0;
    for (int i = 0; i < pos_num; i++)
    {
        feature* fp = new feature;
        
        candidate_lists.push_back(new candidate_list);
        graph_features.push_back(fp);
        feature_updated.push_back(false);
    }

    if (!read_graphs())
        return;
    cout<<"Edge Database****"<<edges.size()<<endl;

    init_edges();

    for (int i = 0; i < iter_num; i++)
    {
        // for(int j=0;j<pos_num;j++)
        //     cout<<j<<" "<<this->candidate_lists[j]->length<<endl;
        evolution();
    }
    cout<<"time elapsed: "<<((double)clock()/(double)1000000.0)<<" seconds"<<endl;
}

/*read the input graphs; 
 assumes the gids in both input files are in increasing order*/
bool EVO::read_graphs()
{
    #ifdef BALANCED
    cout<<"balanced dataset"<<endl;
    #endif
    string nfname = input_path_str + fprefix;
    string efname = input_path_str + fprefix;
    nfname.append(NF_SUFFIX);
    efname.append(EF_SUFFIX);
    ifstream nin(nfname.c_str());
    ifstream ein(efname.c_str());
    if (!nin.is_open() || !ein.is_open())
        return false;

    string info;
    short pre_gid, gid, nid, nid1, nid2, label;

    pre_gid = -1;
    graph* tmp_g = NULL;
    // cout<<nfname<<endl;
    // cout<<efname<<endl;
    while (nin>>info>>gid>>nid>>label)
    {
        // cout<<info<<" "<<gid<<" "<<nid<<" "<<label<<endl;
        #ifdef BALANCED
        if (gid >= pos_num * 2)
            break;
        #endif
        if (gid != pre_gid)
        {
            if (gid != pre_gid + 1)
            {
                cout << "missing graph before " << gid <<endl;
                return false;
            }
            tmp_g = new graph;
            tmp_g->info = info;
            graphs.push_back(tmp_g);
            pre_gid = gid;
        }
        if (nid != tmp_g->nodes.size())
        {
            cout << "missing node before " << nid << " in graph " << gid <<endl;
            return false;
        }
        tmp_g->nodes.push_back(label);
    }//reading the node file

    pre_gid = -1;
    while (ein>>info>>gid>>nid1>>nid2>>label)
    {
        #ifdef BALANCED
        if (gid >= pos_num * 2)
            break;
        #endif
        if (gid < 0 || gid >= graphs.size())
        {
            cout<<"gid "<<gid<<" not found in the node file"<<endl;
            return false;
        }
        if (gid != pre_gid)
        {
            if (gid < pre_gid)
            {
                cout<<"graphs in edge file are not in order: "<<gid<<endl;
                return false;
            }
            pre_gid = gid;
        }
        if (nid1 >= graphs[gid]->nodes.size()
                || nid2 >= graphs[gid]->nodes.size())
        {
            cout<<"undefined node "<<nid1<<" or "<<nid2<<" in graph "<<gid<<endl;
            return false;
        }
        if (graphs[gid]->adjList.size() == 0)
            for (int i = 0; i < graphs[gid]->nodes.size(); i++)
                graphs[gid]->adjList.push_back(NULL);
        if (graphs[gid]->adjList[nid1] == NULL)
            graphs[gid]->adjList[nid1] = new vector<pair<short, short> >;
        if (graphs[gid]->adjList[nid2] == NULL)
            graphs[gid]->adjList[nid2] = new vector<pair<short, short> >;
        graphs[gid]->adjList[nid1]->push_back(pair<short, short>(nid2, label));
        graphs[gid]->adjList[nid2]->push_back(pair<short, short>(nid1, label));
        short l1 = graphs[gid]->nodes[nid1];
        short l2 = graphs[gid]->nodes[nid2];
        insert_edge(gid, nid1, nid2, l1, l2, label);
    }
    neg_num = this->graphs.size() - pos_num - test_samples;
    return true;
}

//destructor
EVO::~EVO() {
}
void print_pattern(pattern *p)
{
    cout << p->pgids.size() << " " << p->ngids.size() << " " << p->score_precise << " " << p->score_binned << endl;
}

/*insert an edge into the edge index*/
void EVO::insert_edge(int gid, short n1, short n2, short l1, short l2, short le)
{
    short tmp;
    bool identical_nodes = false;
    if (l1 > l2)
    {
        tmp = l1;
        l1 = l2;
        l2 = tmp;
        tmp = n1;
        n1 = n2;
        n2 = tmp;
    }
    else if (l1 == l2)
        identical_nodes = true;
    string tmp_code;
    char s[4];
    s[0] = (char)l1;
    s[1] = (char)le;
    s[2] = (char)l2;
    s[3] = 0;
    tmp_code.append(s);

    map<string, pattern*>::iterator mit = this->edges.find(tmp_code);
    if (mit == edges.end())
    {
        if (gid >= pos_num)
            return;
        pattern* p = new pattern;
#ifdef MOMENTUM
        p->momentum = INITIAL_M;
#endif
        p->code.push_back(l1);
        p->code.push_back(le);
        p->code.push_back(l2);
        edges[tmp_code] = p;
        p->size = 2;
        p->edge_size = 1;
        p->matrix = new short*[2];
        for (int i = 0; i < 2; i++)
            p->matrix[i] = new short[2];
        #ifdef DEAD_NODE
        p->dead_node.push_back(false);
        p->dead_node.push_back(false);
        #endif
        p->matrix[0][0] = l1;
        p->matrix[1][1] = l2;
        p->matrix[0][1] = le;
        p->matrix[1][0] = le;
        mit = this->edges.find(tmp_code);
    }

    //assumes the gids in both input files are in increasing order
    if (gid < pos_num &&
            (mit->second->pgids.size() == 0
             || gid != mit->second->pgids[mit->second->pgids.size()-1]))
        mit->second->pgids.push_back(gid);
    else if (gid >= pos_num &&
            (mit->second->ngids.size() == 0
             || gid != mit->second->ngids[mit->second->ngids.size()-1]))
        mit->second->ngids.push_back(gid);
    
    vector<pair<short, vector<occ*>* > >& emb = mit->second->embeddings;
    occ* nocc = new occ;
    nocc->push_back(n1);
    nocc->push_back(n2);
    if (emb.size() == 0 || emb[emb.size()-1].first != gid)
    {
        emb.push_back(pair<int, vector<occ*>* >(gid, NULL));
        emb[emb.size()-1].second = new vector<occ*>;
    }
    emb[emb.size()-1].second->push_back(nocc);
    //if the two nodes have the same label, then the occurrence should
    //be inserted twice: once is n1-n2 and once is n2-n1. otherwise
    //the embedding misses some edge occurrences and is incomplete
    if (identical_nodes)
    {
        nocc = new occ;
        nocc->push_back(n2);
        nocc->push_back(n1);
        emb[emb.size()-1].second->push_back(nocc);
    }
}

//pattern candidate migration
/*returns whether the pattern survived,
 it also updates features*/
bool EVO::distribute_pattern(pattern* p)
{
    int gid;
    bool res;
#ifndef BIAS_DISTRIBUTION
    gid = p->pgids[rand() % p->pgids.size()];
    /*
     * if the pattern is only found in no neg graph,
     * there's no need to extend it or consider its co-occurrences for
     * the score cannot be improved. However, having this conditional insertion
     * decreases the mining speed for there are more candidates to consider
     * whose superpatterns can still improve score; having this conditional
     * insertion can increase average score of the final features
    */
    //if (p->ngids.size() != 0)
        res = this->candidate_lists[gid]->insert(p);
#endif
    bool has_updated = false;
    if (p->score_precise > this->features_min_score)
    {
        for (int i = 0; i < p->pgids.size(); i++)
            if (graph_features[p->pgids[i]] == NULL)
                graph_features[p->pgids[i]] = new feature(p);
            else if (p->score_precise > this->graph_features[p->pgids[i]]->score_precise)
            {
                has_updated = true;
                delete graph_features[p->pgids[i]];
                graph_features[p->pgids[i]] = new feature(p);
            }
            
        if (has_updated)
        {
            features_min_score = 100.0;
            for (int i = 0; i < pos_num; i++)
            {
                if (graph_features[i] == NULL)
                {
                    if (features_min_score > 0.0)
                        features_min_score = 0.0;
                    continue;
                }
                if (graph_features[i]->score_precise < features_min_score)
                    features_min_score = graph_features[i]->score_precise;
            }
        }
    }
#ifdef POST_EXTENSION_PRUNING
    bool has_potential = false;
    float tmp_score = calc_score(p->pgids.size(), 0, p->edge_size+1);
    for (int i = 0; i < p->pgids.size(); i++)
        if (tmp_score > graph_features[p->pgids[i]]->score_precise)
        {
            has_potential = true;
            break;
        }
    if (!has_potential)
    {
        p->has_potential = has_potential;
    }
#endif
#ifdef BIAS_DISTRIBUTION
    int min_score_sum = INT_MAX;
    int min_score_sum_id = -1;
    for (int i = 0; i < p->pgids.size(); i++)
    {        
            if (candidate_lists[p->pgids[i]]->score_sum < min_score_sum)
            {
                min_score_sum = candidate_lists[p->pgids[i]]->score_sum;
                min_score_sum_id = p->pgids[i];
            }
    }
    if (min_score_sum_id >= 0)
    {
        res = this->candidate_lists[min_score_sum_id]->insert(p);
        cout<<min_score_sum_id<<" "<<"insertedhere"<<endl;

        if (!res)
            delete p;
    }
    else
    {
        delete p;
        return false;
    }

#endif

    return res;
}

//initialize edge index
/*codes for edges are not calculated because a pattern will
 never grow into an edge*/
void EVO::init_edges()
{
    map<string, pattern*>::iterator mit;
    for (mit = edges.begin(); mit != edges.end(); mit++)
    {
        #ifdef MIN_FREQUENCY
        float pfreq = (float)(mit->second->pgids.size()) / (float)pos_num;
        if (pfreq < pfreq_threshold)
        {
            delete mit->second;
            continue;
        }
        #endif
        mit->second->get_score();
        //checking the correctness of the embeddings of p
        #ifdef CHECK_EMBEDDING
        if (!mit->second->check_embeddings(graphs))
            cout<<"embedding error!"<<mit->second->size<<endl;
        #endif
        //end of checking
        if (mit->second->score_precise > 0)
            distribute_pattern(mit->second);
    }

    //debug only; output scores and supporting gids
    /*for (int i = 0; i < candidate_size; i++)
    {
        cout<<candidate_lists[3]->data[i]->pgids.size()<<endl;
        cout<<candidate_lists[3]->data[i]->ngids.size()<<endl;
        cout<<candidate_lists[3]->data[i]->score_precise<<endl;
    }*/
}

/*pick one subgraph pattern from the
 candidate list of positive graph whose ID is gid
 and extend this subgraph pattern*/
void EVO::pick_one_grow(int gid)
{
    pattern* seed_pattern = (candidate_lists[gid])->select_extension();
    if (seed_pattern == NULL)
        return;
    /*if the ngids.size == 0, the score cannot be improved, so there's
     no need to extend this pattern*/
    if (seed_pattern->ngids.size() == 0)
    {
        delete seed_pattern;
        return;
    }
    if (seed_pattern->size >= max_size)
    {
        delete seed_pattern;
        return;
    }
    float max_score = calc_score(seed_pattern->pgids.size(), 0,
            seed_pattern->edge_size+1);
#ifdef MOMENTUM
    #ifdef TOTAL_SCORE
    if (max_score/(float)(seed_pattern->edge_size+1) <
            seed_pattern->score_precise/(float)seed_pattern->edge_size
            && seed_pattern->momentum == 0)
    #endif
    #ifdef UNIT_SCORE
    if (max_score < seed_pattern->score_precise
            && seed_pattern->momentum == 0)
    #endif
    {
        delete seed_pattern;
        return;
    }
#endif
#ifndef MOMENTUM
    #ifdef TOTAL_SCORE
    if (max_score/(float)(seed_pattern->edge_size+1) <
            seed_pattern->score_precise/(float)seed_pattern->edge_size)
    #endif
    #ifdef UNIT_SCORE
    if (max_score < seed_pattern->score_precise)
    #endif
    {
        delete seed_pattern;
        return;
    }
#endif
#ifdef PRE_EXTENSION_PRUNING
    if (!has_potential_pre(seed_pattern))
    {
        delete seed_pattern;
        return;
    }
#endif
#ifdef POST_EXTENSION_PRUNING
    if (!seed_pattern->has_potential)
    {
        delete seed_pattern;
        return;
    }
#endif

    cout<<gid<<" "<<"Passed Tests"<<endl;
    print_pattern(seed_pattern);
    vector<pattern*>* new_patterns = seed_pattern->extend(graphs, pat_idx);
    cout<<new_patterns->size()<<endl;
    for(vector<pattern*>::iterator itr=new_patterns->begin();itr!=new_patterns->end();itr++)
    {
        // auto itr2=*itr;
        print_pattern(*itr);
        // cout<<(*itr)->pgids.size()<<" "<<(*itr)->ngids.size()<<" "<<(*itr)->score_precise<<" "<<(*itr)->score_binned<<endl;
    }

    delete seed_pattern;
    vector<pattern*>::iterator vit;
    for (vit = new_patterns->begin(); vit != new_patterns->end(); vit++)
        this->distribute_pattern(*vit);
}

#ifdef PRE_EXTENSION_PRUNING
/*if the score of a pattern p is no 
 * greater than the scores of p's supporting graphs' features, there's no need to
 * extend p*/
bool EVO::has_potential_pre(pattern* p)
{
    float max_score =
    calc_score(p->pgids.size(), 0, p->edge_size+1);
    for (int i = 0; i < p->pgids.size(); i++)
    {
        if (max_score > graph_features[i]->score_precise)
            return true;
    }
    return false;
}
#endif

//one call of evolution is one iteration
void EVO::evolution()
{
    #ifdef ECONOMIC_PICK_GROW
    for (int i = 0; i < pos_num; i++)
        feature_updated[i] = false;
    #endif
    // for (int i = 0; i < pos_num; i++)
    // {
    //     for (int j = 0; j < this->candidate_lists[i]->length;j++)
    //     {
    //         cout << this->candidate_lists[i]->data[j]->pgids.size() << " " << this->candidate_lists[i]->data[j]->ngids.size() << " " << this->candidate_lists[i]->data[j]->score_precise << " " << this->candidate_lists[i]->data[j]->score_binned<<endl;
    //         // for(auto itr:candidate_lists[i]->data[j]->pgids)
    //         //     cout<<itr<<" ";
    //         // cout<<endl;
    //         // for (auto itr : candidate_lists[i]->data[j]->ngids)
    //         //     cout << itr << " ";
    //         // cout<<endl;
    //     }
    // }
    // exit(0);

    for (int i = 0; i < pos_num; i++)
    {
        #ifdef ECONOMIC_PICK_GROW
        if (feature_updated[i])
            continue;
        #endif
        cout<<"Picking and growing"<<i<<endl;
        cout<<"Candidate List Size "<<candidate_lists[i]->length<<endl;
        pick_one_grow(i);
    }
}

