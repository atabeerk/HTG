#include "RankSupport.hpp"
#include "NaiveRankSupport.hpp"

#include <ctime>

int main()
{
    bit_vector b = bit_vector(16, 0);
    b[1] = 1;
    for (size_t i = 0; i < b.size(); i++) {
        b[i] = 1;
    }
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
    cout << endl << "****************" << endl;
    bit_vector bv1 = {1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,1,0,0,1,1,0};
    bit_vector bv2 = {1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1};
    bit_vector bv3 = {1,1,0,0,1,1,0,0,1,1,1,0,1,1,0,1,0,0,0,1,1,1,0,1,0,1,0,0,1,0,1,0};
    bit_vector bv4 = {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0 ,1 ,0};

    //RankSupport rs1 = RankSupport(&bv1, true);
    //RankSupport rs2 = RankSupport(&bv2);
    //RankSupport rs3 = RankSupport(&bv3);
    RankSupport rs4 = RankSupport(&bv4);

     // cout << bv4 << " " <<  rs4.rank1(15) << endl;
     //cout << "PASSED " << ((result == true_result && bv1[x] == 0) || ((result == (true_result + 1)) && bv1[x] == 1)) << endl;

//    for (uint64_t c = 1; c < 10; c++) {
//        time_t start_time;
//        bit_vector b = bit_vector(10000 * c, 0);
//        for (size_t i = 0; i < b.size(); i += 100) {
//            b[i] = 1;
//        }
//        RankSupport rs = RankSupport(&b);
//        time(&start_time);
//        rs.rank1(5000);
//        cout <<  time(NULL) - start_time << " seconds for c=" << c << endl;
//        cout << rs.overhead() << endl;
//    }
}