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

// /*calculate the normalized accuracy based on numbers of false positives and false negatives*/
float EVO::norm_acc(int mis_pos, int mis_neg)
{
    int cpos = positive_graph_count - mis_pos;
    int cneg = graphs.size() - positive_graph_count - mis_neg;
    float sen = (float)cpos / (float)positive_graph_count;
    float spe = (float)cneg / (float)(graphs.size() - positive_graph_count);
    return (sen+spe)/2;    
}

/*select feature subgraph patterns to generate classification rules based on the selected patterns;
 normalized accuracy of the classification rules is maximized for the input graphs*/
void EVO::feature_selection(vector<feature*>& features)
{
    vector<bool> has_feature;
    vector<bool> covered;
    int mis_pos = positive_graph_count;
    int mis_neg = 0;
    int delta_mis_pos, delta_mis_neg;
    float nacc = norm_acc(mis_pos, mis_neg);
    for (int i = 0; i < positive_graph_count; i++)
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
        if (i < positive_graph_count && !has_feature[i])
            mis_pos++;
        else if (i >= positive_graph_count && has_feature[i])
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
    int pid = 0;

    feature_selection(features);

    out<<"rules: "<<features.size()<<endl;
    for (int i = 0; i < features.size(); i++)
    {
        double freq = (double)(features[i]->pgids.size())/(double)positive_graph_count;
        double bg_freq = (double)(features[i]->ngids.size())/(double)(graphs.size()-positive_graph_count);

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
        double bg_freq = (double)(features[i]->ngids.size())/(double)(graphs.size()-positive_graph_count);
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
                int m_size = features[i]->codes[j]->size();
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
void EVO::fout_feature_matrix(int gid, ofstream& fout)
{
    vector<int>& code = this->graph_features[gid]->code;
    int size = this->graph_features[gid]->size;
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
void EVO::fout_feature_matrix(vector<int>& code, int size, ofstream& fout)
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
void EVO::fout_feature_matrix(feature* f, ofstream& fout)
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



