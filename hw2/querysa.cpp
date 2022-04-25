//
// Created by Ataberk Donmez on 24.04.2022.
//

#include "querysa.hpp"

querysa::querysa() {}


bool querysa::query(SuffixArray sa_object, std::string query, std::string query_mode, std::string output, std::vector <uint32_t>& occ) {
    uint32_t left = sa_object.get_search_range(query)[0];
    uint32_t right = sa_object.get_search_range(query)[1];
    if (query_mode.compare("naive") == 0) {
        // these are sa indices, need to do sa[first] to get index at the actual string
        int first = naive_search(sa_object, query, left, right, "first");
        if (first == -1) {
            return false;
        }
        int last = naive_search(sa_object, query, left, right, "last");
        if (first != -1 && last != -1) {
            occ.push_back(first);
            occ.push_back(last);
            return true;
       }
    }
}

int querysa::naive_search(SuffixArray sa_object, std::string query, uint32_t left, uint32_t right, std::string order) {
    int center = (right + left) / 2;
    // std::cout << "left, center, right:" << left << " " << center << " " << right << std::endl;
    if (center < 0 || right < left) {
        return -1;
    }
    std::string suffix = sa_object.get_suffix(center);
    // std::cout << query << " " << suffix << std::endl;
    int cmp = query.compare(suffix.substr(0, query.length()));
    if (cmp == 0) {
        if (order.compare("first") == 0) {
            // found a matching suffix but is it the first matching suffix?
            // if it is the first suffix it must be the first matching suffix
            if (center == 0) {
                return center;
            }

            // if the query is not a prefix of the previous suffix than it must be the first suffix
            std::string prev_suffix = sa_object.get_suffix(center - 1);
            int cmp_prev_suffix = query.compare(prev_suffix.substr(0, query.length()));
            if (cmp_prev_suffix != 0) {
                return center;
            } else {
                // otherwise, keep looking in the left part
                return naive_search(sa_object, query, left, center - 1, order);
            }
        }
        else {
            // similar to first but this time we are looking for the last one
            if (center == sa_object.sa.size() - 1) {
                return center;
            }

            // if the query is not a prefix of the next suffix than it must be the last suffix
            std::string prev_suffix = sa_object.get_suffix(center + 1);
            int cmp_prev_suffix = query.compare(prev_suffix.substr(0, query.length()));
            if (cmp_prev_suffix != 0) {
                return center;
            } else {
                // otherwise, keep looking in the right part
                return naive_search(sa_object, query, center + 1, right, order);
            }
        }
    }
    else if (cmp < 0) {
        // keep looking in the left part
        return naive_search(sa_object, query, left, center - 1, order);
    }
    else {
        // keep looking in the right part
        return naive_search(sa_object, query, center + 1, right, order);
    }
}
