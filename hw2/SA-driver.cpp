//
// Created by Ataberk Donmez on 22.04.2022.
//

#include <getopt.h>
#include <iostream>

#include "fasta/fasta.h"

#include "SuffixArray.hpp"



using namespace std;

std::string* processArgs(int argc, char** argv) {

    // [genome_path, output_path, prefix_size]
    std::string* inputs = new std::string[3];
    const char* short_opts = "-:-:p:";
    const option long_opts[] = {
            {"prefix", required_argument, nullptr, 'p'},
    };

    while (true) {
        const auto opt = getopt_long(argc, argv, short_opts, long_opts, nullptr);

        if (-1 == opt)
            break;

        switch (opt) {
            case 'p':
                inputs[2] = optarg;
                break;
            case 1:
                if (inputs[0] == "")
                    inputs[0] = optarg;
                else
                    inputs[1] = optarg;
                break;
        }
    }
    return inputs;
}


int main(int argc, char **argv) {
    std::string* inputs = processArgs(argc, argv);
    std::string genome_path = inputs[0];
    std::string output_path = inputs[1];
    uint32_t k = 0;
    k = std::atoi(inputs[2].c_str());

    FASTAFILE *ffp;
    char *seq;
    char *name;
    int L;
    ffp = OpenFASTA(&genome_path[0]);

    // read the fasta file
    ReadFASTA(ffp, &seq, &name, &L);
    //printf("name: %s\n", name);
    //printf("size: %d\n", L);
    CloseFASTA(ffp);

    // create a suffix array out of the fasta file
    SuffixArray sa_object(seq, output_path, k);
    free(seq);
    free(name);

//    test the suffix array
//    cout << "csa.size(): " << sa_object.sa.size() << endl;
//    cout << "csa.sigma : " << sa_object.sa.sigma << endl;
//    cout << "csa : " << sa_object.sa << endl;
//    cout << extract(sa_object.sa, sa_object.sa.size()-10, sa_object.sa.size()-1) << endl;

    sa_object.save();

//    test the prefix table
//    sa_object.print_pt();
//    for (auto i = sa_object.pt.begin(); i != sa_object.pt.end(); i++) {
//        std::cout << i->first << std::endl;
//    }

//     test save/load
//    sa_object.save();
//    SuffixArray n;
//    n.load("output");
//    std::cout << n.get_genome().length() << "this was n" << std::endl;
//    n.print_pt();





    return 0;



}
