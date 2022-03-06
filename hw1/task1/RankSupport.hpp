#ifndef RANKSUPPORT_H
#define RANKSUPPORT_H

#include <string>
#include <vector>

#include <sdsl/bit_vectors.hpp>


using namespace std;
using namespace sdsl;

class RankSupport {
    public:
        RankSupport(bit_vector *b, bool debug = false);
        uint64_t rank1(uint64_t i);
        uint64_t overhead();
        void save(string& fname);
        void load(string& fname);
        uint64_t get_size();
        uint64_t to_decimal(bit_vector *b);
        bit_vector to_bitvector(uint64_t i);

    private:
        bool debug;
        bit_vector *b;
        uint64_t sb_size;
        uint64_t block_size;
        uint64_t table_size;
        vector<bit_vector> superblocks;
        vector<bit_vector> blocks;
        vector<vector<bit_vector>> tables;
        void fill_superblocks();
        void fill_blocks();
        void fill_tables();

};

#endif
