//
// Created by Ataberk Dönmez on 22.04.2022.
//

#include <getopt.h>
#include <iostream>
#include <sstream>
#include <fstream>

#include "SuffixArray.hpp"
#include "fasta/fasta.h"


#include <cereal/archives/xml.hpp>

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
    uint64_t prefix_len = std::atoi(inputs[2].c_str());

    FASTAFILE *ffp;
    char *seq;
    char *name;
    int L;
    ffp = OpenFASTA("example/ecoli.fa");

    ReadFASTA(ffp, &seq, &name, &L);
    printf("name: %s\n", name);
    printf("size: %d\n", L);
    CloseFASTA(ffp);

    SuffixArray sa = SuffixArray(seq, &output_path[0]);
    free(seq);
    free(name);

//    sdsl::csa_bitcompressed<> csa;
//    construct_im(csa, "abaaba", 1);
//    cout << "csa.size(): " << csa.size() << endl;
//    cout << "csa.sigma : " << csa.sigma << endl;
//    cout << csa << endl;  // output CSA
//    cout << extract(sa.sa, 0, sa.sa.size()-1) << endl;


    {
        {
            std::ofstream os("output.xml");
            cereal::XMLOutputArchive oarchive(os); // stream to cout

            SuffixArray myStuff(seq, &output_path[0]);

            oarchive(CEREAL_NVP(myStuff));

        }

        {
            std::ifstream is("output.xml");
            cereal::XMLInputArchive archive(is);

            SuffixArray myStuff;

            archive( myStuff ); // NVPs not strictly necessary when loading
            std::cout << myStuff.x << std::endl;
            // but could be used (even out of order)
        }
    }

    return 0;



}
