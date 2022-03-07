#include "RankSupport.hpp"
#include "NaiveRankSupport.hpp"
#include "SelectSupport.hpp"
#include "SparseArray.hpp"

#include <ctime>
#include <string>
#include <chrono>

using namespace std::chrono;


void rank_experiments(char experiment) {
    vector<bit_vector> bv_vector;
    vector <RankSupport> rs_vector;

    cout << "Printing rank overhead:" << endl;
    for (uint64_t i = 0; i < 30; i++) {
        bv_vector.push_back(bit_vector(15000 + (5000 * i), 0));
        for (uint64_t j = 0; j < bv_vector[i].size(); j += bv_vector[i].size() / 100) {
            bv_vector[i][j] = 1;
        }
        rs_vector.push_back(RankSupport(&bv_vector[i]));
        cout << rs_vector[i].overhead() << ", ";
    }
    cout << endl;
    cout << "Test vectors created" << endl;

    if (experiment == 'R') {
        cout << "Printing rank1 times:" << endl;
        for (uint64_t i = 0; i < rs_vector.size(); i++) {
            // start timer here
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            for (uint64_t j = 0; j < rs_vector[i].size(); j += bv_vector[i].size() / 100) {
                rs_vector[i].rank1(j);
            }
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            cout << time_span.count() << ", ";
        }
        cout << endl;
    }
    else if (experiment == 'S') {
        cout << "Printing select1 times:" << endl;
        vector <SelectSupport> ss_vector;
        for (uint64_t i = 0; i < rs_vector.size(); i++) {
            RankSupport rs = rs_vector[i];
            ss_vector.push_back(SelectSupport(&rs));
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            for (uint64_t j = 5; j < 50; j += 1) {
                cout << bv_vector[i].size() << endl;
                auto x = ss_vector[i];
                x.select1(j);
            }
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            cout << time_span.count() << ", ";
        }
    }
}


int main()
{
    bit_vector b = bit_vector(16, 0);
    b[1] = 1;
    for (size_t i = 0; i < b.size(); i++) {
        b[i] = 1;
    }
    rank_support_v<1> b_rank(&b);
    // cout << b << " b_rank: " << b_rank(2) << endl;
    // cout << size_in_mega_bytes(b) << endl;

    NaiveRankSupport nrs = NaiveRankSupport(&b);
    uint64_t index = 8000;
    auto r = nrs.rank1(index);

    cout << "NRS rank for " << index << " is " << r << endl;

//    uint64_t d1 = 32;
//    uint64_t d2 = 57;
//    uint64_t d3 = 5;
//    bit_vector bv = {1, 1, 1, 1, 1, 1, 1};
//    cout << endl << "****************" << endl;
//    bit_vector bv1 = {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0};
//    bit_vector bv2 = {1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1};
//    bit_vector bv3 = {1,1,0,0,1,1,0,0,1,1,1,0,1,1,0,1,0,0,0,1,1,1,0,1,0,1,0,0,1,0,1,0};
//    bit_vector bv4 = {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0 ,1 ,0};
//
//    RankSupport rs1 = RankSupport(&bv1);
//    RankSupport rs2 = RankSupport(&bv2);
//    RankSupport rs3 = RankSupport(&bv3);
//    RankSupport rs4 = RankSupport(&bv4);

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
//    SelectSupport ss1 = SelectSupport(&rs1);
//    cout << ss1.select1(3) << endl;
//
//    SparseArray sa;
//    sa.create(10);
//    sa.append("foo", 1);
//    sa.append("bar", 5);
//    sa.append("baz", 9);
//
//    string e;
//    cout << sa.get_at_rank(1, e) << " e: " << e <<  endl;
//    e = "";
//    cout << sa.get_at_index(3, e) << " e: " << e <<  endl;
//    e = "";
//    cout << sa.get_at_index(5, e) << " e: " << e <<  endl;
//    cout << sa.size() << endl;

    //store_to_file(bv1, "sdb.sdsl");
    //store_to_file(bv2, "sdb.sdsl");

//    cout << bv1 << endl;
//    cout << bv2 << endl;
//
//    bit_vector five_zeros = bit_vector(5, 0), five_ones = bit_vector(5, 1);
//    vector<bit_vector> v;
//    v.push_back(five_zeros);
//    v.push_back(five_ones);
//
//    //store_to_file(v, "sdb.sdsl");
//    store_to_file(bit_vector(5, 0), "sdb.sdsl");
//    store_to_file(bit_vector(5, 1), "sdb.sdsl");
//    bit_vector from_file;
//    load_vector_from_file(from_file, "sdb.sdsl", 1);
//
//    cout << "from_file:" << from_file << endl;
//
//    bit_vector big = bit_vector(20000, 1);
//    RankSupport big_rs(&big);
//    SelectSupport big_ss(&big_rs);
//    for (int i = 1; i < 50; i++) {
//        big_ss.select1(i);
//    }
    rank_experiments('S');
}