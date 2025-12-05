/**
 * Collection of things needed from (rest of) the bitcoin core.
 */

#pragma once

#include <cstdint>
#include <memory>
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

typedef std::shared_ptr<const CTransaction> CTransactionRef;

struct MempoolAcceptResult {
    /** Used to indicate the results of mempool validation. */
    enum class ResultType {
        VALID, //!> Fully validated, valid.
        INVALID, //!> Invalid.
        MEMPOOL_ENTRY, //!> Valid, transaction was already in the mempool.
        DIFFERENT_WITNESS, //!> Not validated. A same-txid-different-witness tx (see m_other_wtxid) already exists in the mempool and was not replaced.
    };
    /** Result type. Present in all MempoolAcceptResults. */
    const ResultType m_result_type;

    MempoolAcceptResult(ResultType result) : m_result_type(result) {}
    // ...
};

class ChainstateManager {
public:
    ChainstateManager() {}

    MempoolAcceptResult ProcessTransaction(const CTransactionRef& tx, bool test_accept=false) {
        // Placeholder implementation
        return MempoolAcceptResult(MempoolAcceptResult::ResultType::VALID);
    }
};

class NodeContext {
    ChainstateManager chainman;
public:
    NodeContext() : chainman(ChainstateManager()) {}

    ChainstateManager& EnsureChainman()
    {
        return chainman;
    }
};

