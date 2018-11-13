/* 
 * File:   feature.h
 * Author: njin
 *
 * Created on October 10, 2009, 7:37 PM
 */

#ifndef _FEATURE_H
#define	_FEATURE_H

#include "pattern.h"

//class: feature 
/*this data structure, "feature", is used to store important information of feature patterns,
 including code, score, size, positive gids and negative gids*/
class feature {
public:
    vector<int> code; //code of a feature if it has only one pattern
    vector<vector<int>*> codes;   /*if a feature has more than one pattern,
                                     its code is a vector of codes of its member patterns;
									 currently not used*/
    int size;				//number of labels
    vector<int> pgids;    //IDs of the supporting positive graphs
    vector<int> ngids;    //IDs of the supporting negative graphs
    float score_binned;     //discretized score
    float score_precise;    //accurate score

    feature(pattern* p);    //constructor; uses one pattern as a feature
    //constructor without parameters
	feature(){
        size = 0;
        score_binned = 0;
        score_precise = 0.0;
    };
};

#endif	/* _FEATURE_H */

