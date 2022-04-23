//
// Created by Ataberk Donmez on 22.04.2022.
//


#ifndef SUFFIXARRAY_H
#define SUFFIXARRAY_H

#include <string>
#include <sdsl/suffix_arrays.hpp>
#include "cereal/archives/binary.hpp"
#include "cereal/types/memory.hpp"


class SuffixArray {

public:
    SuffixArray();
    SuffixArray(char* genome, char* output);
    friend class cereal::access;

    sdsl::csa_bitcompressed<> sa;
    std::unique_ptr<char> genome;
    char* output;
    int x;
private:
    friend class cereal::access;

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar( x, genome );
    }
};


#endif //SUFFIXARRAY_H
