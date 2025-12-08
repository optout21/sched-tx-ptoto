/**
 * Collection of things needed from (rest of) the bitcoin core.
 */

#pragma once

#include <any>
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

// class ChainstateManager {
// public:
//     ChainstateManager() {}

//     MempoolAcceptResult ProcessTransaction(const CTransactionRef& tx, bool test_accept=false) {
//         // Placeholder implementation
//         return MempoolAcceptResult(MempoolAcceptResult::ResultType::VALID);
//     }
// };

namespace node {
    class NodeContext {
    public:
        // ChainstateManager chainman;
        NodeContext() {}
        // NodeContext() : chainman(ChainstateManager()) {}

        // ChainstateManager& EnsureChainman(const node::NodeContext& context)
        // {
        //     return chainman;
        // }
    };
}

node::NodeContext& EnsureAnyNodeContext(const std::any& context);

// ChainstateManager& EnsureChainman(node::NodeContext& context);

std::string HexStr(const std::vector<uint8_t>& s);

typedef uint64_t CAmount;

namespace node {
    /**
    * How to broadcast a local transaction.
    * Used to influence `BroadcastTransaction()` and its callers.
    */
    enum class TxBroadcast : uint8_t {
        /// Add the transaction to the mempool and broadcast to all peers for which tx relay is enabled.
        MEMPOOL_AND_BROADCAST_TO_ALL,
        /// Add the transaction to the mempool, but don't broadcast to anybody.
        MEMPOOL_NO_BROADCAST,
    };

    enum TransactionError : uint8_t {
        TXERR_OK
    };

    TransactionError BroadcastTransaction(NodeContext& node,
                                            CTransactionRef tx,
                                            std::string& err_string,
                                            const CAmount& max_tx_fee,
                                            TxBroadcast broadcast_method,
                                            bool wait_callback);
}
