#include "feature.h"

//constructor; uses one pattern as a feature
feature::feature(pattern* p)
{
    this->code = p->code;
    this->pgids = p->pgids;
    this->ngids = p->ngids;
    this->score_binned = p->score_binned;
    this->score_precise = p->score_precise;
    this->size = p->size;
}
