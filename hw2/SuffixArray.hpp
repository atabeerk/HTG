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
    void print_pt();
    void save();
    void load(std::string filename);
    std::string get_genome();
    std::vector<uint32_t> get_search_range(std::string prefix);
    std::string get_suffix(uint32_t i);

    sdsl::csa_bitcompressed<> sa;
    std::unordered_map<std::string, std::vector<uint32_t>> pt; //prefix table

private:
    friend class cereal::access;

    void generate_pt();

    std::string output; // output file prefix (without the extension)
    std::string genome;
    uint32_t k; // prefix length

    template <class Archive>
    void serialize( Archive & ar )
    {
        ar(genome, output, pt, k);
    }
};


#endif //SUFFIXARRAY_H
