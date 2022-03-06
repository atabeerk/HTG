#include "RankSupport.hpp"
#include "NaiveRankSupport.hpp"

int main()
{
    bit_vector b = bit_vector(80*(1<<20), 0);
    b[1] = 1;

    rank_support_v<1> b_rank(&b);
    // cout << b << " b_rank: " << b_rank(2) << endl;
    cout << size_in_mega_bytes(b) << endl;

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
    //RankSupport rs2 = RankSupport(&bv2);
    //RankSupport rs3 = RankSupport(&bv3);
    //RankSupport rs4 = RankSupport(&bv4);

    uint64_t x = 21;
    uint64_t result = rs1.rank1(x);
    rank_support_v<1> true_rank(&bv1);
    uint64_t true_result = true_rank(x);
    // cout << bv2 << " " <<  rs2.rank1(1) << endl;
    // cout << "PASSED " << ((result == true_result && bv1[x] == 0) || ((result == (true_result + 1)) && bv1[x] == 1)) << endl;
    cout << rs1.overhead() << endl;

}