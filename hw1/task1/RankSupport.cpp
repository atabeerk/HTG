#include "RankSupport.hpp"
#include <iostream>

RankSupport::RankSupport(bit_vector *b){
    this->b = b;

    uint64_t n = (*b).size();

    /* set this early as superblock size is based on this */
    block_size = 1 + ((log2(n) - 1) / 2); // each block contains ceil(log(n)/2 elements

    /*** HANDLE SUPERBLOCKS ***/
    /* size of each superblock
    * instead of the formula from the slides, superblock size is the square of block size
    * this is so that blocks fit into superblocks "perfectly"
    */
    sb_size = pow(block_size, 2);

    /* number of superblocks */
    /* this rather confusing division is to round up without overflow */
    uint64_t n_sb = 1 + ((n - 1) / sb_size);

    /* create superblocks */
    for (uint64_t i = 0; i < n_sb; i++) {
        superblocks.push_back(bit_vector(sb_size, 0));
    }
    /* fill superblocks with correct rank values */
    fill_superblocks();

    cout << "printing information about " << (*b) << " with length " << (*b).size() << endl;
    cout << "\t" << n_sb << " superblocks, each with size " << sb_size << endl;
    cout << "\tcontents of superblocks are:" << endl;
    for (bit_vector sb : superblocks){
        cout << "\t\t" << sb << endl;
    }

    /*** HANDLE BLOCKS ***/
    /* number of blocks is n_sb * (blocks per superblock) */
    uint64_t n_b = n_sb * (1 + ((sb_size - 1) / block_size));

    /* create blocks */
    for (uint64_t i = 0; i < n_b; i++) {
        blocks.push_back(bit_vector(block_size, 0));
    }
    /* fill superblocks with correct rank values */
    fill_blocks();

    cout << "\t" << n_b << " blocks, each with size " << block_size << endl;
    cout << "\tcontents of blocks are:" << endl;
    for (bit_vector b : blocks){
        cout << "\t\t" << b << endl;
    }

    /*** HANDLE TABLES ***/
    table_size = block_size;
    uint64_t n_table = n_b;
    for (uint64_t i = 0; i < n_table; i++) {
        tables.push_back(bit_vector(table_size, 0));
    }
    fill_tables();
    cout << "\t" << n_table << " tables, each with size " << table_size << endl;
    cout << "\tcontents of tables are:" << endl;
    for (bit_vector t : tables){
        cout << "\t\t" << t << endl;
    }
    cout << "*****" << endl << endl;


}

void RankSupport::fill_superblocks() {
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


void RankSupport::fill_blocks() {
    /* Each block will store the rank form start of its superblock to start of itself */

    uint64_t blocks_per_sb = blocks.size() / superblocks.size();

    /*
    * First, fill each block similar to filling superblocks (code duplication with fill_superblocks :( ).
    * Then, we will subtract the value of block's prev superblock
    */
    uint64_t rank_so_far = 0;
    for (uint64_t block_index = 0; block_index < blocks.size(); block_index++) {
        for(uint64_t i = 0; i < block_size; i++) {
            /* first block always stores 0 */
            if (block_index == 0) {
                continue;
            }

            /* Similar to full_superblock */
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


void RankSupport::fill_tables() {
    /* A bit different than other two fill functions. This time we need the position information as well */

    for (uint64_t table_index = 0; table_index < tables.size(); table_index++) {
        /* store which bits are one in a bit_vector */
        bit_vector bv = bit_vector(table_size, 0);
        for (uint64_t i = 0; i < table_size; i++) {

            /* Jump to start of the current table */
            uint64_t true_index = i + (table_index * table_size);
            if ((*b)[true_index] == 1) {
                bv[i] = 1;
            }
        }
        tables[table_index] = bv;
    }

}


uint64_t RankSupport::rank1(uint64_t i) {
    /* superblock value + block value + popcount */
    bit_vector superblock = superblocks[i / sb_size];
    bit_vector block = blocks[i / block_size];
    bit_vector table = tables[i / table_size];

    uint64_t block_start = ((i / block_size) * block_size);
    uint64_t table_index = (i - block_start);

//    cout << "i: " << i << endl;
//    cout << "sb rank: " << superblock << endl;
//    cout << "block rank: " << block << endl;
//    cout << "table content: " << table << endl;
//    cout << "table index: " << table_index << endl;

    /* Is the following considered O(1) because the table is super small? */
    /* get the value from the table for the corresponding table index */
    uint64_t true_table_rank = 0;
    for (uint64_t j = 0; j < table_index + 1; j++) {
        if (table[j] == 1) {
            true_table_rank++;
        }
    }
    return to_decimal(&superblock) + to_decimal(&block) + true_table_rank;
}


uint64_t RankSupport::overhead() {
    uint64_t sb_overhead = 0, block_overhead = 0, table_overhead = 0;
    /* all of these data structures store bit_vectors so: n_bitvectors * bitvector_size */
    sb_overhead = superblocks.size() * sb_size;
    block_overhead = blocks.size() * block_size;
    table_overhead = tables.size() * table_size;
    return sb_overhead + block_overhead + table_overhead;
}


void RankSupport::save(string& fname) {
    return;
}


void RankSupport::load(string& fname) {
    return;
}

/*** AUXILIARY FUNCTIONS ***/

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