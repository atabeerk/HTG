#ifndef SPARSEARRAY_H
#define SPARSEARRAY_H

#include <string>
#include <vector>
#include "RankSupport.hpp"
#include "SelectSupport.hpp"
#include <sdsl/bit_vectors.hpp>


class SparseArray {
    public:
        SparseArray();
        SparseArray(bit_vector b, vector<string> v);
        void create(uint64_t size);
        void append(string elem, uint64_t pos);
        bool get_at_rank(uint64_t r, string& elem);
        bool get_at_index(uint64_t r, string& elem);
        uint64_t num_elem_at(uint64_t r);
        uint64_t size();
        uint64_t num_elem();
        void save(string& fname);
        void load(string& fname);
    private:
        bit_vector sparse_bitvector;
        vector<string> values;
        RankSupport rs;
        SelectSupport ss;
};

#endif