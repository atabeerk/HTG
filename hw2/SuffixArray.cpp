//
// Created by Ataberk Donmez on 22.04.2022.
//


#include <iostream>
#include <memory>
#include <ctime>

#include "SuffixArray.hpp"
#include "querysa.hpp"

SuffixArray::SuffixArray() {

}

SuffixArray::SuffixArray(std::string genome, std::string output, uint32_t k) {
    std::chrono::high_resolution_clock::time_point t1 = std::chrono::high_resolution_clock::now();
    sdsl::construct_im(sa, std::string(genome), 1);
    std::chrono::high_resolution_clock::time_point t2 = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> diff = t2 - t1;
    std::cout << "sa construction time with k=" << k << " is " << diff.count() / 1000 << " seconds" << std::endl;
    this->output = output;
    this->genome = genome;
    this->k = k;

    // create a pt if necessary
    if (k != 0) {
        t1 = std::chrono::high_resolution_clock::now();
        generate_pt();
        t2 = std::chrono::high_resolution_clock::now();
        diff = t2 - t1;
        std::cout << "pt construction time with k=" << k << " is " << diff.count() / 1000 << "seconds" << std::endl;
    }
}


// delegate to main constructor
SuffixArray::SuffixArray(std::string genome, std::string output) : SuffixArray(genome, output, 0) {}


/*
 * Populate the prefix table (unordered_map<std::string, std::vector<uint32_t>>). The values in the pt are
 * two dimensional vectors corresponding to start and end indices for a prefix. Starting from the first suffix,
 * keep going right as long as the prefix of length k is shared. If a suffix with a different prefix is found,
 * that is the end of current suffix range.
 * */
void SuffixArray::generate_pt() {
    uint32_t i = 1;
    // init curr_prefix to the prefix of the first suffix
    std::string curr_prefix = extract(sa, sa[i], sa.size() - 1).substr(0, k);
    uint32_t curr_suffix_start = 1;
    char genome_array[genome.length() + 1];
    strcpy(genome_array, genome.c_str());
    char tmp_pref[k + 2];
    while (i < sa.size()) {
        // check if there is a mismatch
        if (genome.compare(sa[curr_suffix_start], k, genome, sa[i], k)) {
            // if there is a mismatch, insert the curr_prefix into the pt and update the curr_prefix
            std::vector<uint32_t> equal_prefix_range = {curr_suffix_start, i - 1};
            strncpy(tmp_pref, genome_array + sa[curr_suffix_start], k);
            tmp_pref[k] = '\0'; // if I don't have this there is aproblem when k > 7 ???
            pt[tmp_pref] = equal_prefix_range;
            curr_suffix_start = i;
        }
        i++;
    }
    // handle the last one
    std::vector<uint32_t> equal_prefix_range = {curr_suffix_start, i - 1};
    strncpy(tmp_pref, genome_array + sa[curr_suffix_start], k);
    pt[tmp_pref] = equal_prefix_range;
}



// For debugging
void SuffixArray::print_pt() {
    std::cout << "pt size: " << pt.size() << std::endl;
    if (pt.size() == 0) {
        return;
    }
    for (uint32_t i = 1; i < sa.size(); i++) {
        std::string curr_prefix = extract(sa, sa[i], sa.size() - 2).substr(0, k);
        std::cout << "current prefix: " << curr_prefix << std::endl;
        std::cout << "i:" << i << " start:" << pt[curr_prefix][0] << " end:" << pt[curr_prefix][1] << std::endl;
        std::cout << "current suffix: " << extract(sa, sa[i], sa.size() - 1) << std::endl;
        std::cout << "first suffix with equal prefix " << extract(sa, sa[pt[curr_prefix][0]], sa.size() - 1)
                  << std::endl;
        std::cout << "last suffix with equal prefix " << extract(sa, sa[pt[curr_prefix][1]], sa.size() - 1)
                  << std::endl;
        std::cout << "-------------" << std::endl;
    }
}


std::string SuffixArray::get_suffix(uint32_t i) {
    return extract(sa, sa[i], sa.size() - 1);
}


/*
 * Wrapper function for accessing pt. Handles the case where the prefix is not in the table.
 * */
std::vector<uint32_t> SuffixArray::get_search_range(std::string query) {
    // if the prefix is not in pt, return [0, sa.size()]
    std::string prefix = query.substr(0, k);
    if (pt.count(prefix) == 0 || k == 0) {
        std::vector<uint32_t> v = {0, (uint32_t) sa.size() - 1};
        return v;
    }
    return pt[prefix];
}


/*
 * Saves all necessary instance variables to a file(.bin) and the SDSL object (suffix array) into another file (.sdsl)
 * */
void SuffixArray::save() {
    std::ofstream os(output + ".bin");
    cereal::BinaryOutputArchive oarchive(os);

    oarchive(CEREAL_NVP(*this));
    sdsl::store_to_file(sa, output + ".sdsl");
}


void SuffixArray::load(std::string filename) {
    std::ifstream is(filename + ".bin");
    cereal::BinaryInputArchive archive(is);

    archive(*this);
    sdsl::load_from_file(sa, output + ".sdsl");
}