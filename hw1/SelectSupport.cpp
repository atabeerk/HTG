#include "SelectSupport.hpp"

SelectSupport::SelectSupport() {

}

SelectSupport::SelectSupport(RankSupport *r) {
    this->r = r;
    this->size = r->size();
}


uint64_t SelectSupport::select1(uint64_t i) {
    /* 0 indexing */
    if (i == 0) {
        cout << "INVALID INPUT" << endl;
        return 0;
    }
    return bsearch(i, size / 2, 0, size - 1);

}


uint64_t SelectSupport::bsearch(uint64_t rank, uint64_t index, uint64_t start, uint64_t end) {
    if (index >= size || index < 0) {
        cout << "*** NO INDEX WITH RANK " << rank << " ***" <<  endl;
        return 0;
    }
    uint64_t current_rank = r->rank1(index);

    if (current_rank == rank) {
        /* Found "an" index with the required rank but is it the first index with that rank? */
        if (index == 0 || (r->rank1(index - 1) == current_rank - 1)) {
            /* If the index is 0 or if the rank one left of the current index is less than current rank we are done */
            return index;
        }
        else {
            // Keep looking in the left part
            return bsearch(rank, (start + index) / 2, start, index);
        }
    }
    /* continue with the right part */
    else if (current_rank < rank) {
        return bsearch(rank, (index + end) / 2, index, end);
    }
    /* continue with the left part */
    else {
        return bsearch(rank, (start + index) / 2, start, index);
    }
}


uint64_t SelectSupport::overhead() {
    /* SelectSupport class does not contain data structures other than the RankSupport*/
    return r->overhead();
}


void SelectSupport::save(string& fname) {
    r->save(fname);
}


void SelectSupport::load(string& fname) {
    r->load(fname);
    size = r->size();
}