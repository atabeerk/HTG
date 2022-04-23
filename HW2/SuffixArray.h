//
// Created by Ataberk Donmez on 22.04.2022.
//


#ifndef SUFFIXARRAY_H
#define SUFFIXARRAY_H

#include <string>

class SuffixArray {

public:
    SuffixArray();
    void save();
    void load();

private:
    string genome;
    string filename;
};


#endif //SUFFIXARRAY_H
