#ifndef _CANDIDATE_LIST_H
#define	_CANDIDATE_LIST_H

#include <vector>
using namespace std;
// #include "pattern.h"

class pattern;/* Forward Declaration */

class candidate_list {
public:
    vector<pattern*> data;	
    int score_sum;	
    int length;		

    candidate_list();	
    ~candidate_list();	
    pattern* select_extension();	/*randomly select a pattern from the candidate list and return it;
									 the probability of a pattern being selected is proportional to its
									 discrimination score*/
    bool insert(pattern* p);		/*inserts a candidate subgraph pattern into the candidate list, 
									 returns true if the pattern is successfully inserted; 
									 false otherwise*/
	
};

#endif

