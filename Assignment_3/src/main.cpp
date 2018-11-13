#include "EVO.h"
#include <bits/stdc++.h>
using namespace std;

//global variables
int pos_num;				//number of positive graphs
int neg_num;				//number of negative graphs
int candidate_size;			//size of each candidate list
int iter_num;				//number of iterations
int max_size;				//maximum size of subgraph patterns
double pfreq_threshold;		//positive frequency threshold (optional)
double bg_freq_threshold;	//negative frequency threshold (optional)
string input_path_str;		//path of input files
string output_path_str;		//path of output files
string NF_SUFFIX;               //node file name
string EF_SUFFIX;               //edge file name
int test_samples=0;

//converts a string to an integer; returns the result
int stoi(string s)
{
	stringstream ss(s);
	int res;
	ss>>res;
	return res;
}

//converts a string to a double; returns the result
double stod(string s)
{
	stringstream ss(s);
	double res;
	ss>>res;
	return res;
}

//converts an integer to a string; returns the result
string itos(int i)
{
	stringstream ss;
	ss<<i;
	return ss.str();
}

void outputArguments()
{   
    cout<<"node_file_name = "<<NF_SUFFIX<<endl;
    cout<<"edge_file_name = "<<EF_SUFFIX<<endl;
    cout<<"number_of_positive_graphs = "<<pos_num<<endl;
    cout<<"candidate_list_size = "<<candidate_size<<endl;
    cout<<"number_of_iterations = "<<iter_num<<endl;
}

bool readConfig()
{
    input_path_str = "";
    output_path_str = "";
    NF_SUFFIX = "node_file.txt";
    EF_SUFFIX = "edge_file.txt";
    candidate_size = 100;
    iter_num = 10;
    pos_num = 50;

    max_size = 1000;       //hidden from casual users
    pfreq_threshold = 0.0; //hidden from casual users

    ifstream fin("GAIA_config");
    if (!fin.is_open())
    {
        fin.close();
        return false;
    }
    string tmp;
    while (fin >> tmp)
    {
        if (tmp == "node_file_name")
            fin >> tmp >> NF_SUFFIX;
        else if (tmp == "edge_file_name")
            fin >> tmp >> EF_SUFFIX;
        else if (tmp == "number_of_positive_graphs")
            fin >> tmp >> pos_num;
        else if (tmp == "candidate_list_size")
            fin >> tmp >> candidate_size;
        else if (tmp == "number_of_iterations")
            fin >> tmp >> iter_num;
        else
        {
            cout << "error: unknown argument name: " << tmp << endl;
            cout << "error: fail to read GAIA_config" << endl;
            fin.close();
            return false;
        }
    }

    fin.close();
    return true;
}

//main function
/*required arguments:
 input directory; output directory; prefix of the input graph files;
 size of each candidate list; number of iterations; number of positive graphs in the input files*/
/*optional arguments:
 maximal number of nodes allowed in a subgraph pattern; minimal positive frequency*/
int main(int argc, char** argv)
{
    
    else if (argc > 1)
    {
        for (int i = 1; i < argc; i++)
        {
            string arg = string(argv[i]);
            if (arg == "-h")
            {
                cout<<"Below is an example of configuration file GAIA_config. ";
                cout<<"All arguments can be set in file GAIA_config ";
                cout<<"and the file should be in the same directory as this binary. ";
                cout<<"If GAIA_config is absent or cannot be parsed successfully, ";
                cout<<"default values (described as below) will be used."<<endl;
            
                outputArguments();
                cout<<endl;
                
                cout<<"Or you can set arguments in command line with the following options:"<<endl;
                cout<<"-v: node_file_name"<<endl;
                cout<<"-e: edge_file_name"<<endl;
                cout<<"-p: number_of_positive_graphs"<<endl;
                cout<<"-n: number_of_iterations"<<endl;
                cout<<"-s: candidate_list_size"<<endl;
                cout<<"-s: test_samples at the end"<<endl;
                return EXIT_FAILURE;
            }
            else if (arg == "-v")
            {
                i++;
                if (i >= argc)
                {
                    cout<<"error: missing value after option "<<arg<<endl;
                    return EXIT_FAILURE;
                }
                NF_SUFFIX = string(argv[i]);
            }
            else if (arg == "-e")
            {
                i++;
                if (i >= argc)
                {
                    cout<<"error: missing value after option "<<arg<<endl;
                    return EXIT_FAILURE;
                }
                EF_SUFFIX = string(argv[i]);
            }
            else if (arg == "-p")
            {
                i++;
                if (i >= argc)
                {
                    cout<<"error: missing value after option "<<arg<<endl;
                    return EXIT_FAILURE;
                }
                pos_num = stoi(string(argv[i]));
            }
            else if (arg == "-n")
            {
                i++;
                if (i >= argc)
                {
                    cout<<"error: missing value after option "<<arg<<endl;
                    return EXIT_FAILURE;
                }
                iter_num = stoi(string(argv[i]));
            }
            else if (arg == "-s")
            {
                i++;
                if (i >= argc)
                {
                    cout<<"error: missing value after option "<<arg<<endl;
                    return EXIT_FAILURE;
                }
                candidate_size = stoi(string(argv[i]));
            }
            else if (arg == "-t")
            {
                i++;
                if (i >= argc)
                {
                    cout<<"error: missing value after option "<<arg<<endl;
                    return EXIT_FAILURE;
                }
                test_samples = stoi(string(argv[i]));
            }
            else
            {
                cout<<"error: unknown option "<<arg<<endl;
                cout<<"please use -h for help"<<endl;
                return EXIT_FAILURE;
            }
        }
    }
    
    if (!is_successful)
    {
        cout<<"error: fail to open GAIA_config"<<endl;
        cout<<"running with default argument setting:"<<endl;
    }
    else
        cout<<"running with the following argument setting"<<endl;
    outputArguments();
    
    
    int seed = time(NULL);	//seed for srand
    srand(seed);
    
    bool checkFile = checkFileCanOpen(NF_SUFFIX) && checkFileCanOpen(EF_SUFFIX);
    if (!checkFile)
        return EXIT_FAILURE;
    EVO test("");	//discriminative subgraph mining
	
    double score_sum = 0.0;
    for (int i = 0; i < pos_num; i++)
    {
        score_sum += test.graph_features[i]->score_precise;
		/*if a positive graph has no discriminative subgraph pattern found by GAIA,
		 the score of the feature of this positive graph is 0*/
        if (test.graph_features[i]->score_precise < 0.000001)
            cout<<"graph "<<i<<" has no discriminative feature"<<endl;
    }
    /*output to screen the average score of all the feature subgraph patterns*/
    cout<<"average_score: "<<(score_sum/(double)pos_num)<<endl;
    cout<<"mining is completed successfully"<<endl;
	
        /*output the resulting feature subgraph patterns to file*/
    test.fout_features();			
    test.fout_training_matrix();
    
    return EXIT_SUCCESS;
}

