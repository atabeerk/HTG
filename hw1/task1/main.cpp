#include "RankSupport.hpp"
#include "NaiveRankSupport.hpp"
#include "SelectSupport.hpp"
#include "SparseArray.hpp"

#include <ctime>
#include <string>
#include <chrono>

using namespace std::chrono;

void sparse_array_experiments(double sparsity) {
    SparseArray sa;
    cout << "sparsity: " << sparsity << endl;
    vector<duration<double, std::milli>> get_rank_times;
    vector<duration<double, std::milli>> get_index_times;
    vector<uint64_t> sizes;
    for (uint64_t i = 1; i < 11; i++) {
        /* Create a sparse array with different size and sparsity each time */
        uint64_t size = 100000 * i;
        sa.create(size);
        cout << "starting experiments for size: " << size << endl;
        for (uint64_t j = 0; j < size; j +=  1 / sparsity) {
//            if ( j % 10000 == 0) {
//                cout << j << endl;
//            }
            sa.append("foo", j);
        }
        string s;

        high_resolution_clock::time_point t1 = high_resolution_clock::now();
        /* For a single array, run 100 get_at_rank experiments */
        int c = 0;
        for (uint64_t j = 1; j < size * sparsity; j += (size * sparsity) / 100) {
            c++;
            sa.get_at_rank(j, s);
        }
        cout << c << " get_at_rank experiments run" << endl;
        /* For a single array, run 100 get_at_index experiments */
        high_resolution_clock::time_point t2 = high_resolution_clock::now();
        duration<double, std::milli> time_span = t2 - t1;
        get_rank_times.push_back(time_span);

        t1 = high_resolution_clock::now();
        c = 0;
        for (uint64_t j = 1; j < size; j += size / 100) {
            c++;
            sa.get_at_index(j, s);
        }
        cout << c << " get_at_index experiments run" << endl;
        t2 = high_resolution_clock::now();
        time_span = t2 - t1;
        get_index_times.push_back(time_span);

        sizes.push_back(sa.size());
    }
    cout << "Printing get_at_rank times: " << endl;
    for (auto t : get_rank_times) {
        cout << t.count() << ", ";
    }
    cout << endl << "Printing get_at_index times: " << endl;
    for (auto t : get_index_times) {
        cout << t.count() << ", ";
    }
    cout << endl << "Printing sizes: " << endl;
    for (auto s : sizes) {
        cout << s << ", ";
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
    bit_vector b = bit_vector(500000, 0);
    b[1] = 1;
    for (size_t i = 0; i < b.size(); i++) {
        b[i] = 1;
    }
    rank_support_v<1> b_rank(&b);

    NaiveRankSupport nrs = NaiveRankSupport(&b);
    uint64_t index = 8000;
    auto r = nrs.rank1(index);


    cout << endl << "****************" << endl;
    bit_vector bv1 = {1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,0};
    bit_vector bv2 = {1,0,1,1,0,1,0,1,0,0,1,1,0,0,1,1};
    bit_vector bv3 = {1,1,0,0,1,1,0,0,1,1,1,0,1,1,0,1,0,0,0,1,1,1,0,1,0,1,0,0,1,0,1,0};
    bit_vector bv4 = {1, 0, 0, 1, 0, 1, 1, 1, 0, 1, 0, 0, 1, 0 ,1 ,0};

    RankSupport rs1 = RankSupport(&bv1);
    RankSupport rs2 = RankSupport(&bv2);
    RankSupport rs3 = RankSupport(&bv3);
    RankSupport rs4 = RankSupport(&bv4);

    SelectSupport ss1 = SelectSupport(&rs1);
    SelectSupport ss2 = SelectSupport(&rs2);
    SelectSupport ss3 = SelectSupport(&rs3);
    SelectSupport ss4 = SelectSupport(&rs4);


    //rank_select_experiments('S');
    sparse_array_experiments(0.01);
    sparse_array_experiments(0.05);
    sparse_array_experiments(0.1);

}