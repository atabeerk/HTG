//
// Created by Ataberk Donmez on 24.04.2022.
//

#ifndef HTG_QUERYSA_H
#define HTG_QUERYSA_H

#include "SuffixArray.hpp"

class querysa {
public:
    querysa();
    bool query(SuffixArray sa_object, std::string query, std::string query_mode, std::string output, std::vector <uint32_t>& occ);

private:
    int naive_search(SuffixArray sa_object, std::string query, uint32_t left, uint32_t right, std::string order);
    int simple_accel(SuffixArray sa_object, std::string query, uint32_t left, uint32_t right, uint32_t left_lcp, uint32_t right_lcp, std::string order);
    uint32_t lcp(std::string query, std::string genome, uint32_t suffix_start);
};


#endif //HTG_QUERYSA_H
