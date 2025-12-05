#include "bitcoincore.h"

void CTransaction::SetHash() {
    this->hash.clear();
    auto datasize{this->txdata.size()};
    // set dummy hash
    Txid h;
    h.resize(HASH_SIZE);
    if (datasize < 1) {
        for (auto i{0}; i < HASH_SIZE; ++i) {
            h[i] = 0;
        }
    } else {
        for (auto i{0}; i < HASH_SIZE; ++i) {
            h[i] = this->txdata[datasize - 1 - i % datasize];
        }
    }
    this->hash = h;
}
