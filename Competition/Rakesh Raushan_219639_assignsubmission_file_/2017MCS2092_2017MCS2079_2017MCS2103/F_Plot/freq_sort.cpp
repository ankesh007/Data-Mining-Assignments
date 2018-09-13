#include "fp_tree.h"

vector<pair<int, int>> freq_pair;           //Stores all frequency count from freq_map in form of vector pair
void freq_sort(int thresh) {
    for(auto v:freq_map)
        freq_pair.emplace_back(v.second,v.first);

    auto it = copy_if(freq_pair.begin(), freq_pair.end(), back_inserter(freq_sorted),
                      [&thresh](const pair<int, int> &item_pair) { return (item_pair.first >= thresh); });
    sort(freq_sorted.begin(), freq_sorted.end());

    for (int i = freq_sorted.size() - 1,j=0; i >= 0; --i,j++){
        item_corr_order.push_back(freq_sorted[i].second);
        item_map[item_corr_order[j]]=j;
    }
}
