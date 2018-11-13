#include "candidate_list.h"
#include "declaration.h"

candidate_list::candidate_list()
{
    score_sum = 0;
    this->length = 0;
}
candidate_list::~candidate_list()
{
    for (int i = 0; i < data.size(); i++)
        delete data[i];
}

bool candidate_list::insert(pattern* p)
{
    if (data.size() == 0)
        data.resize(candidate_size, NULL);

    if (length < data.size())
    {
        data[length] = p;
        length++;
        this->score_sum += p->score_binned;
        return true;
    }
    else
    {
        int i = rand()%length;
        if (data[i]->score_precise > p->score_precise)
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

// void candidate_list::insert(vector<pattern*>* patterns)
// {
//     for (int i = 0; i < patterns->size(); i++)
//         insert(patterns->at(i));
// }

/*randomly select a pattern from the candidate list and return it;
 the probability of a pattern being selected is proportional to its
 discrimination score*/
/*the selected pattern is removed from the candidate list, the number
 of patterns and the sum of scores of this candidate list is updated*/
pattern* candidate_list::select_extension()
{
	//if the candidate list is empty, return NULL
    if (length <= 0 || score_sum==0)
        return NULL;
    int i = length - 1;
    int n;

    n = rand() % score_sum;
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

    score_sum -= data[i]->score_binned;
    // pattern* res = data[i];
    // pattern* tmp = data[this->length-1];
    // data[i] = tmp;
    swap(data[i],data[this->length-1]);
    length--;
    
    return data[this->length];
}
