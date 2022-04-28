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
                inputs[0] = optarg;
                break;
            case 'q':
                inputs[1] = optarg;
                break;
            case 'm':
                inputs[2] = optarg;
                break;
            case 'o':
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
    std::map<std::string, std::string> output_strings;
    while (ReadFASTA(ffp, &seq, &name, &L)) {
        //printf("name: %s\n", name);
        //printf("size: %d\n", L);
        std::vector<uint32_t> occ;
        std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();;
        bool r = q.query(sa_object, seq, query_mode, occ);
        std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double, std::milli> diff = t2 - t1;
        naive_times.push_back(diff);
        std::string name_str = name;
        if (!r) {
            output_strings[name] = name_str + "\t" + "0" + "\n";
        } else {
            int count = occ[1] - occ[0] + 1;
            std::string tmp = name_str + "\t" + std::to_string(count);
            for (int i = occ[0]; i <= occ[1]; i++) {
                tmp += "\t" + std::to_string(sa_object.sa[i]);
            }
            tmp += "\n";
            output_strings[name] = tmp;
        }
    }
    CloseFASTA(ffp);
    // Write the results to a file
    std::ofstream output_file;
    output_file.open(output_path);
    double total_time = 0;
    for (auto i : naive_times) {
        total_time += i.count();
    }
    output_file << total_time << std::endl;
    for (auto i = output_strings.begin(); i != output_strings.end(); i++) {
        output_file << i->second;
    }
    output_file.close();
    std::cout << total_time << std::endl;

}

