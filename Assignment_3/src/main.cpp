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



/*compare the discrimination power of two features*/
bool cmp_feature(feature* f1, feature* f2)
{
    return f1->score_precise > f2->score_precise;
}

// /*calculate the normalized accuracy based on numbers of false positives and false negatives*/
float norm_acc(int mis_pos, int mis_neg)
{
    int cpos = positive_graph_count - mis_pos;
    int cneg = graph_database.size() - positive_graph_count - mis_neg;
    float sen = (float)cpos / (float)positive_graph_count;
    float spe = (float)cneg / (float)(graph_database.size() - positive_graph_count);
    return (sen+spe)/2;    
}
/*output to a file the adjacency matrix of the feature subgraph pattern 
 for a certain positive graph*/
void fout_feature_matrix(int gid, ofstream& fout)
{
    vector<int>& code = graph_features[gid]->code;
    int size = graph_features[gid]->size;
    int** matrix = new int*[size];
    for (int i = 0; i < size; i++)
        matrix[i] = new int[size];
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
void fout_feature_matrix(vector<int>& code, int size, ofstream& fout)
{
    int** matrix = new int*[size];
    for (int i = 0; i < size; i++)
        matrix[i] = new int[size];
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
void fout_feature_matrix(feature* f, ofstream& fout)
{
    vector<int>& code = f->code;
    int size = f->size;
    int** matrix = new int*[size];
    for (int i = 0; i < size; i++)
        matrix[i] = new int[size];
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












/*select feature subgraph patterns to generate classification rules based on the selected patterns;
 normalized accuracy of the classification rules is maximized for the input graph_database*/
void feature_selection(vector<feature*>& features)
{
    vector<bool> has_feature;
    vector<bool> covered;
    int mis_pos = positive_graph_count;
    int mis_neg = 0;
    int delta_mis_pos, delta_mis_neg;
    float nacc = norm_acc(mis_pos, mis_neg);
    for (int i = 0; i < positive_graph_count; i++)
        covered.push_back(false);
    for (int i = 0; i < graph_database.size(); i++)
        has_feature.push_back(false);
    vector<feature*> sorted_features;
    for (int i = 0; i < graph_features.size(); i++)
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

    for (int i = 0; i < graph_database.size(); i++)
        if (i < positive_graph_count && !has_feature[i])
            mis_pos++;
        else if (i >= positive_graph_count && has_feature[i])
            mis_neg++;
    cout<<"mis_pos: "<<mis_pos<<" mis_neg: "<<mis_neg<<endl;

    cout<<"normalized accuracy in training set: "<<nacc<<endl;
}

/*output selected feature subgraph patterns to a file, including the information such as 
 adjacency matrix, positive frequency and negative frequency*/
void fout_features()
{
    string fname = "feature1.txt";
    string fname2 ="feature2.txt";
    ofstream out(fname.c_str());
    ofstream fout(fname2.c_str());
    vector<feature*> features;
    int pid = 0;

    feature_selection(features);

    out<<"rules: "<<features.size()<<endl;
    for (int i = 0; i < features.size(); i++)
    {
        double freq = (double)(features[i]->pgids.size())/(double)positive_graph_count;
        double bg_freq = (double)(features[i]->ngids.size())/(double)(graph_database.size()-positive_graph_count);

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
        double freq = (double)(features[i]->pgids.size())/(double)positive_graph_count;
        double bg_freq = (double)(features[i]->ngids.size())/(double)(graph_database.size()-positive_graph_count);
        if (features[i]->size != 0)
        {
            out<<"id: "<<(pid++)<<endl;
            out<<"adj matrix: "<<features[i]->size<<endl;
            fout_feature_matrix(features[i], out);
        }
        else
            for (int j = 0; j < features[i]->codes.size(); j++)
            {
                out<<"id: "<<(pid++)<<endl;
                int m_size = features[i]->codes[j]->size();
                m_size = sqrt(2*m_size-1);
                out<<"adj matrix: "<<m_size<<endl;
                fout_feature_matrix(*(features[i]->codes[j]), m_size, out);
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
void fout_training_matrix()
{
    string fname = "training.matrix";
    ofstream out(fname.c_str());
    map<vector<int>,pattern *> tmp_idx;
    vector<feature*> features;
    // cout<<graph_database.size()<<"****"<<graph_features.size()<<endl;
    // cout<<graph_features.size()<<endl;

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
                out<<(j+1)<<":1.0 ";
        out<<endl;
    }
}

/*output the size, positive support and negative support information of the 
 resulting subgraph patterns without the adjacency matrices*/
void fout_basic_result()
{
    string fname = "Basic_res.txt";
    ofstream out(fname.c_str());

    map<vector<int>,pattern *> tmp_idx;
    for (int i = 0; i < graph_features.size(); i++)
    {
        if (tmp_idx.find(graph_features[i]->code)!=tmp_idx.end())
            continue;
        else
            tmp_idx[graph_features[i]->code]=0;
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
    bool has_updated=false;
    if(p->score_precise>features_min_score)
    {
        for(auto itr:p->pgids)
        {
            if(graph_features[itr]==NULL)
                graph_features[itr]=new feature(p);
            
            else if(p->score_precise > graph_features[itr]->score_precise)
            {
                has_updated=true;
                delete graph_features[itr];
                graph_features[itr]=new feature(p);
            }
            
            // if(graph_features[itr]==NULL || graph_features[itr]->score_precise<p->score_precise)
            // {
            //     if(graph_features[itr])
            //         delete graph_features[itr];
            //     graph_features[itr]=new feature(p);
            //     features_min_score=min(features_min_score,p->score_precise);                
            // }
        }
        if(has_updated)
        {
            features_min_score=100;
            for(int i=0;i<positive_graph_count;i++)
            {
                if(graph_features[i]==NULL)
                    features_min_score=min(features_min_score,0.0f);
                else
                    features_min_score=min(features_min_score,graph_features[i]->score_precise);
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
        cout<<min_sum_id<<" "<<"insertedhere"<<endl;
        if(!could_insert) 
            delete p;
    }
}
bool has_potential_pre(pattern *p,float max_pattern_score)
{
    // cout<<"has_potenz"<<endl;
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
        // cout<<"Not NULL"<<endl;
        // cout<<"Seleceted Ext"<<endl;

        if(sampled_pattern->ngids.size()==0 || sampled_pattern->size>=max_size)
        {
            // cout<<"Reached =in"<<endl;
            delete sampled_pattern;
            continue;
        }
        // cout<<"After Selected"<<endl;

        float max_pattern_score=get_score(sampled_pattern->pgids.size(),0);
        float best_score_peredge = max_pattern_score*1.0 / (sampled_pattern->edge_size + 1);
        float cur_score_peredge = sampled_pattern->score_precise*1.0/sampled_pattern->edge_size;
        // cout<<"Bef Comparisons"<<endl;
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
        cout<<"We got from candidate List"<<endl;
        cout<<i<<" "<<"Passed manytests"<<endl;
        print_pattern(sampled_pattern);
        vector<pattern *> *new_patterns = sampled_pattern->extend(graph_database);
        cout<<new_patterns->size()<<endl;
        for(auto itr=new_patterns->begin();itr!=new_patterns->end();itr++)
        {
            auto itr2=*itr;
            print_pattern(itr2);
            // cout<<itr2->pgids.size()<<" "<<itr2->ngids.size()<<" "<<itr2->score_precise<<" "<<itr2->score_binned<<endl;
        }
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
    // cout<<"Edge Evlive"<<endl;

    for(auto itr:edge_code)
    {
        update_pattern_score(itr.y);
        if(itr.y->score_precise>0)
            migrate(itr.y);
    }
    // for (int i = 0; i < positive_graph_count; i++)
    // {
    //     for (int j = 0; j < candidate_lists[i]->length;j++)
    //     {
    //         cout << candidate_lists[i]->data[j]->pgids.size() << " " << candidate_lists[i]->data[j]->ngids.size() << " " << candidate_lists[i]->data[j]->score_precise << " " << candidate_lists[i]->data[j]->score_binned<<endl;
    //         // for(auto itr:candidate_lists[i]->data[j]->pgids)
    //         //     cout<<itr<<" ";
    //         // cout<<endl;
    //         // for (auto itr : candidate_lists[i]->data[j]->ngids)
    //         //     cout << itr << " ";
    //         // cout<<endl;
    //     }
    // }
    // exit(0);
    // cout<<"Here"<<endl;

    while(iter_num--)
    {
        cout<<"Evolution:"<<iter_num<<endl;
        // for(int i=0;i<positive_graph_count;i++)
        // {
        //     cout<<i<<" "<<candidate_lists[i]->length<<endl;
        // }
    
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
    // output_feature();

	
    double score_sum = 0.0;
    for (int i = 0; i < positive_graph_count; i++)
    {
        score_sum += graph_features[i]->score_precise;
		/*if a positive graph has no discriminative subgraph pattern found by GAIA,
		 the score of the feature of this positive graph is 0*/
        if (graph_features[i]->score_precise < 0.000001)
            cout<<"graph "<<i<<" has no discriminative feature"<<endl;
    }
    /*output to screen the average score of all the feature subgraph patterns*/
    cout<<"average_score: "<<(score_sum/(double)positive_graph_count)<<endl;
    cout<<"mining is completed successfully"<<endl;
	
        /*output the resulting feature subgraph patterns to file*/
    fout_features();			
    fout_training_matrix();
    
    // return EXIT_SUCCESS;
}

