//
// Created by Ataberk Dönmez on 22.04.2022.
//

#include "SuffixArray.hpp"
#include <iostream>
#include <memory>

SuffixArray::SuffixArray() {

}

SuffixArray::SuffixArray(std::string genome, std::string output, uint32_t k) {
    std::cout << "Creating a SA object" << std::endl;
    sdsl::construct_im(sa, std::string(genome), 1);
    this->output = output;
    this->genome = genome;
    this->k = k;

    std::vector<uint32_t> v;
    v.push_back(5);
    v.push_back(10);
    for (int i = 0; i < genome.length(); i++) {
        pt["asd"] = v;
    }

    if (k != 0) {
        generate_pt();
    }
}

SuffixArray::SuffixArray(std::string genome, std::string output) : SuffixArray(genome, output, 0) {}


void SuffixArray::generate_pt() {
    // for each suffix, look at its k-len prefix, figure out how many suffixes before or after that
    // has the same prefix and store this information (start and end index of suffixes with this prefix)
    // in the pt
    for (uint32_t i = 1; i < sa.size(); i++) {
        // get the suffix (extract) and then get the prefix(substr)
        std::string curr_prefix = extract(sa, sa[i], sa.size() - 1).substr(0, k);

        // starting from the current suffix keep going left as long as the prefixes are the same
        uint32_t j = i;
        while (j >= 0) {
            // compare prefix of the current suffix with the previous ones
            if (curr_prefix.compare(extract(sa, sa[j], sa.size() - 1).substr(0, k)) != 0) {
                break;
            }
            j--;
        }
        std::cout << "i:" << i <<" j:" << j << std::endl;
    }
}


std::string SuffixArray::get_genome() {
    return genome;
}

void SuffixArray::save() {
    std::ofstream os(output + ".bin");
    cereal::BinaryOutputArchive oarchive(os); // stream to cout

    oarchive(CEREAL_NVP(*this));
    sdsl::store_to_file(sa, output + ".sdsl");

}

void SuffixArray::load(std::string filename) {
    std::ifstream is(filename + ".bin");
    cereal::BinaryInputArchive archive(is);

    archive(*this);
    sdsl::load_from_file(sa, output + ".sdsl");

}
