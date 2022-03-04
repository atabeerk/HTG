#include <iostream>
#include <sdsl/bit_vectors.hpp>

using namespace std;
using namespace sdsl;

class NaiveRankSupport {
    public:
        NaiveRankSupport(bit_vector *b);
        uint64_t rank1(uint64_t i);
    private:
        bit_vector *b;

};