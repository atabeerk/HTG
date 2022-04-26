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
                std::cout << optarg << std::endl;
                inputs[2] = optarg;
                break;
            case 1:
                printf("Non-option arg: %s\n", optarg);
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
    uint32_t k = std::atoi(inputs[2].c_str());

    FASTAFILE *ffp;
    char *seq;
    char *name;
    int L;
    ffp = OpenFASTA(&genome_path[0]);

    // read the fasta file
    ReadFASTA(ffp, &seq, &name, &L);
    printf("name: %s\n", name);
    printf("size: %d\n", L);
    CloseFASTA(ffp);

    // create a suffix array out of the fasta file
    SuffixArray myStuff(seq, output_path, k);

    // test the suffix array
    cout << "csa.size(): " << myStuff.sa.size() << endl;
    cout << "csa.sigma : " << myStuff.sa.sigma << endl;
    // cout << "csa : " << myStuff.sa << endl;
    cout << extract(myStuff.sa, myStuff.sa.size()-10, myStuff.sa.size()-1) << endl;

    // test the prefix table
    myStuff.print_pt();

    // test save/load
    myStuff.save();
    SuffixArray n;
    n.load("output");
    std::cout << n.get_genome().length() << "this was n" << std::endl;
    n.print_pt();
    cout << n.get_suffix(66) << endl;
    cout << n.get_suffix(0) << endl;
    cout << n.get_suffix(1) << endl;
    string s1 = "TTGCTAACACTGAACCAAAGAACAGA";
    string s2 = "TTGCTAACACTGAACCAAAGAACAGATTGATATCCATGATCACCAGGTTGATGTCATATTCAGAGAGGATCTGATGCATTTCCGCGCCATCTGTCGCTTCGAAAACATCATAGCCTTCCGCTTCGAAAATACTTACTTTACCGATATCATCGCCGCTGTGCGGAGCTTTTCATTCTGACTGCAACGGGCAATATGTCTCTGTGT";
    string s3 = "TTGCTAACACTGAACCAAAGAACAGA";
    cout << s1.compare(s2.substr(0, s1.length())) << endl;
    cout << n.get_search_range("AAASS")[1] << endl;
    cout << (s1 == s3) << endl;
    cout << s1.compare(0, s1.length(), s2) << endl;


    free(seq);
    free(name);


    return 0;



}
