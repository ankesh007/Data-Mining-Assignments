/* 
 * File:   candidate_list.h
 * Author: njin
 *
 * Created on October 10, 2009, 7:22 PM
 */

#ifndef _CANDIDATE_LIST_H
#define	_CANDIDATE_LIST_H

extern int candidate_size;	//user-specified parameter: size of each candidate list

#include "pattern.h"
#include <vector>
using namespace std;

//candidate list
class candidate_list {
public:
    vector<pattern*> data;	//vector to store the pointers to subgraph patterns
    int score_sum;	//the sum of scores in the candidate list
    int length;		//the number of candidates in the list

    candidate_list();	//constructor
    ~candidate_list();	//detructor
    pattern* select_extension();	/*randomly select a pattern from the candidate list and return it;
									 the probability of a pattern being selected is proportional to its
									 discrimination score*/
    int select_duel();				/*randomly select a pattern in the candidate list and 
									 return the index of this pattern*/
    bool insert(pattern* p);		/*inserts a candidate subgraph pattern into the candidate list, 
									 returns true if the pattern is successfully inserted; 
									 false otherwise*/
    void insert(vector<pattern*>* patterns);	/*insert a vector of patterns into the candidate list 
												 by inserting them individually*/
};

#endif	/* _CANDIDATE_LIST_H */

