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

pattern* candidate_list::select_extension()
{
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
    swap(data[i],data[this->length-1]);
    length--;
    
    return data[this->length];
}
