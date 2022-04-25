//
// Created by Ataberk Dönmez on 22.04.2022.
//


#include <iostream>
#include <memory>

#include "SuffixArray.hpp"

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

        // starting from the prev suffix keep going left as long as the prefixes are the same
        uint32_t j = i;
        while (j >= 0) {
            // compare prefix of the current suffix with the previous ones
            if (curr_prefix.compare(extract(sa, sa[j], sa.size() - 1).substr(0, k)) != 0) {
                break;
            }
            j--;
        }
        // j stores the index of the first non equal prefix when going left, add 1 to get the last equal in the left.
        j += 1;
        // vector that stores the starting and end indices of range with the same prefix
        std::vector<uint32_t> equal_prefix_range;
        equal_prefix_range.push_back(j);

        j = i;
        while (j < sa.size()) {
            // compare prefix of the current suffix with the previous ones
            if (curr_prefix.compare(extract(sa, sa[j], sa.size() - 1).substr(0, k)) != 0) {
                break;
            }
            j++;
        }
        // j stores the index of the first non equal prefix when going right, subtract 1 to get the last equal in the right.
        j -= 1;
        equal_prefix_range.push_back(j);
        pt[curr_prefix] = equal_prefix_range;
    }
}


void SuffixArray::print_pt() {
    for (uint32_t i = 1; i < sa.size(); i++) {
        std::string curr_prefix = extract(sa, sa[i], sa.size() - 1).substr(0, k);
        std::cout << "i:" << i << " start:" << pt[curr_prefix][0] << " end:" << pt[curr_prefix][1] << std::endl;
        std::cout << "current suffix: " << extract(sa, sa[i], sa.size() - 1) << std::endl;
        std::cout << "first suffix with equal prefix " << extract(sa, sa[pt[curr_prefix][0]], sa.size() - 1) << std::endl;
        std::cout << "last suffix with equal prefix " << extract(sa, sa[pt[curr_prefix][1]], sa.size() - 1) << std::endl;
        std::cout << "-------------" << std::endl;
    }
}


std::string SuffixArray::get_genome() {
    return genome;
}

std::string SuffixArray::get_suffix(uint32_t i) {
    return extract(sa, sa[i], sa.size() - 1);
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
