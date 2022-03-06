#ifndef SELECTSUPPORT_H
#define SELECTSUPPORT_H

#include "RankSupport.hpp"
#include <string>


class SelectSupport {
    public:
        SelectSupport(RankSupport *r);
        uint64_t select1(uint64_t i);
        uint64_t overhead();
        void save(string& fname);
        void load(string& fname);
        uint64_t bsearch(uint64_t rank, uint64_t index, uint64_t start, uint64_t end);
    private:
        RankSupport *r;
        uint64_t size;
};

#endif