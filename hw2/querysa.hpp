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
};


#endif //HTG_QUERYSA_H
