#include<bits/stdc++.h>
using namespace std;
#define MAX_LINE_LENGTH 1048576

FILE *infile, *outfile;
int n, support;
unordered_map<int,int> first_cand;
unordered_map<int,int>::iterator it;
vector<vector<int>> f,can;
vector<int> v,freq,t;
static char itemset[MAX_LINE_LENGTH];
static char outline[MAX_LINE_LENGTH];

bool mark;
inline bool getnextint(char *fline, int &x, int &in) {
	in = 0;
	mark = false;
	while (fline[x]) {
		if (fline[x] >= '0' && fline[x] <= '9') {
			in *= 10; in += fline[x]-'0';
			mark = true;
		}
		else {
			if (mark) return true;
		}
		x++;
	}
	return false;
}


inline void putnextint(char *pline, int &x, int in) {
	char ti[10];
	char *p = ti;
	do {
		*p++ = char(in % 10) + '0'; in /= 10;
	} while (in > 0);
	do {
		pline[x++] = *--p;
	} while (p != ti);
	pline[x++] = ' ';
}


bool check_subset(vector<int> cand, vector<vector<int>> & f){
    vector<int> sub(cand.size()-1);
    int skip=0,i;
    //Pruning
    //all the subset of a frequent itemset should be frequent
    while(skip<cand.size()){
    	int k = 0;
        for(i=0;i<cand.size();++i)
            if(i!=skip) sub[k++] = cand[i];
        for(i=0;i<f.size();++i){
            bool b=0;
            for(int j=0;j<f[i].size();++j){
                if(f[i][j]!=sub[j]) {b=1;break;}
            }
            if(b==0)   break;
        }
        if(i==f.size()) return 0;
        skip++;
    }
    return 1;
}

vector<vector<int>> cand_gen(vector<vector<int>> & f){
    vector<vector<int>> can;
    int no_it = f.size(),k;
    vector<int> f1,f2,cand(f[0].size()+1);
    for(int i=0;i<no_it;++i){
        f1=f[i];
        for(int j=0;j<no_it;++j){
            if(j==i)    continue;
            f2=f[j];
            for(k=0;k<f1.size()-1;++k)  if(f1[k]!=f2[k])    break;
            if(k!=f1.size()-1)  continue;
            if(f1.back() >= f2.back())  continue;

            for(k=0;k<f1.size();++k)    cand[k] = f1[k];
            cand[k] = f2.back();
//            for(int k=0;k<cand.size();++k)    printf("%d ",cand[k]);
//            cout<<endl;
            if(check_subset(cand,f))    can.push_back(cand);
        }
    }
//    printf("Candidates:-\n");
//    for(int i=0;i<can.size();++i){
//        for(int j=0;j<can[i].size();++j)
//            printf("%d ",can[i][j]);
//        printf("\n");
//    }
//    printf("Finish\n");
    return can;

}

int main(int argc, char* argv[]) {
	if (argc < 4) {
		printf("Usage: ./a.out <X> <inputfile> <outputfile>\n");
		return 0;
	}
	clock_t beg,en;
	beg = clock();
	double X;
	sscanf(argv[1], "%lf", &X);
	infile = fopen(argv[2],"r");
	outfile = fopen(argv[3],"w");

//	int total=0;
	n = 0;
	int x,item;
	while (fgets(itemset, MAX_LINE_LENGTH, infile) != NULL) {
        x=0;item=0;
		while (getnextint(itemset, x, item)) {
//			 v.push_back(item); //to check_subset
			first_cand[item]++;
//			 total++;
		}
//		f.push_back(v); //to check_subset
//		v.clear(); //to check_subset
		n++;
	}
	support = (int)ceil(n * X / 100);
	printf("Total: %d\n",n);
	printf("Threshold: %d\n",support);

//    Cand1 to Freq1
	for(it=first_cand.begin();it!=first_cand.end();++it){ //f1
        if(it->second>=support){
            fprintf(outfile,"%d\n",it->first);
            v.push_back(it->first);
            f.push_back(v);
            v.pop_back();
        }
	}

//	for(int i=0;i<can.size();++i){
//        for(int j=0;j<can[i].size();++j)
//            printf("%d ",can[i][j]);
//        printf("\n");
//	}
	for(int k=2;!f.empty();++k){ //
        can=cand_gen(f);
        freq.clear();
        freq.resize(can.size(),0);//resizes and puts default value as 0
        f.clear(); // removes everything from vector of vectors
        //To reread a file, rewind(infile);
        rewind(infile);
        while(fgets(itemset,MAX_LINE_LENGTH,infile)!=NULL){ // reading transactions from the file
            t.clear();// 1 transaction fetched in t.
            x=0;item=0;
            while(getnextint(itemset, x, item)){ //sscanf return no. of items scanned
                t.push_back(item);
            }
            sort(t.begin(),t.end());
            //print transaction
//            printf("Transaction to be matched:\n");
//            for(int i=0;i<t.size();++i) printf("%d ",t[i]);
//            cout<<endl;
            //match each transaction with all the candidates generated to increase count if matched
            for(int i=0;i<can.size();++i){
                int j,z;
                for(j=0,z=0;j<t.size()&& z<can[i].size();++j)
                    if(can[i][z]==t[j]) z++;

                if(z==can[i].size())    freq[i]++;//candidate present in transaction
            }
        }
//        printf("Freq count of candidates:\n");
//        for(int i=0;i<freq.size();++i)  printf("%d ",freq[i]);
//        cout<<endl;
        for(int i=0;i<can.size();++i)
            if(freq[i]>=support)    f.push_back(can[i]);


        for(int i=0;i<f.size();++i){
			int x = 0;
            for(int j=0;j<f[i].size();++j){
//                printf("%d ",f[i][j]);
            	putnextint(outline, x, f[i][j]);
            }
            outline[x++] = '\n';
			outline[x++] = '\0';
			fputs(outline, outfile);
            // printf("\n");
        }
    }

//	 printf("Total items:- %d\n",total);
//	printf("Unique items:- %d\n",cand.size());
    printf("Done finding Frequent itemsets.\n");
	fclose(outfile);
	fclose(infile);
	en = clock();
	double time_secs = double(en-beg)/CLOCKS_PER_SEC;
	printf("Time taken: %lf secs\n",time_secs);
	return 0;
}

