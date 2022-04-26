//
// Created by Ataberk Donmez on 22.04.2022.
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
        if (i % 20 == 0) {
            std::cout << i << std::endl;
        }
        // get the suffix (extract) and then get the prefix(substr)
        std::string curr_prefix = extract(sa, sa[i], sa.size() - 1).substr(0, k);

        // starting from the prev suffix keep going left as long as the prefixes are the same
        int j = i;
        while (j >= 0) {
            // compare prefix of the current suffix with the previous ones
            if (curr_prefix.compare(0, k, genome, sa[j], k) != 0) {
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
            if (curr_prefix.compare(0, k, genome, sa[j], k) != 0) {
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
    std::cout << "pt size: " << pt.size() << std::endl;

    if (pt.size() == 0) {
        return;
    }

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


std::vector<uint32_t> SuffixArray::get_search_range(std::string query) {
    // if the prefix is not in pt, return [0, sa.size()]
    std::string prefix = query.substr(0, k);
    if (pt.count(prefix) == 0 || k == 0) {
        std::vector<uint32_t> v = {0, (uint32_t) sa.size() - 1};
        return v;
    }
    return pt[prefix];
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


bool SuffixArray::compare(std::string curr_prefix, uint32_t start) {
    //std::cout << curr_prefix << " " << genome.substr(start, k) << std::endl;
    try {
        return 0 == curr_prefix.compare(start, k, genome);
    }
    catch (const std::out_of_range& oor) {
        return false;
    }
    for (uint32_t i = start; i < start + k; i++) {
        //std::cout << "-\t-" << i << " " << curr_prefix[i - start]  << " " << genome[i] << std::endl;
        if( i >= genome.length() || curr_prefix[i - start] != genome[i]) {
            return false;
        }
    }
    return true;
}
