#include "NaiveRankSupport.hpp"
#include "RankSupport.hpp"

int main()
{
    bit_vector b = bit_vector(15, 0);
    b[1] = 1;

    rank_support_v<1> b_rank(&b);
    cout << b << " b_rank: " << b_rank(2) << endl;

    NaiveRankSupport nrs = NaiveRankSupport(&b);
    uint64_t index = 8000;
    auto r = nrs.rank1(index);

    cout << "NRS rank for " << index << " is " << r << endl;

    uint64_t d1 = 32;
    uint64_t d2 = 57;
    uint64_t d3 = 5;
    bit_vector bv = {1, 1, 1, 1, 1, 1, 1};
    RankSupport rs = RankSupport(&bv);
    cout << endl << "****************" << endl;
    bit_vector bv1 = {1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,1,0,0,1,1,0};
    bit_vector bv2 = {1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1};
    bit_vector bv3 = {1,1,0,0,1,1,0,0,1,1,1,0,1,1,0,1,0,0,0,1,1,1,0,1,0,1,0,0,1,0,1,0};
    bit_vector bv4 = {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0 ,1 ,0};

    RankSupport rs1 = RankSupport(&bv1);
    RankSupport rs2 = RankSupport(&bv2);
    RankSupport rs3 = RankSupport(&bv3);
    RankSupport rs4 = RankSupport(&bv4);
}