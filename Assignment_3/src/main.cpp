// #include "EVO.h"
#include "graph.h"
#include "declaration.h"
#include "feature.h"
#include "edge_evolution.h"
#include "candidate_list.h"
#include <bits/stdc++.h>
using namespace std;

/* 
Some global values
 */
int positive_graph_count;				
int negative_graph_count;				
int candidate_size;			
int iter_num;				
int max_size=1000;				
double pfreq_threshold=0;		
int test_samples=0;
string node_filename = "node_file.txt";
string edge_filename = "edge_file.txt";
float features_min_score=0;
/* 
Database specific for evolution
 */
vector<graph *> graph_database;
map<vector<int>, pattern *> pattern_code;
map<string, pattern *> edge_code;
vector<candidate_list *> candidate_lists;
vector<feature*> graph_features;
vector<bool> feature_updated;

void fout_training_matrix()
{
    string fname = "train_test.svm";
    ofstream out(fname.c_str());
    map<vector<int>,pattern *> tmp_idx;
    vector<feature*> features;

    for (int i = 0; i < graph_features.size(); i++)
    {
        if (graph_features[i]->code.size() == 0)
        {
            graph_features[i]->code = graph_features[i]->pgids;
        }
        if (tmp_idx.find(graph_features[i]->code)!=tmp_idx.end())
            continue;
        else
        {
            tmp_idx[graph_features[i]->code]=0;
            features.push_back(graph_features[i]);
        }
    }

    bool** svm_matrix;
    svm_matrix = new bool*[graph_database.size()];
    for (int i = 0; i < graph_database.size(); i++)
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

    for (int i = 0; i < graph_database.size(); i++)
    {
        if (i < positive_graph_count)
            out<<"+1 ";
        else
            out<<"-1 ";
        for (int j = 0; j < features.size(); j++)
            if (svm_matrix[i][j])
                out<<(j+1)<<":1 ";
            else
                out<<(j+1)<<":0 ";
        out<<endl;
    }
}

void read_graph()
{
    ifstream node_filereader;
    node_filereader.open(node_filename);
    string graph_id;
    int graph_num,node_id,label;

    while(node_filereader>>graph_id>>graph_num>>node_id>>label)
    {
        if(graph_database.size()==graph_num)
        {
            graph *new_graph=new graph();
            graph_database.pb(new_graph);
        }
        graph_database[graph_num]->labels.pb(label);        
    }
    node_filereader.close();

    for(auto itr:graph_database)
        itr->adjList.resize(itr->labels.size());


    ifstream edge_filereader;
    edge_filereader.open(edge_filename);
    int  node_id1,node_id2,edge_label;

    while (edge_filereader >> graph_id >> graph_num >> node_id1 >> node_id2 >> label)
    {
        if (graph_database.size() == graph_num)
        {
            graph *new_graph = new graph();
            graph_database.pb(new_graph);
        }
        graph_database[graph_num]->adjList[node_id1].pb({node_id2,label});
        graph_database[graph_num]->adjList[node_id2].pb({node_id1, label});
    }
    edge_filereader.close();
    negative_graph_count=graph_database.size();
    negative_graph_count-=positive_graph_count+test_samples;
}

void init()
{
    features_min_score = 0.0;
    for (int i = 0; i < positive_graph_count; i++)
    {
        feature *fp = new feature;
        candidate_lists.push_back(new candidate_list);
        graph_features.push_back(fp);
        feature_updated.push_back(false);
    }
}

void update_feature_min_score(pattern *p,int needed)
{
    if(needed!=1)
        return;
    features_min_score=1e9;
    for(int i=0;i<positive_graph_count;i++)
        features_min_score = min(features_min_score, graph_features[i] == NULL ? 0 : graph_features[i]->score_precise);
}

void migrate(pattern *p)
{    
    if(p->score_precise>features_min_score)
    {
        for(auto itr:p->pgids)
        {       
            if(graph_features[itr]==NULL || graph_features[itr]->score_precise<p->score_precise)
            {
                if(graph_features[itr])
                    delete graph_features[itr];
                graph_features[itr]=new feature(p);
            }
        }
        update_feature_min_score(p,1);
    }
    
    int max_possible_for_pattern=get_score(p->pgids.size(),0);
    bool has_potential=false;
    float val=1e9;
    int min_sum=1e9;
    int min_sum_id=-2;

    for(auto itr:p->pgids)
    {
        val = min(val, graph_features[itr]->score_precise);
        if(candidate_lists[itr]->score_sum<min_sum)
        {
            min_sum=candidate_lists[itr]->score_sum;
            min_sum_id=itr;
        }
    }

    if (val < max_possible_for_pattern)
        has_potential=true;

    p->has_potential=p->has_potential&has_potential;

    if(min_sum_id<0)
        delete p;
    
    else
    {
        bool could_insert;
        could_insert = candidate_lists[min_sum_id]->insert(p);
        // cout<<min_sum_id<<" "<<"insertedhere"<<endl;
        if(!could_insert) 
            delete p;
    }
}

bool has_potential_pre(pattern *p,float max_pattern_score)
{
    int i=0;
    for (auto itr:p->pgids)
    {
        if (max_pattern_score > graph_features[i++]->score_precise)
            return true;
    }
    return false;
}
void print_pattern(pattern *p)
{
    cout << p->pgids.size() << " " << p->ngids.size() << " " << p->score_precise << " " << p->score_binned << endl;
}

void evolve()
{
    for(int i=0;i<positive_graph_count;i++)
    {
        pattern *sampled_pattern=candidate_lists[i]->select_extension();
        if (sampled_pattern == NULL)
            continue;

        if(sampled_pattern->ngids.size()==0 || sampled_pattern->size>=max_size)
        {
            delete sampled_pattern;
            continue;
        }

        float max_pattern_score=get_score(sampled_pattern->pgids.size(),0);
        float best_score_peredge = max_pattern_score*1.0 / (sampled_pattern->edge_size + 1);
        float cur_score_peredge = sampled_pattern->score_precise*1.0/sampled_pattern->edge_size;
        if (best_score_peredge<cur_score_peredge && sampled_pattern->momentum==0)
        {
            delete sampled_pattern;
            continue;
        }
        if(!has_potential_pre(sampled_pattern,max_pattern_score)|| !sampled_pattern->has_potential)
        {
            delete sampled_pattern;
            continue;
        }
        vector<pattern *> *new_patterns = sampled_pattern->extend_pattern();
        delete sampled_pattern;
        for (auto itr = new_patterns->begin(); itr != new_patterns->end(); itr++)
        {
            migrate(*itr);
        }
    }   
}

void pattern_evolution()
{
    edge_evolution();

    for(auto itr:edge_code)
    {
        update_pattern_score(itr.y);
        if(itr.y->score_precise>0)
            migrate(itr.y);
    }

    while(iter_num--)
    {
        cout<<"Evolution:"<<iter_num<<endl;
    
        bool flag=true;
        for(auto itr:candidate_lists)
        {
            if(itr->length)
            {
                flag=false;
                break;
            }
        }
        if(flag)
            break;
        
        evolve();
    }
}

int main(int argc, char** argv)
{
    positive_graph_count = stoi(string(argv[1]));
    iter_num = stoi(string(argv[2]));
    candidate_size = stoi(string(argv[3]));
    test_samples = stoi(string(argv[4]));
        
    srand(time(NULL));

    read_graph();
    cout<<"Read Graph"<<endl;
    cout<<positive_graph_count<<" "<<negative_graph_count<<" "<<test_samples<<endl;
    init();
    cout << "Init" << endl;
    pattern_evolution();		
    fout_training_matrix();
}