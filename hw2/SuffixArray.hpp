//
// Created by Ataberk Donmez on 22.04.2022.
//


#ifndef SUFFIXARRAY_H
#define SUFFIXARRAY_H

#include <string>
#include <unordered_map>

#include <sdsl/suffix_arrays.hpp>
#include "cereal/archives/binary.hpp"
#include <cereal/types/vector.hpp>
#include "cereal/types/unordered_map.hpp"
#include <cereal/types/string.hpp>

class SuffixArray {

public:
    SuffixArray();
    SuffixArray(std::string genome, std::string output, uint32_t k);
    SuffixArray(std::string genome, std::string output);
    void save();
    void load(std::string filename);

    std::string get_genome();

    sdsl::csa_bitcompressed<> sa;

private:
    friend class cereal::access;

    std::string output;
    std::string genome;
    uint32_t k; // prefix length
    std::unordered_map<std::string, std::vector<uint32_t>> pt;

    void generate_pt();

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar(genome, output, pt);
    }
};


#endif //SUFFIXARRAY_H
