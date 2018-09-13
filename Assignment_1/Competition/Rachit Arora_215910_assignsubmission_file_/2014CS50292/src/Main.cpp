#include <iostream>
#include "AprioriHandler.h"
#include "FPHandler.h"
using namespace std;
int main(int argc, char* argv[])
{
    if (argc != 5)
    {
        cerr<<"[ERROR] "<<argv[0]<<" : Invalid number of arguments!\n";
        cerr<<"Input Usage : "<<argv[0]<<" input_file output_file support mode\n";
        cerr<<" mode : 0 -> apriori, 1 -> fpgrowth , 2 -> sorter\n";
        exit(0);
    }
    string input(argv[1]), output(argv[2]), ssupport(argv[3]), smode(argv[4]);
    double min_support = stod(ssupport)/100.0;
    int mode = stoi(smode);
    switch(mode)
    {
        case 0:
        {
            AprioriHandler* apriori_h = new AprioriHandler(input, output, min_support);
            apriori_h->generateFrequentItemSet();
            delete apriori_h;
            break;
        }
        case 1:
        {
            FPHandler* fp_h = new FPHandler(input, output, min_support);
            fp_h->generateFrequentItemSet();
            delete fp_h;
            break;
        }
        case 2:
        {
            break;
        }
        default:
        {
            cerr<<"Invalid mode\n";
            exit(0);  
        }
    }

    return 0;
}
