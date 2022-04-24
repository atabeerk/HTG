//
// Created by Ataberk Dönmez on 24.04.2022.
//

#ifndef HTG_QUERYSA_H
#define HTG_QUERYSA_H

#include "SuffixArray.hpp"

class querysa {
public:
    querysa();
    void query(SuffixArray sa_object, std::string queries, std::string query_mode, std::string output);
};


#endif //HTG_QUERYSA_H
