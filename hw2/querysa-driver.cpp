//
// Created by Ataberk Donmez on 24.04.2022.
//

#include <iostream>
#include <ctime>
#include <getopt.h>
#include <fstream>

#include "fasta/fasta.h"

#include "querysa.hpp"
#include "SuffixArray.hpp"



std::string* processArgs(int argc, char** argv) {

    // [genome_path, output_path, prefix_size]
    std::string* inputs = new std::string[4];
    const char* short_opts = "i:q:m:o:";
    const option long_opts[] = {
            {"index", required_argument, nullptr, 'i'},
            {"queries", required_argument, nullptr, 'q'},
            {"query_mode", required_argument, nullptr, 'm'},
            {"output", required_argument, nullptr, 'o'},
    };

    while (true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt) {
            case 'i':
                std::cout << optarg << std::endl;
                inputs[0] = optarg;
                break;
            case 'q':
                std::cout << optarg << std::endl;
                inputs[1] = optarg;
                break;
            case 'm':
                std::cout << optarg << std::endl;
                inputs[2] = optarg;
                break;
            case 'o':
                std::cout << optarg << std::endl;
                inputs[3] = optarg;
                break;
        }
    }
    return inputs;
}

int main(int argc, char** argv) {
    // read arguments
    std::string* inputs = processArgs(argc, argv);
    std::string index_path = inputs[0];
    std::string query_path = inputs[1];
    std::string query_mode = inputs[2];
    std::string output_path = inputs[3];

    std::cout <<  index_path << " " << query_path << " " << query_mode << " " << output_path << std::endl;

    // read suffix array object
    SuffixArray sa_object;
    sa_object.load(index_path);
    // read fasta file containing the queries
    FASTAFILE *ffp;
    char *seq;
    char *name;
    int L;
    ffp = OpenFASTA(&query_path[0]);
    querysa q;
    std::vector<std::chrono::duration<double, std::milli>> naive_times;
    std::ofstream naive_output;
    naive_output.open("naive_output.txt");
    while (ReadFASTA(ffp, &seq, &name, &L)) {
        //printf("name: %s\n", name);
        //printf("size: %d\n", L);
        std::vector<uint32_t> occ;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();;
        bool r = q.query(sa_object, seq, "naive", output_path, occ);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = t2 - t1;
        naive_times.push_back(diff);
        if (!r) {
            naive_output << name << "\t" << "0" << std::endl;
        } else {
            int count = occ[1] - occ[0] + 1;
            naive_output << name << "\t" << count;
            for (int i = occ[0]; i <= occ[1]; i++) {
                naive_output << "\t" << sa_object.sa[i];
            }
            naive_output << std::endl;
        }
    }
    naive_output.close();
    double total_time = 0;
    for (auto i : naive_times) {
        total_time += i.count();
    }
    std::cout << "total naive time:" << total_time / 1000 << std::endl;
    CloseFASTA(ffp);


    ffp = OpenFASTA(&query_path[0]);
    std::vector<std::chrono::duration<double, std::milli>> simpleacc_times;
    std::ofstream simpleacc_output;
    simpleacc_output.open("simpleacc_output.txt");
    while (ReadFASTA(ffp, &seq, &name, &L)) {
        //printf("name: %s\n", name);
        //printf("size: %d\n", L);
        std::vector<uint32_t> occ;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();;
        bool r = q.query(sa_object, seq, "simpleacc", output_path, occ);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = t2 - t1;
        simpleacc_times.push_back(diff);
        if (!r) {
            simpleacc_output << name << "\t" << "0" << std::endl;
        } else {
            int count = occ[1] - occ[0] + 1;
            simpleacc_output << name << "\t" << count;
            for (int i = occ[0]; i <= occ[1]; i++) {
                simpleacc_output << "\t" << sa_object.sa[i];
            }
            simpleacc_output << std::endl;
        }
    }
    total_time = 0;
    for (auto i : simpleacc_times) {
        total_time += i.count();
    }
    std::cout << "total simpleacc time:" << total_time / 1000 << std::endl;
    CloseFASTA(ffp);

}

