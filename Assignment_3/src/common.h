/* 
 * File:   common.h
 * Author: njin
 *
 * Created on October 10, 2009, 7:15 PM
 */

#ifndef _COMMON_H
#define	_COMMON_H

#include <math.h>
#include <limits.h>
#include <iostream>
#include <time.h>
#include <vector>
#include <stdlib.h>
using namespace std;

#define BIAS_DISTRIBUTION	/*if not defined, then a new pattern is randomly assigned to an 
							empty candidate list;
							if defined, the a new pattern is assigned to the candidate list
							with the lowest sum of scores, i.e. the candidate list with the
							least competition*/

#define TOTAL_SCORE	//calculate discrimination score solely based on frequencies
//#define UNIT_SCORE	//takes into account size of patterns when calculating discrimination scores

#define DEAD_NODE	/*if a node in a subgraph pattern has no neighboring node for extension,
					then this node will be ignored in future extension*/

#define PRE_EXTENSION_PRUNING	/*before a pattern p is extended, check if its upper-bound
								can update any feature pattern; if its upper-bound is no greater than
								any feature pattern, then the pattern p is discarded from further
								consideration*/

#define POST_EXTENSION_PRUNING	/*after a new pattern p is generated, check if its upper-bound
								can update any feature pattern; if its upper-bound is no greater than
								any feature pattern, then the pattern p is discarded from further
								consideration*/

#define LOG_RATIO	//uses log ratio of positive frequency to negative frequency as discrimination score
//#define GTEST		//uses g-test as discrimination score
//#define RATIO		//uses ratio of positive frequency to negative frequency as discrimination score


/*using momentum can increase average score,
 * but it will also slow down the algorithm*/
#define MOMENTUM	//uses momentum when deciding whether a pattern should be considered for further extension
#define INITIAL_M 1	//initial momentum
#define M_GAIN 1    //this must be a positive number; momentum will gain so much if the unit score doesn't decrease
#define M_LOSS 1    //this must be a positive number; momentum will lose so much if the unit score decreases

extern int pos_num;	//number of positive graphs in the input
extern int neg_num;	//number of negative graphs in the input

#define NULLCODE "NULL"
#define BF_SUFFIX "basicResult_file.txt"	//file name suffix of basic output files
#define PF_SUFFIX "patternResult.txt"	//file name suffix of output pattern files
#define FF_SUFFIX "featureResult.txt"	//file name suffix of output feature files
#define TF_SUFFIX "svmResult.txt"		//file name suffix of output svm files

/*encoding and decoding used in subgraph pattern extension*/
#define SOURCE_SHIFT 19
#define TYPE_SHIFT 31
#define EDGE_SHIFT 12
#define SOURCE_MASK 0x00000fff
#define DRAIN_MASK 0x00000fff
#define EDGE_MASK 0x0000007f
#define TYPE_MASK 0x00000001
#define CODE_TYPE int
//highest bit: whether this is an internal extension; if 1, then lowest 12 bits represent node id; else, node type
//high 12 bits: source node id
//next 7 bits: edge label
//low 12 bits: drain node id
#define EXTCODE(u, x, y, z) ( ( ( (CODE_TYPE)(u) ) << TYPE_SHIFT ) | ( ( (CODE_TYPE)(x) ) << SOURCE_SHIFT ) |  ((CODE_TYPE)(y) << EDGE_SHIFT ) | ((CODE_TYPE)(z)) )
#define EXTSOURCE(x)   ( (CODE_TYPE) ( (x)  >> SOURCE_SHIFT ) & SOURCE_MASK )
#define EXTEDGE(x)  ( (CODE_TYPE) ( ((x)  ) >> EDGE_SHIFT ) & EDGE_MASK )
#define EXTDRAIN(x)   ( ((CODE_TYPE) ((x)  ) ) & DRAIN_MASK )
#define EXTTYPE(x) ((CODE_TYPE) ( (x)  >> TYPE_SHIFT ) & TYPE_MASK)

/*returns the discrimination score calculated based on positive support, negative support
 and the size of the subgraph pattern*/
inline float calc_score(int psup, int nsup, float size)
{
    float pfreq, nfreq;
    if (psup <= 0)
        return -1.0;
    pfreq = (float)psup / (float)pos_num;
    nfreq = (float)(nsup+1) / (float)(neg_num+1);
    #ifdef UNIT_SCORE
    #ifdef LOG_RATIO
    return log(pfreq/nfreq) / size;
    #endif
    #ifdef LOG_ODDS_RATIO
    return log(pfreq/(1-pfreq)/nfreq*(1-nfreq)) / size;
    #endif
    #ifdef RATIO
    return pfreq/nfreq/size;
    #endif
    #endif
#ifdef TOTAL_SCORE
    #ifdef LOG_RATIO
    return log(pfreq/nfreq);
    #endif
    #ifdef LOG_ODDS_RATIO
    return log(pfreq/(1-pfreq)/nfreq*(1-nfreq));
    #endif
    #ifdef RATIO
    return pfreq/nfreq;
    #endif
    #ifdef GTEST
    return pfreq*log(pfreq/nfreq)+(1-pfreq)*log((1-pfreq)/(1-nfreq));
    #endif
#endif
}

/*calculate the intersection vector of two vectors*/
inline vector<short>* intersect_vshort(vector<short>& a, vector<short>& b)
{
    vector<short>* c = new vector<short>;
    vector<short>::iterator ait, bit;
    ait = a.begin();
    bit = b.begin();
    while (ait != a.end() && bit != b.end())
    {
        if (*ait < *bit)
            ait++;
        else if (*ait > *bit)
            bit++;
        else
        {
            c->push_back(*ait);
            ait++;
            bit++;
        }
    }

    return c;
}

/*function to determine the result of a duel between two scores*/
//if p1 wins, returns true; else, returns false
inline bool duel(float p1, float p2)
{
    return p1 > p2;
}

#endif	/* _COMMON_H */

