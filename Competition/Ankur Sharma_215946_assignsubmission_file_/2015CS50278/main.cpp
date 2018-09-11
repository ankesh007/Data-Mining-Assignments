using namespace std;
#include <iostream>
#include <string>

void apriori(const string infilename, const string outfilename, double threshold);

int main(int argc, char* argv[]) {
    //sample test for apriori, edit paths as required, last argument is support threshold

    double threshold = atoi(argv[1]) * 1.0 / 100;
    string infile = argv[2];
    string outfilename = argv[3];
    apriori(infile, outfilename + ".txt", threshold);

    return 0;
}