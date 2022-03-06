#include <iostream>
#include <string>
#include <vector>
#include <cmath>

#include <sdsl/bit_vectors.hpp>


using namespace std;
using namespace sdsl;

class RankSupport {
    public:
        RankSupport(bit_vector *b);
        uint64_t rank1(uint64_t index);
        uint64_t overhead();
        void save(string& fname);
        void load(string& fname);
        uint64_t to_decimal(bit_vector *b);
        bit_vector to_bitvector(uint64_t i);

    private:
        bit_vector *b;
        /* These should be bit vectors of bit vectors */
        /* Should include superblocks, blocks and table here */
        /* Each element (which is a bit_vector) of superblock is log^2n/2 bits*/
        vector<bit_vector> superblocks;
        vector<bit_vector> blocks;
        vector<bit_vector> table;
        /* Superblock size=(logn*logn)/2 */
        /* Block size= logn/2 */
        /* there are 2^(block size) possible blocks this is the table size */
        void fill_superblocks(uint64_t sb_size);
        void fill_blocks(uint64_t sb_size, uint64_t block_size);

};

