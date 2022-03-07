#include "SparseArray.hpp"

SparseArray::SparseArray() {

}


void SparseArray::create(uint64_t size) {
    sparse_bitvector = bit_vector(size, 0);
    /* can't initialize rs and ss instances here as they are modified after every append */
}



void SparseArray::append(string elem, uint64_t pos) {
    values.push_back(elem);
    sparse_bitvector[pos] = 1;
    /* Need to reinitialize rs and ss after each append */
    rs = RankSupport(&sparse_bitvector);
    ss = SelectSupport(&rs);
}


bool SparseArray::get_at_rank(uint64_t r, string& elem) {
    if (values.size() >= r) {
        /* 1st element is the element at index 0 (subtract 1) */
        elem = values[r];
        return true;
    }
    return false;
}


bool SparseArray::get_at_index(uint64_t r, string& elem) {
    bool bit = sparse_bitvector[r];
    if (!bit) {
        return false;
    }
    /* find the index in values vector corresponding to r in bit_vector */
    /* rank counts the 1's (starts from 1) whereas indexing starts from 0, thus subtract 1 */
    uint64_t i = rs.rank1(r) - 1;
    /* bind elem to the string at index i */
    elem = values[i];
    return true;
}


uint64_t SparseArray::num_elem_at(uint64_t r) {
    return rs.rank1(r);
}


uint64_t SparseArray::size() {
    uint64_t values_size = 0;
    for (string s : values) {
        values_size = sizeof(char) * s.length();
    }
    return sparse_bitvector.size() + values_size + rs.overhead() + ss.overhead();
}


uint64_t SparseArray::num_elem() {
    return rs.rank1(rs.size());
}


void SparseArray::save(string& fname) {
    return;
}


void SparseArray::load(string& fname) {
    return;
}


