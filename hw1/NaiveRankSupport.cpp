#include "NaiveRankSupport.hpp"

NaiveRankSupport::NaiveRankSupport(bit_vector *b) {
    this->b = b;
}


uint64_t NaiveRankSupport::rank1(uint64_t index){
    uint64_t rank = 0;
    for (uint64_t i = 0; i < index; i++) {
        if ((*b)[i] == 1) {
            rank++;
        }
    }
    return rank;
}