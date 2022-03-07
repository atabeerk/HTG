#include "RankSupport.hpp"
#include "NaiveRankSupport.hpp"
#include "SelectSupport.hpp"
#include "SparseArray.hpp"

#include <ctime>
#include <string>
#include <chrono>

using namespace std::chrono;

void sparse_array_experiments() {
    SparseArray sa;
    double sparsity = 0.01; // change this value to create arrays with different sparsity
    vector<duration<double, std::milli>> get_rank_times;
    vector<duration<double, std::milli>> get_index_times;
    for (uint64_t i = 0; i < 30; i++) {
        /* Create a sparse array with different size and sparsity each time */
        uint64_t size = 15000 + (5000 * i);
        sa.create(size);
        for (uint64_t j = 0; j < size; j += size * sparsity) {
            sa.append("foo", j);
        }
        string s;

        cout << "Starting get at rank experiments" << endl;
        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        /* For a single array run 100 get_at_rank experiments */
        for (uint64_t j = 1; j < size; j += size / 100) {
            sa.get_at_rank(j, s);
        }
        /* For a single array run 100 get_at_index experiments */
        cout << "get at rank experiments finished" << endl;
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double, std::milli> time_span = t2 - t1;
        get_rank_times.push_back(time_span);

        cout << "Starting get at index experiments" << endl;
        t1 = high_resolution_clock::now();
        for (uint64_t j = 1; j < size * sparsity; j += size * sparsity / 100) {
            sa.get_at_index(j, s);
        }
        t2 = high_resolution_clock::now();
        time_span = t2 - t1;
        get_index_times.push_back(time_span);
        cout << "get at index experiments finished" << endl;
    }
    cout << "Printing get_at_rank times: " << endl;
    for (auto t : get_rank_times) {
        cout << t.count() << ", ";
    }
    cout << endl << "Printing get_at_index times: " << endl;
    for (auto t : get_index_times) {
        cout << t.count() << ", ";
    }

}

void rank_select_experiments(char experiment) {
    vector<uint64_t> overheads;
    vector<duration<double, std::milli>> times;
    for (uint64_t i = 0; i < 30; i++) {
        /* Create bit_vectors of varying size (15k to 165k)*/
        bit_vector bv = bit_vector(15000 + (5000 * i));
        for (uint64_t j = 0; j < bv.size(); j += bv.size() / 100) {
            bv[j] = 1;
        }

        RankSupport rs = RankSupport(&bv);
        overheads.push_back(rs.overhead());

        /* Rank experiments */
        if (experiment == 'R') {
            high_resolution_clock::time_point t1 = high_resolution_clock::now();
            /* Run 100 rank1 experiments with different rank values */
            for (uint64_t k = 0; k < bv.size(); k += bv.size() / 100) {
                rs.rank1(k);
            }
            high_resolution_clock::time_point t2 = high_resolution_clock::now();
            duration<double, std::milli> time_span = t2 - t1;
            times.push_back(time_span);
        }

        /* Select experiments */
        else if (experiment == 'S') {
            SelectSupport ss = SelectSupport(&rs);
            /* Store the overhead */
            overheads.push_back(ss.overhead());
            high_resolution_clock::time_point t1 = high_resolution_clock::now(); // start time
            /* Run 100 select1 experiments with different rank values */
            for (uint64_t k = 5; k < 75; k += 5) {
                ss.select1(k);
            }
            high_resolution_clock::time_point t2 = high_resolution_clock::now(); // end time
            duration<double, std::milli> time_span = t2 - t1; // time difference
            times.push_back(time_span);
        }
    }

    /* Print the results */
    cout <<"Printing overheads:" << endl;
    for (auto o : overheads) {
        cout << o << ", ";
    }
    cout << endl;
    cout << "Printing times:" << endl;
    for (auto t : times) {
        cout << t.count() << ", ";
    }
    cout << endl;
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
    //rank_select_experiments('S');
    sparse_array_experiments();
}