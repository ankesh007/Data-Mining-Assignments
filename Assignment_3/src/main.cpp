#include "EVO.h"
#include "graph.h"
#include "declaration.h"
#include "feature.h"
#include "edge_evolution.h"
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
map<string, pattern *> pattern_code;
vector<candidate_list *> candidate_lists;
vector<feature*> graph_features;
vector<bool> feature_updated;

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
                features_min_score=min(features_min_score,p->score_precise);                
            }
        }
    }
    int max_possible_for_pattern=get_score(p->pgids.size(),0);
    bool has_potential=false;
    for(auto itr:p->pgids)
    {
        if(graph_features[itr]->score_precise<max_possible_for_pattern)
        {
            has_potential=true;
            break;
        }
    }
    
    p->has_potential=p->has_potential&has_potential;

    int min_sum=1e9;
    int min_sum_id=-2;

    for(auto itr:p->pgids)
    {
        if(candidate_lists[itr]->score_sum<min_sum)
        {
            min_sum=candidate_lists[itr]->score_sum;
            min_sum_id=itr;
        }
    }
    if(min_sum_id<0)
        delete p;
    
    else
    {
        bool could_insert;
        could_insert = candidate_lists[min_sum_id]->insert(p);
        if(!could_insert) 
            delete p;
    }
}
bool has_potential_pre(pattern *p,float max_pattern_score)
{
    for (auto itr:p->pgids)
    {
        if (max_pattern_score > graph_features[itr]->score_precise)
            return true;
    }
    return false;
}

void evolve()
{
    for(int i=0;i<positive_graph_count;i++)
    {
        pattern *sampled_pattern=candidate_lists[i]->select_extension();

        if(sampled_pattern->ngids.size()==0 || sampled_pattern->size>=max_size)
        {
            delete sampled_pattern;
            return;
        }

        float max_pattern_score=get_score(sampled_pattern->pgids.size(),0);
        float best_score_peredge = max_pattern_score*1.0 / (sampled_pattern->edge_size + 1);
        float cur_score_peredge = sampled_pattern->score_precise*1.0/sampled_pattern->edge_size;
        if (best_score_peredge<cur_score_peredge && sampled_pattern->momentum==0)
        {
            delete sampled_pattern;
            return;
        }
        if(!has_potential_pre(sampled_pattern,max_pattern_score)|| !sampled_pattern->has_potential)
        {
            delete sampled_pattern;
            return;
        }
        vector<pattern *> *new_patterns = sampled_pattern->extend(graph_database, pattern_code);
        delete sampled_pattern;
        // vector<pattern *>::iterator vit;
        for (auto itr = new_patterns->begin(); itr != new_patterns->end(); itr++)
            migrate(*itr);
    }   
}

void pattern_evolution()
{
    edge_evolution();

    for(auto itr:pattern_code)
    {
        update_pattern_score(itr.y);
        migrate(itr.y);
    }
    
    while(iter_num--)
    {
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
    init();
    pattern_evolution();
    // output_feature();

	
    // double score_sum = 0.0;
    // for (int i = 0; i < positive_graph_count; i++)
    // {
    //     score_sum += test.graph_features[i]->score_precise;
	// 	/*if a positive graph has no discriminative subgraph pattern found by GAIA,
	// 	 the score of the feature of this positive graph is 0*/
    //     if (test.graph_features[i]->score_precise < 0.000001)
    //         cout<<"graph "<<i<<" has no discriminative feature"<<endl;
    // }
    // /*output to screen the average score of all the feature subgraph patterns*/
    // cout<<"average_score: "<<(score_sum/(double)positive_graph_count)<<endl;
    // cout<<"mining is completed successfully"<<endl;
	
    //     /*output the resulting feature subgraph patterns to file*/
    // test.fout_features();			
    // test.fout_training_matrix();
    
    // return EXIT_SUCCESS;
}

