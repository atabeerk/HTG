#include "NaiveRankSupport.hpp"

int main()
{
    bit_vector b = bit_vector(8000, 0);
    for (size_t i = 0; i < b.size(); i += 100){
        b[i] = 1;
    }

    NaiveRankSupport nrs = NaiveRankSupport(&b);
    uint64_t index = 8000;
    auto r = nrs.rank1(index);

    cout << "NRS rank for " << index << " is " << r << endl;
}