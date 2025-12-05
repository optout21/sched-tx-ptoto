/**
 * Collection of things needed from (rest of) the bitcoin core.
 */

#pragma once

#include <cstdint>
#include <vector>

typedef std::vector<uint8_t> ByteArray;
typedef ByteArray Hash;
typedef Hash Txid;

static const uint8_t HASH_SIZE = 32;

class CTransaction
{
private:
    ByteArray txdata;
    Txid hash;

public:
    CTransaction() {
        this->SetHash();
    }
    CTransaction(const std::vector<uint8_t>& txdata) : txdata(std::move(txdata)) {
        this->SetHash();
    }

    const size_t size() const { return this->txdata.size(); }
    const ByteArray& data() const { return this->txdata; }
    const Txid& GetHash() const { return this->hash; }

protected:
    void SetHash();
};
