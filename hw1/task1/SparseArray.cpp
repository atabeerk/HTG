#include "SparseArray.hpp"

SparseArray::SparseArray() {

}

SparseArray::SparseArray(bit_vector b, vector<string> v) {
    sparse_bitvector = b;
    values = v;
    // add one for good measure :)
    sparse_bitvector[sparse_bitvector.size() - 1] = 0;
    append("foo", sparse_bitvector.size());
}


void SparseArray::create(uint64_t size) {
    sparse_bitvector = bit_vector(size, 0);
    /* can't initialize rs and ss instances here as they are modified after every append */
}



void SparseArray::append(string elem, uint64_t pos) {
    /* put the elem in values array and set the corresponding bit in the sparse_bitvector */
    values.push_back(elem);
    sparse_bitvector[pos] = 1;

    /* Need to reconstruct rs and ss after each append */
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
    if (r > sparse_bitvector.size()) {
        cout << "r is out of bounds" << endl;
        return false;
    }
    bool bit = sparse_bitvector[r];
    if (!bit) {
        /* If the bit is 0, simply return false */
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
    uint64_t c = 0;
    for (string s : values) {
        values_size += sizeof(char) * s.length() * 8;
        c++;
    }
    /* Not including size of the SelecSupport as it stores a pointer to RankSupport */
    return values_size + rs.overhead();
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


