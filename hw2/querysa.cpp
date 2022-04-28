//
// Created by Ataberk Donmez on 24.04.2022.
//

#include <ctime>
#include "querysa.hpp"


querysa::querysa() {}

/*
 * Query entry point. Based on the query_mode delegates to either naive_search() or simple_accel()
 * returns true if the query exists in the sa_object.genome. The start and end indices of the all occurrences in
 * the sa_object.sa is put into the occ out parameter.
 * */
bool querysa::query(SuffixArray& sa_object, std::string query, std::string query_mode, std::vector <uint32_t>& occ) {
    // if there is no pt, the following values are 0 and sa_object.sa.size() respectively
    uint32_t left = sa_object.get_search_range(query)[0];
    uint32_t right = sa_object.get_search_range(query)[1];
    int first, last;
    if (query_mode.compare("naive") == 0) {
        // these are sa indices, need to do sa[first] to get index at the actual string
        first = naive_search(sa_object, query, left, right, "first");
        if (first == -1) {
            // shortcut, no need to look for the last one if it doesn't exist!
            occ.push_back(0);
            occ.push_back(0);
            return false;
        }
        last = naive_search(sa_object, query, left, right, "last");
    } else {
        first = simple_accel(sa_object, query, left, right, 0, 0, "first");
        if (first == -1) {
            occ.push_back(0);
            occ.push_back(0);
            return false;
        }
        last = simple_accel(sa_object, query, left, right, 0, 0, "last");
    }
    if (first != -1 && last != -1) {
        occ.push_back(first);
        occ.push_back(last);
        return true;
    }
}


/*
 * Naive, modified binary search. Depending on the order parameter finds either the first or last occurrence of query
 * */
int querysa::naive_search(SuffixArray& sa_object, std::string query, uint32_t left, uint32_t right, std::string order) {
    int center = (right + left) / 2;
    if (right < left) {
        return -1;
    }
    uint32_t suffix_start = sa_object.sa[center];
    // starting from the beginning of the both strings, compare query.length() chars
    int cmp = query.compare(0, query.length(), sa_object.genome, suffix_start, query.length());
    if (cmp == 0) {
        if (order.compare("first") == 0) {
            // found a matching suffix but is it the first matching suffix?
            // if it is the first suffix it must be the first matching suffix
            if (center == 0) {
                return center;
            }

            // if the query is not a prefix of the previous suffix than it must be the first suffix
            uint32_t prev_start = sa_object.sa[center - 1];
            int cmp_prev_suffix = query.compare(0, query.length(), sa_object.genome, prev_start, query.length());
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
            uint32_t next_start = sa_object.sa[center + 1];
            int cmp_next_suffix = query.compare(0, query.length(), sa_object.genome, next_start, query.length());
            if (cmp_next_suffix != 0) {
                return center;
            } else {
                // otherwise, keep looking in the right part
                return naive_search(sa_object, query, center + 1, right, order);
            }
        }
    } else if (cmp < 0) {
        // keep looking in the left part
        return naive_search(sa_object, query, left, center - 1, order);
    } else {
        // keep looking in the right part
        return naive_search(sa_object, query, center + 1, right, order);
    }
}


/*
 * Improved version of the naive algorithm. Skips the first n common characters of left and right for comparison.
 * Sometimes slower than the naive due to the extra function call for computing longest common prefix.
 * */
int querysa::simple_accel(SuffixArray& sa_object, std::string query, uint32_t left, uint32_t right, uint32_t left_lcp,
                          uint32_t right_lcp, std::string order) {
    int center = (right + left) / 2;
    if (right < left) {
        return -1;
    }
    uint32_t suffix_start = sa_object.sa[center];
    uint32_t min_lcp = (left_lcp < right_lcp) ? left_lcp : right_lcp;

    // Would be nice to have a function that both compares and returns the length of the lcp simultaneously
    // so I wouldn't have to make the following two calls separately
    int cmp = query.compare(min_lcp, query.length() - min_lcp, sa_object.genome, suffix_start + min_lcp,
                            query.length() - min_lcp);
    uint32_t curr_lcp = lcp(query, sa_object.genome, suffix_start);
    if (cmp == 0) {
        if (order.compare("first") == 0) {
            // found a matching suffix but is it the first matching suffix?
            // if it is the first suffix it must be the first matching suffix
            if (center == 0) {
                return center;
            }

            // if the query is not a prefix of the previous suffix than it must be the first suffix
            uint32_t prev_start = sa_object.sa[center - 1];
            int cmp_prev_suffix = query.compare(min_lcp, query.length() - min_lcp, sa_object.genome,
                                                prev_start + min_lcp,query.length() - min_lcp);
            if (cmp_prev_suffix != 0) {
                return center;
            } else {
                // otherwise, keep looking in the left part
                return simple_accel(sa_object, query, left, center - 1, left_lcp, curr_lcp, order);
            }
        }
        else {
            // similar to first but this time we are looking for the last one
            if (center == sa_object.sa.size() - 1) {
                return center;
            }

            // if the query is not a prefix of the next suffix than it must be the last suffix
            uint32_t next_start = sa_object.sa[center + 1];
            int cmp_next_suffix = query.compare(min_lcp, query.length() - min_lcp, sa_object.genome,
                                                next_start + min_lcp,query.length() - min_lcp);
            if (cmp_next_suffix != 0) {
                return center;
            } else {
                // otherwise, keep looking in the right part
                return simple_accel(sa_object, query, center + 1, right, curr_lcp, right_lcp, order);
            }
        }
    } else if (cmp < 0) {
        // keep looking in the left part
        // curr lcp is going to be the right lcp
        return simple_accel(sa_object, query, left, center - 1, left_lcp, curr_lcp, order);
    } else {
        // keep looking in the right part
        // curr lcp is going to be the left lcp
        return simple_accel(sa_object, query, center + 1, right, curr_lcp, right_lcp, order);
    }
}


/*
 * Longest common prefix used in simple_accel algorithm. Ideally, a function could return both the comparison and the
 * length of the lcp of two strings. But instead of implementing a function that does both, I use str.compare() for
 * comparison and this one for length of lcp. If I were using a function that does both, the naive one would be slower
 * (because I don't see myself implementing a faster string comparison than std).
 * */
uint32_t querysa::lcp(std::string query, std::string& genome, uint32_t suffix_start) {
    uint32_t counter = 0;
    for (uint32_t i = 0; i < query.length(); i++) {
        if (query[i] == genome[suffix_start + i]) {
            counter++;
        } else {
            return counter;
        }
    }
}
