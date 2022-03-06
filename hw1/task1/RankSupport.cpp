#include "RankSupport.hpp"

RankSupport::RankSupport(bit_vector *b){
    this->b = b;

    uint64_t n = (*b).size();

    /*** HANDLE SUPERBLOCKS ***/
    /* size of each superblock
    * (number of elements each superblock corresponds to in the bit vector)
    * ceil(log^2n/2) bits, rounding up
    * rounding up formula for x / y is 1 + ((x - 1) / y)
    */
    uint64_t sb_size = 1 + ((pow(log2(n), 2) - 1) / 2);

    /* number of superblocks */
    /* this rather confusing division is to round up without overflow */
    uint64_t n_sb = 1 + ((n - 1) / sb_size);

    /* create superblocks */
    for (uint64_t i = 0; i < n_sb; i++) {
        superblocks.push_back(bit_vector(sb_size, 0));
    }
    /* fill superblocks with correct rank values */
    fill_superblocks(sb_size);

    cout << "printing information about " << (*b) << " with length " << (*b).size() << endl;
    cout << "\t" << n_sb << " superblocks, each with size " << sb_size << endl;
    cout << "\t values of superblocks are:" << endl;
    for (bit_vector sb : superblocks){
        cout << "\t\t" << sb << endl;
    }

    /*** HANDLE BLOCKS ***/
    /* each block contains log(n)/2 elements */
    uint64_t block_size = log2(n) / 2;
    /* number of blocks is n_sb * (blocks per superblock) */
    uint64_t n_b = n_sb * (1 + ((sb_size - 1) / block_size));

    /* create blocks */
    for (uint64_t i = 0; i < n_b; i++) {
        blocks.push_back(bit_vector(block_size, 0));
    }
    /* fill superblocks with correct rank values */
    fill_blocks(sb_size, block_size);

    cout << "\t" << n_b << " blocks, each with size " << block_size << endl;
    cout << "\t values of blocks are:" << endl;
    for (bit_vector b : blocks){
        cout << "\t\t" << b << endl;
    }
    cout << "*****" << endl << endl;

}

void RankSupport::fill_superblocks(uint64_t sb_size) {

    uint64_t rank_so_far = 0; // initially 0
    for (uint64_t sb_index = 0; sb_index < superblocks.size(); sb_index++) {
        for (uint64_t j = 0; j < sb_size; j++) {
            /* We need this shortcut for the first superblock which always stores 0*/
            if (sb_index == 0) {
                continue;
            }

            /*
            * j stores the index inside current superblock. We need to add number of bits
            * up until the beginning of the current superblock to get the true index
            */
            /* (sb_index - 1) because we should only count rank until start of current superblock */
            uint64_t true_index = j + ((sb_index - 1) * sb_size);

            /*
            * Make sure we are still in bounds.
            * Need to check this because n_sb * sb_size may be greater than b.size()
            */
            if (true_index >= (*b).size()) {
                break;
            }
            if ((*b)[true_index] == 1) {
                /* as we go through the bits check if it is 1 and increase the current rank */
                rank_so_far++;
            }
        }
        /* Rank for this superblock is computed, store it */
        superblocks[sb_index] = to_bitvector(rank_so_far);
    }
}


void RankSupport::fill_blocks(uint64_t sb_size, uint64_t block_size) {
    /* Each block will store the rank form start of its superblock to start of itself */

    uint64_t blocks_per_sb = blocks.size() / superblocks.size();

    /*
    * First, fill each block similar to filling superblocks (code duplication with fill_superblocks :( ).
    * Then, we will subtract the value of block's prev superblock
    */
    uint64_t rank_so_far = 0;
    for (uint64_t block_index = 0; block_index < blocks.size(); block_index++) {
        for(uint64_t i = 0; i < block_size; i++) {
            /* first block always stores 0*/
            if (block_index == 0) {
                continue;
            }

            uint64_t true_index = i + ((block_index - 1) * block_size);

            if (true_index >= (*b).size()) {
                break;
            }

            if ((*b)[true_index] == 1) {
                rank_so_far++;
            }
        }
        uint64_t sb_index = block_index / blocks_per_sb;
        blocks[block_index] = to_bitvector(rank_so_far - to_decimal(&superblocks[sb_index]));
    }


}


uint64_t RankSupport::rank1(uint64_t index) {
    return 0;
}


uint64_t RankSupport::overhead() {
    return 0;
}


void RankSupport::save(string& fname) {
    return;
}


void RankSupport::load(string& fname) {
    return;
}

/* builtin get_int method is broken so sb_index need to write my own */
uint64_t RankSupport::to_decimal(bit_vector *b) {
    uint64_t decimal = 0;
    uint64_t n_bits = (*b).size();
    for (uint64_t sb_index = 0; sb_index < n_bits; sb_index++) {
        if ((*b)[sb_index] == 1) {
            /* first element (index 0) is the msb of the bitvector: CONFUSING */
            decimal += pow(2, sb_index);
        }
    }
    return decimal;
}


bit_vector RankSupport::to_bitvector(uint64_t decimal) {

    /* log(0) is undefined so here is a shortcut: */
    if (decimal == 0) {
        return bit_vector(1, 0);
    }
    /* compute the length of the binary represention and create the bitvector accordingly */
    char bitvector_length = log2(decimal) + 1;
    bit_vector result = bit_vector(bitvector_length, 0); // initially all zeros
    char vector_index = 0;
    /* apply the conversion algorithm based on : https://geeksforgeeks.org/program-decimal-binary-conversion/ */
    while(decimal){
        if(decimal & 1) {
            // 1
            result[vector_index] = 1;
        }
        else { // 0
          result[vector_index] = 0;
        }
        decimal>>=1; // Right Shift by 1
        vector_index++; // increment index by 1;
    }
    return result;
}