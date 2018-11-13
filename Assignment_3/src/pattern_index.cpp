/* 
 * File:   pattern_index.cpp
 * Author: njin
 * 
 * Created on October 10, 2009, 7:04 PM
 */

#include "pattern_index.h"

/*insert a code "s" into "data"*/
void pattern_index::insert(vector<short>& s)
{
    this->data.insert(s);
}

/*find out if code "s" is already in "data"*/
bool pattern_index::find(vector<short>& s)
{
    return this->data.find(s) != this->data.end();
}

