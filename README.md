# Scheduled Transaction Broadcast

## Overview

In certain cases the user creating a transaction does not want to broadcast it to the bitcoin network right away, but later in the future. This scheduling can be done on the client side, but sometimes it would be practical if this handled by the node itself.

With Scheduled Transaction Broadcast the bitcoin node accepts a transaction for later broadcast, stores it, and broadcasts automatically at the specified time.


## Details

Uses. Delayed transaction broadcast can be useful for/when:
- hiding the real time of transaction creation (with random delay; for non-urgent transactions)
- there is some precondition that is expected to occur in the future (e.g. occurance of a transaction), which condition is not possible or desired to be expressed in bitcoin scripts.

Result, error check. As the actual broadcast happens later, the broadcast result -- success or some error -- is not available at acceptance time. Only parsing checks are done at acceptance time. The client can check the result only by monitoring the bitcoin network. Note: some checks are not possible to be done at acceptance time, as conditions may change (e.g. inputs with timelocks). (Note: it could be possible to offer an API to query for broadcast result, but that's not planned.)

Absolute time vs. block time. The target time can be specified as an absolute time or a block height.

No guarantees. Delayed broadcast is best-effort, there is no delivery guarantee.

Retries. It is possible to sepcify a number of retries, and a retry period. Then the scehduled broadcast is retried accordingly in case of failure.


## Implementation Details

Scheduled transactions are kept in a separate memory structure, and are persisted to a separate file (for persistence accress restarts). The file is read at startup, and written whenever there is a relevant change to its content, and at shutdown.

A new RPC 'schedulerawtransaction' is introduced, with syntax very similar to 'sendrawtransaction'. However, the transaction is not sent rigth away, but stored for later broadcast.


## Testing

curl --user <user>:<pass> --data-binary '{"jsonrpc": "1.0", "id": "curltest", "method": "schedulerawtransaction", "params": ["signedhex"]}' -H 'content-type: text/plain;' http://localhost:8332/

