//
// Created by Wojciech Żyła on 31/12/2020.
//
#include "gtest/gtest.h"
#include "factory.hpp"
#include "nodes.hpp"
#include "types.hpp"

TEST(FactoryTest, RemoveReceiver){
    Factory f1;
    f1.add_worker(Worker(1,3, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))));
    f1.add_worker(Worker(2,3, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))));
    f1.add_worker(Worker(3,3, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))));
    f1.add_ramp(Ramp(1,2));
    f1.add_ramp(Ramp(2,2));
    f1.add_storehouse(Storehouse(1));
    f1.add_storehouse(Storehouse(2));
    f1.add_storehouse(Storehouse(3));
    auto w1 = f1.find_worker_by_id(1);
    auto w2 = f1.find_worker_by_id(2);
    auto w3 = f1.find_worker_by_id(3);
    auto r1 = f1.find_ramp_by_id(1);
    auto s1 = f1.find_storehouse_by_id(1);
    auto s2 = f1.find_storehouse_by_id(2);
    auto s3 = f1.find_storehouse_by_id(3);

    w1->receiver_preferences_.add_receiver(&*w2);
    w1->receiver_preferences_.add_receiver(&*s1);

    r1->receiver_preferences_.add_receiver(&*w2);
    r1->receiver_preferences_.add_receiver(&*w1);
    r1->receiver_preferences_.add_receiver(&*w3);
    r1->receiver_preferences_.add_receiver(&*s2);
    r1->receiver_preferences_.add_receiver(&*s3);

    ReceiverPreferences::preferences_t expected_receivers_w1;
    expected_receivers_w1.emplace(std::make_pair(&*w2, 0.5));
    expected_receivers_w1.emplace(std::make_pair(&*s1, 0.5));

    ReceiverPreferences::preferences_t expected_receivers_r1;
    expected_receivers_r1.emplace(std::make_pair(&*w2, 0.2));
    expected_receivers_r1.emplace(std::make_pair(&*w1, 0.2));
    expected_receivers_r1.emplace(std::make_pair(&*w3, 0.2));
    expected_receivers_r1.emplace(std::make_pair(&*s2, 0.2));
    expected_receivers_r1.emplace(std::make_pair(&*s3, 0.2));


    EXPECT_EQ(w1->receiver_preferences_.get_preferences(), expected_receivers_w1);
    EXPECT_EQ(r1->receiver_preferences_.get_preferences(), expected_receivers_r1);

    f1.remove_worker(2);

    expected_receivers_w1.clear();
    expected_receivers_r1.clear();

    expected_receivers_w1.emplace(std::make_pair(&*s1, 1.0));

    expected_receivers_r1.emplace(std::make_pair(&*w1, 0.25));
    expected_receivers_r1.emplace(std::make_pair(&*w3, 0.25));
    expected_receivers_r1.emplace(std::make_pair(&*s2, 0.25));
    expected_receivers_r1.emplace(std::make_pair(&*s3, 0.25));

    EXPECT_EQ(w1->receiver_preferences_.get_preferences(), expected_receivers_w1);
    EXPECT_EQ(r1->receiver_preferences_.get_preferences(), expected_receivers_r1);

    f1.remove_storehouse(2);
    f1.remove_worker(1);

    expected_receivers_r1.clear();

    expected_receivers_r1.emplace(std::make_pair(&*w3, 0.5));
    expected_receivers_r1.emplace(std::make_pair(&*s3, 0.5));

    EXPECT_EQ(r1->receiver_preferences_.get_preferences(), expected_receivers_r1);

    f1.remove_ramp(1);
    f1.remove_worker(3);

    EXPECT_TRUE(f1.ramp_cbegin()->get_id() == 2);
    EXPECT_TRUE(f1.worker_cbegin() == f1.worker_cend());
}

TEST(FactoryTest, IsConsistent){
    Factory f1;
    f1.add_worker(Worker(1,3, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))));
    f1.add_worker(Worker(2,3, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))));
    f1.add_worker(Worker(3,3, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))));
    f1.add_ramp(Ramp(1,2));
    f1.add_ramp(Ramp(2,2));
    f1.add_storehouse(Storehouse(1));
    f1.add_storehouse(Storehouse(2));

    auto w1 = f1.find_worker_by_id(1);
    auto w2 = f1.find_worker_by_id(2);
    auto w3 = f1.find_worker_by_id(3);
    auto r1 = f1.find_ramp_by_id(1);
    auto r2 = f1.find_ramp_by_id(2);
    auto s1 = f1.find_storehouse_by_id(1);
    auto s2 = f1.find_storehouse_by_id(2);

    r1->receiver_preferences_.add_receiver(&*w1);
    r1->receiver_preferences_.add_receiver(&*w2);

    w1->receiver_preferences_.add_receiver(&*s1);

    w2->receiver_preferences_.add_receiver(&*w1);

    r2->receiver_preferences_.add_receiver(&*w2);
    r2->receiver_preferences_.add_receiver(&*w3);

    w3->receiver_preferences_.add_receiver(&*s2);

    EXPECT_TRUE(f1.is_consistent());

    w3->receiver_preferences_.remove_receiver(&*s2);

    r2->receiver_preferences_.remove_receiver(&*w2);

    EXPECT_FALSE(f1.is_consistent());
}

TEST(FactoryTest, PackagePassing){
    Package::reset_IDs();
    Factory f1;
    f1.add_worker(Worker(1,1, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))));
    f1.add_worker(Worker(2,2, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))));
    f1.add_ramp(Ramp(1,1));
    f1.add_ramp(Ramp(2,1));
    f1.add_storehouse(Storehouse(1));

    auto w1 = f1.find_worker_by_id(1);
    auto w2 = f1.find_worker_by_id(2);
    auto r1 = f1.find_ramp_by_id(1);
    auto r2 = f1.find_ramp_by_id(2);
    auto s1 = f1.find_storehouse_by_id(1);

    r1->receiver_preferences_.add_receiver(&*w1);
    r2->receiver_preferences_.add_receiver(&*w2);
    w1->receiver_preferences_.add_receiver(&*s1);
    w2->receiver_preferences_.add_receiver(&*s1);

    for(int i=0; i<4;i++){
        f1.do_deliveries(i);
        f1.do_package_passing();
        f1.do_work(i);
    }

    std::vector<ElementID> expected_ids = {1,3,2};
    std::vector<ElementID> actual_ids;

    for(auto it=s1->begin();it!=s1->end();it++){
        actual_ids.push_back(it->get_id());
    }

    EXPECT_EQ(expected_ids,actual_ids);
}