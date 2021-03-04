//
// Created by Wojciech Żyła on 20/12/2020.
//
#include "gtest/gtest.h"
#include "storage_types.hpp"
#include "package.hpp"
#include "types.hpp"
#include "helpers.hpp"
#include <vector>

TEST(PackageQueueTest, FIFO){
    Package::reset_IDs();
    PackageQueue fifo_storage(PackageQueueType::FIFO);
    ASSERT_TRUE(fifo_storage.empty());
    ASSERT_EQ(fifo_storage.get_queue_type(), PackageQueueType::FIFO);

    fifo_storage.push(Package());
    fifo_storage.push(Package());
    fifo_storage.push(Package());
    fifo_storage.push(Package());
    std::vector<ElementID> id_expected = {1,2,3,4};
    std::vector<ElementID> id_in_storage;

    for(auto it = fifo_storage.begin(); it != fifo_storage.end(); it++){
        id_in_storage.push_back(it->get_id());
    }
    ASSERT_EQ(id_in_storage,id_expected);
    ASSERT_FALSE(fifo_storage.empty());


    ASSERT_EQ(fifo_storage.size(),4);
    Package poped = fifo_storage.pop();
    ASSERT_EQ(poped.get_id(),1);
    ASSERT_EQ(fifo_storage.size(),3);
    Package::reset_IDs();
}

TEST(PackageQueueTest, LIFO){
    Package::reset_IDs();
    PackageQueue lifo_storage(PackageQueueType::LIFO);
    ASSERT_TRUE(lifo_storage.empty());
    ASSERT_EQ(lifo_storage.get_queue_type(), PackageQueueType::LIFO);

    lifo_storage.push(Package());
    lifo_storage.push(Package());
    lifo_storage.push(Package());
    lifo_storage.push(Package());
    std::vector<ElementID> id_expected = {1,2,3,4};
    std::vector<ElementID> id_in_storage;

    for(auto it = lifo_storage.begin(); it != lifo_storage.end(); it++){
        id_in_storage.push_back(it->get_id());
    }
    ASSERT_EQ(id_in_storage,id_expected);
    ASSERT_FALSE(lifo_storage.empty());

    ASSERT_EQ(lifo_storage.size(),4);
    Package poped = lifo_storage.pop();
    ASSERT_EQ(poped.get_id(),4);
    ASSERT_EQ(lifo_storage.size(),3);
    Package::reset_IDs();
}

TEST(PackageQueueTypeTest, PackageQueueTypeToString){
    ASSERT_EQ(queue_type_to_string(PackageQueueType::LIFO), "LIFO");
    ASSERT_EQ(queue_type_to_string(PackageQueueType::FIFO), "FIFO");
}
