#include "fp_tree.h"

inline int fast_atoi( const char * st )
{    int val = 0;
    while( *st )
        val = val*10 + (*st++ - '0');
    return val;
}
int trans_freq_count(string& filename){
    const char* c = filename.c_str();
    int trans_count=0;
    FILE *fp;
    char  buffer[16],str[1048576];
    int i,it = 0;
    fp = fopen(c, "r");
    while(fgets(str,1048576,fp) != nullptr){
        i=0;
        trans_count++;
        while(str[i]!='\n'){
            while(isdigit(str[i])){
                buffer[it++]=str[i++];
            }
            freq_map[fast_atoi(buffer)]++;
            i++;
            memset(buffer,0,16);
            it=0;
        }
    }
    fclose(fp);
    return trans_count;
}

