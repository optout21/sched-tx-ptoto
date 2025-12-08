#include "bitcoincore.h"

#include <array>
#include <cassert>
#include <cstring>
#include <string>
#include <vector>

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

namespace util {
    /**
     * Helper function to access the contained object of a std::any instance.
     * Returns a pointer to the object if passed instance has a value and the type
     * matches, nullptr otherwise.
     */
    template<typename T>
    T* AnyPtr(const std::any& any) noexcept
    {
        T* const* ptr = std::any_cast<T*>(&any);
        return ptr ? *ptr : nullptr;
    }
} // namespace util

node::NodeContext& EnsureAnyNodeContext(const std::any& context)
{
    assert(context.has_value());
    auto node_context = util::AnyPtr<node::NodeContext>(context);
    assert(node_context);
    // if (!node_context) {
    //     throw JSONRPCError(RPC_INTERNAL_ERROR, "Node context not found");
    // }
    return *node_context;
}

// ChainstateManager& EnsureChainman(node::NodeContext& node) {
//     return node.chainman;
// }

typedef std::array<char, 2> ByteAsHex;

constexpr std::array<ByteAsHex, 256> CreateByteToHexMap()
{
    constexpr char hexmap[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

    std::array<ByteAsHex, 256> byte_to_hex{};
    for (size_t i = 0; i < byte_to_hex.size(); ++i) {
        byte_to_hex[i][0] = hexmap[i >> 4];
        byte_to_hex[i][1] = hexmap[i & 15];
    }
    return byte_to_hex;
}

std::string HexStr(const std::vector<uint8_t>& s) {
    // std::string rv(s.size() * 2, '\0');
    // static constexpr auto byte_to_hex = CreateByteToHexMap();
    // static_assert(sizeof(byte_to_hex) == 512);

    // char* it = rv.data();
    // for (uint8_t v : s) {
    //     std::memcpy(it, byte_to_hex[v].data(), 2);
    //     it += 2;
    // }

    // assert(it == rv.data() + rv.size());
    // return rv;
    return std::string("");
}

namespace node {
    TransactionError BroadcastTransaction(NodeContext& node,
                                            CTransactionRef tx,
                                            std::string& err_string,
                                            const CAmount& max_tx_fee,
                                            TxBroadcast broadcast_method,
                                            bool wait_callback) {
        // Placeholder implementation
        return TransactionError::TXERR_OK;
    }
}