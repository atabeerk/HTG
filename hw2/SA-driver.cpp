//
// Created by Ataberk Dönmez on 22.04.2022.
//

#include <getopt.h>
#include <iostream>

#include "SuffixArray.hpp"
#include "fasta/fasta.h"


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

    ReadFASTA(ffp, &seq, &name, &L);
    printf("name: %s\n", name);
    printf("size: %d\n", L);
    CloseFASTA(ffp);

    SuffixArray myStuff(seq, output_path, k);


    cout << "csa.size(): " << myStuff.sa.size() << endl;
    cout << "csa.sigma : " << myStuff.sa.sigma << endl;
    //cout << "csa : " << myStuff.sa << endl;
    cout << extract(myStuff.sa, myStuff.sa.size()-10, myStuff.sa.size()-1) << endl;

    myStuff.save();
    SuffixArray n;
    n.load("output");
    std::cout << n.get_genome().length() << "this was n" << std::endl;

    cout << "csa.size(): " << n.sa.size() << endl;
    cout << "csa.sigma : " << n.sa.sigma << endl;
    cout << "csa : " << n.sa << endl;
    cout << extract(n.sa, n.sa[66], n.sa.size()-1) << endl;

    free(seq);
    free(name);


    return 0;



}
