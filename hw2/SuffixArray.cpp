//
// Created by Ataberk Dönmez on 22.04.2022.
//

#include "SuffixArray.hpp"
#include <iostream>
#include <memory>

SuffixArray::SuffixArray() {

}

SuffixArray::SuffixArray(char* genome, char* output) {
    std::cout << "Creating a SA object" << std::endl;
    sdsl::construct_im(sa, std::string(genome), 1);
    x = 15;
    this->output = output;
    this->genome.reset(genome);

}


