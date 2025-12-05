#include "schedtx.h"

#include <cassert>
#include <chrono>
#include <iostream>
#include <thread>
#include <sstream>


void add_sample_txs(ScheduledTxPool& pool) {
    auto n = 20;
    printf("Adding %d txs to the pool '%s' ... ", n, pool.ToString().c_str());
    auto now{time(NULL)};
    for (auto i{0}; i < n; ++i) {
        auto rem3 = i % 3;
        auto rem7 = i % 7;
        auto to_wait = (9 + 3*i + 73*rem3*rem3*rem3 + 37*rem7*rem7) / 30;
        std::vector<uint8_t> dummy_tx;
        dummy_tx.resize(30 + i + rem3 * 7 + rem7 * 3);
        auto _txid = pool.Add(now + i + to_wait, dummy_tx);
    }
    printf("'%s' \n", pool.ToString().c_str());
}

/*
void test_earliest_and_remove() {
    ScheduledTxCollection p;
    add_sample_txs(p);

    auto now{time(NULL)};
    auto current_time{now};
    while (p.Count() > 0) {
        if (p.ProcessOne(current_time)) {
            // A Tx was processed
            printf("%ld:                                        '%s'\n", current_time - now, p.ToString().c_str());
            continue;
        }

        auto next = p.GetEarliest();
        if (!next.has_value()) {
            break;
        }
        auto time = std::get<0>(next.value());
        auto idx = std::get<1>(next.value());
        printf("%ld: Next is idx %ld at %ld (%d) ...\n", current_time - now, idx, time - now, time);
        // Now we wait until that time
        current_time = time;
    }
}
*/

void do_basic_tests() {
    ScheduledTx tx1;
    printf("ScheduledTx class created, %s \n", tx1.ToString().c_str());
    assert(tx1.ToString().compare("0 0 0 (1 x 3600)") == 0);

    auto now{time(NULL)};
    auto target{now + 86400};
    std::vector<uint8_t> dummy_tx = {1, 2, 3};
    ScheduledTx tx2(now, target, dummy_tx);
    printf("ScheduledTx class created, %s \n", tx2.ToString().c_str());
    assert(tx2.size() == 3);
    assert(tx2.max_retries == 1);
    assert(tx2.retry_period == 3600);

    // Test serialization
    std::ostringstream oss;
    tx2.Serialize(oss);
    // printf("Serialized ScheduledTx to stream, stream size %ld \n", oss.str().size());

    // Test deserialization
    std::istringstream iss(oss.str());
    ScheduledTx tx3 = ScheduledTx::Deserialize(iss);
    printf("Deserialized ScheduledTx from stream, %s \n", tx3.ToString().c_str());

    // Verify the deserialized object matches the original
    assert(tx3.submitted_time == tx2.submitted_time);
    assert(tx3.target_time == tx2.target_time);
    assert(tx3.max_retries == tx2.max_retries);
    assert(tx3.retry_period == tx2.retry_period);
    assert(tx3.retry_count == tx2.retry_count);
    assert(tx3.last_try_time == tx2.last_try_time);
    assert(tx3.tx.data() == tx2.tx.data());
    assert(tx3.ToString().compare(tx2.ToString()) == 0);
    printf("Serialization/deserialization test passed!\n");

    // Serialize more
    std::vector<uint8_t> dummy2_tx = {1, 2, 3, 4, 5, 6, 7};
    ScheduledTx tx4(now, target + 3600, dummy2_tx);
    std::ostringstream oss2;
    tx2.Serialize(oss2);
    printf("Serialized 1st object to stream, stream size %ld \n", oss2.str().size());
    tx4.Serialize(oss2);
    printf("Serialized 2nd object to stream, stream size %ld \n", oss2.str().size());

    // Deserialize
    std::istringstream iss2(oss2.str());
    ScheduledTx tx5 = ScheduledTx::Deserialize(iss2);
    ScheduledTx tx6 = ScheduledTx::Deserialize(iss2);
    printf("Deserialized 2 objects from stream, %s %s \n", tx5.ToString().c_str(), tx6.ToString().c_str());
    assert(tx5.ToString().compare(tx2.ToString()) == 0);
    assert(tx6.ToString().compare(tx4.ToString()) == 0);

    ScheduledTxCollection pool1;
    printf("ScheduledTxCollection class created, %s \n", pool1.ToString().c_str());

    ScheduledTxCollection pool2;
    pool2.AddInternal(tx6);
    pool2.AddInternal(tx5);
    printf("ScheduledTxCollection class created, %s \n", pool2.ToString().c_str());

    // Serialize
    std::ostringstream oss3;
    pool2.Serialize(oss3);
    printf("Serialized pool to stream, stream size %ld \n", oss2.str().size());

    // Deserialize
    std::istringstream iss3(oss3.str());
    ScheduledTxCollection pool3 = ScheduledTxCollection::Deserialize(iss3);
    printf("Deserialized ScheduledTxCollection from stream, '%s' \n", pool3.ToString().c_str());
}

void do_tests() {
    do_basic_tests();

    // test_earliest_and_remove();

    NodeContext nc;
    ScheduledTxPool p(nc);
    add_sample_txs(p);
    p.Start();
    // wait until empty
    while (p.Count() > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printf(".");
    }
    printf("Empty, exiting\n");
    p.Stop();
}

int main() {
    printf("Hello from schedtx!\n");

    // do_tests();

    auto filename{"schedtx.dat"};
    NodeContext nc;
    ScheduledTxPool pool(nc);
    pool.CreateFromFile(filename);
    pool.Start();
    printf("Pool created: '%s' (filename '%s')\n", pool.ToString().c_str(), filename);

    // Add a transaction
    std::this_thread::sleep_for(std::chrono::seconds(1));
    std::vector<uint8_t> dummy_tx = {1, 2, 3};
    auto now{time(NULL)};
    auto to_wait = 10;
    printf("Scheduling a tx, in %d secs ...\n", to_wait);
    pool.Add(now + to_wait, dummy_tx);

    // wait until empty
    while (pool.Count() > 0) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        printf(".");
        // printf("%ld \n", pool.Count());
    }
    printf("Empty, exiting ...\n");
    pool.Stop();
    printf("\n");

    return 0;
}
