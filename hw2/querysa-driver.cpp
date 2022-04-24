//
// Created by Ataberk Donmez on 24.04.2022.
//

#include <getopt.h>
#include <iostream>

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
    while (ReadFASTA(ffp, &seq, &name, &L)) {
        printf("name: %s\n", name);
        printf("size: %d\n", L);
        // process the queries based on the algorithm actual functions are in
    }
    CloseFASTA(ffp);



}

