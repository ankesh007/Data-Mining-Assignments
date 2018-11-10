/* 
 * File:   candidate_list.cpp
 * Author: njin
 * 
 * Created on October 10, 2009, 7:22 PM
 */

#include "candidate_list.h"

//constructor
//initializes the size of the candidate list as 0
//initializes the sum of scores as 0
candidate_list::candidate_list()
{
    score_sum = 0;
    this->length = 0;
}

//destructor
//deletes all the candidates in this list
candidate_list::~candidate_list()
{
    for (int i = 0; i < data.size(); i++)
    {
        delete data[i];
    }
}

//inserts a candidate subgraph pattern into the candidate list
//returns true if the pattern is successfully inserted; false otherwise
bool candidate_list::insert(pattern* p)
{
	/*if the candidate list is still empty, sets the size as specified by users
	 and initialize all the candidates as NULL in this list*/
    if (data.size() == 0)
    {
        data.resize(candidate_size, NULL);
    }
	/*if the candidate list is not full yet, insert the pattern to the end of
	 the list; update the number of patterns in the list and update the sum of
	 scores in the list*/
    if (length < data.size())
    {
        data[length] = p;
        length++;
        this->score_sum += p->score_binned;
        return true;
    }
	/*if the candidate list is full, then randomly select one pattern from the 
	 list and compare the scores of the two patterns, the pattern with the larger
	 score survives and is maintained in the list and the other pattern is discarded;
	 update the sum of scores in the list if needed*/
    else
    {
        int i = this->select_duel();
        if (duel(data[i]->score_precise, p->score_precise))
            return false;
        else
        {
            score_sum -= data[i]->score_binned;
            delete data[i];
            data[i] = p;
            score_sum += data[i]->score_binned;
            return true;
        }

    }
}

/*insert a vector of patterns into the candidate list by inserting them
 individually in order*/ 
void candidate_list::insert(vector<pattern*>* patterns)
{
    for (int i = 0; i < patterns->size(); i++)
        insert(patterns->at(i));
}

//randomly select a pattern in the candidate list and return the index of this pattern
int candidate_list::select_duel()
{
    return rand() % length;
}

/*randomly select a pattern from the candidate list and return it;
 the probability of a pattern being selected is proportional to its
 discrimination score*/
/*the selected pattern is removed from the candidate list, the number
 of patterns and the sum of scores of this candidate list is updated*/
pattern* candidate_list::select_extension()
{
	//if the candidate list is empty, return NULL
    if (length <= 0)
        return NULL;
    int i = length - 1;
    int n;
    if (score_sum != 0)
        n = rand() % score_sum;
    else
        return NULL;
    int tmp_sum = data[i]->score_binned;

    while (tmp_sum < n)
    {
        i--;
        if (i < 0)
        {
            i = 0;
            break;
        }
        tmp_sum += data[i]->score_binned;
    }

	//update the sum of scores and the number of patterns in the candidate list
    score_sum -= data[i]->score_binned;
    pattern* res = data[i];
    pattern* tmp = data[this->length-1];
    data[i] = tmp;
    length--;

    return res;
}
