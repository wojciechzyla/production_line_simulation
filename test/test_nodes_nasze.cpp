//
// Created by Wojciech Żyła on 25/12/2020.
//
#include "gtest/gtest.h"
#include "nodes.hpp"
#include "package.hpp"
#include "storage_types.hpp"

TEST(NodesTest, StorehouseDefaultQueue){
    Package::reset_IDs();
    Storehouse sh1(1);
    sh1.receive_package(Package());
    sh1.receive_package(Package());
    sh1.receive_package(Package());
    EXPECT_EQ(sh1.get_id(), 1);

    std::vector<ElementID> packages_in_sh1;
    std::vector<ElementID> expected_packages_in_sh1 = {1,2,3};
    for(auto it=sh1.begin(); it!=sh1.end(); it++){
        packages_in_sh1.push_back(it->get_id());
    }
    EXPECT_EQ(packages_in_sh1, expected_packages_in_sh1);
    Package::reset_IDs();
}

TEST(NodesTest, StorehouseCustomQueue){
    Package::reset_IDs();
    Storehouse sh1(2, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::LIFO)));
    sh1.receive_package(Package());
    sh1.receive_package(Package());
    sh1.receive_package(Package());
    EXPECT_EQ(sh1.get_id(), 2);

    std::vector<ElementID> packages_in_sh1;
    std::vector<ElementID> expected_packages_in_sh1 = {1,2,3};
    for(auto it=sh1.begin(); it!=sh1.end(); it++){
        packages_in_sh1.push_back(it->get_id());
    }
    EXPECT_EQ(packages_in_sh1, expected_packages_in_sh1);
    Package::reset_IDs();
}

double probability_65(){
    return 0.65;}
double probability_25(){
    return 0.25;}
double probability_100(){
    return 1.0;}
double probability_0(){
    return 0.0;}

TEST(NodeTest, ReceiverPreferences){
    Package::reset_IDs();
    //Create instances of IPackageReceiver
    Storehouse sh1(1);
    Storehouse sh2(2);
    Storehouse sh3(3);
    Storehouse sh4(4);
    Storehouse sh5(5);

    // Create ReceiverPreference and add receivers
    ReceiverPreferences rec1(probability_0);
    ReceiverPreferences rec2(probability_25);
    ReceiverPreferences rec3(probability_65);
    ReceiverPreferences rec4(probability_100);
    rec1.add_receiver(&sh1);
    rec1.add_receiver(&sh2);
    rec1.add_receiver(&sh3);
    rec1.add_receiver(&sh4);
    rec1.add_receiver(&sh5);

    rec2.add_receiver(&sh1);
    rec2.add_receiver(&sh2);
    rec2.add_receiver(&sh3);
    rec2.add_receiver(&sh4);
    rec2.add_receiver(&sh5);

    rec3.add_receiver(&sh1);
    rec3.add_receiver(&sh2);
    rec3.add_receiver(&sh3);
    rec3.add_receiver(&sh4);
    rec3.add_receiver(&sh5);

    rec4.add_receiver(&sh1);
    rec4.add_receiver(&sh2);
    rec4.add_receiver(&sh3);
    rec4.add_receiver(&sh4);
    rec4.add_receiver(&sh5);

    IPackageReceiver* received1 = rec1.choose_receiver();
    IPackageReceiver* received2 = rec2.choose_receiver();
    IPackageReceiver* received3 = rec3.choose_receiver();
    IPackageReceiver* received4 = rec4.choose_receiver();

    ReceiverPreferences::preferences_t expexted_pref;
    expexted_pref.emplace(std::make_pair(&sh1,0.2));
    expexted_pref.emplace(std::make_pair(&sh2,0.2));
    expexted_pref.emplace(std::make_pair(&sh3,0.2));
    expexted_pref.emplace(std::make_pair(&sh4,0.2));
    expexted_pref.emplace(std::make_pair(&sh5,0.2));

    EXPECT_EQ(received1, (*expexted_pref.begin()).first);
    EXPECT_EQ(rec1.get_preferences(), expexted_pref);
    auto it = expexted_pref.begin();
    it++;
    EXPECT_EQ(received2, (*it).first);
    EXPECT_EQ(rec2.get_preferences(), expexted_pref);
    it = expexted_pref.begin();
    for(int i=0;i!=3;i++) it++;
    EXPECT_EQ(received3, (*it).first);
    EXPECT_EQ(rec3.get_preferences(), expexted_pref);
    it = expexted_pref.end();
    it--;
    EXPECT_EQ(received4, (*it).first);
    EXPECT_EQ(rec4.get_preferences(), expexted_pref);

    // Delete one receiver
    rec1.remove_receiver(&sh1);
    rec2.remove_receiver(&sh1);
    rec3.remove_receiver(&sh1);
    rec4.remove_receiver(&sh1);

    // Remove receiver that doesn't exist
    Storehouse sh6(6);
    rec1.remove_receiver(&sh6);

    received1 = rec1.choose_receiver();
    received2 = rec2.choose_receiver();
    received3 = rec3.choose_receiver();
    received4 = rec4.choose_receiver();

    ReceiverPreferences::preferences_t expexted_pref2;
    expexted_pref2.emplace(std::make_pair(&sh2,0.25));
    expexted_pref2.emplace(std::make_pair(&sh3,0.25));
    expexted_pref2.emplace(std::make_pair(&sh4,0.25));
    expexted_pref2.emplace(std::make_pair(&sh5,0.25));

    EXPECT_EQ(received1, (*expexted_pref2.begin()).first);
    EXPECT_EQ(rec1.get_preferences(), expexted_pref2);
    EXPECT_EQ(received2, (*expexted_pref2.begin()).first);
    EXPECT_EQ(rec2.get_preferences(), expexted_pref2);
    it = expexted_pref2.begin();
    for(int i=0;i!=2;i++) it++;
    EXPECT_EQ(received3, (*it).first);
    EXPECT_EQ(rec3.get_preferences(), expexted_pref2);
    it = expexted_pref2.end();
    it--;
    EXPECT_EQ(received4, (*it).first);
    EXPECT_EQ(rec4.get_preferences(), expexted_pref2);
}

TEST(NodeTest, PackageSender){
    Package::reset_IDs();
    Storehouse sh1(1);
    Storehouse sh2(2);
    Storehouse sh3(3);

    Worker wrk1(1, 2, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO)));
    wrk1.receiver_preferences_.add_receiver(&sh1);
    wrk1.receiver_preferences_.add_receiver(&sh2);
    wrk1.receiver_preferences_.add_receiver(&sh3);
    wrk1.receive_package(Package());
    wrk1.receive_package(Package());
    wrk1.do_work(1);
    wrk1.do_work(2);
    wrk1.send_package();
    EXPECT_EQ(wrk1.get_sending_buffer(), std::nullopt);
    wrk1.do_work(3);
    EXPECT_NE(wrk1.get_sending_buffer(), std::nullopt);

    Ramp rmp1(1, 1);
    rmp1.receiver_preferences_.add_receiver(&sh1);
    rmp1.receiver_preferences_.add_receiver(&sh2);
    rmp1.receiver_preferences_.add_receiver(&sh3);
    rmp1.deliver_goods(1);
    rmp1.deliver_goods(2);
    rmp1.send_package();
    EXPECT_EQ(rmp1.get_sending_buffer(), std::nullopt);
    rmp1.send_package();
    EXPECT_EQ(rmp1.get_sending_buffer(), std::nullopt);
}

TEST(NodeTest, Ramp){
    Package::reset_IDs();
    Worker wrk1(1, 2, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO)));
    Worker wrk2(2, 1, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::LIFO)));
    Ramp rmp1(1, 1);
    Ramp rmp2(2, 2);
    EXPECT_EQ(rmp1.get_sending_buffer(), std::nullopt);
    EXPECT_EQ(rmp2.get_sending_buffer(), std::nullopt);
    rmp1.receiver_preferences_.add_receiver(&wrk1);
    rmp1.receiver_preferences_.add_receiver(&wrk2);
    rmp2.receiver_preferences_.add_receiver(&wrk1);
    rmp2.receiver_preferences_.add_receiver(&wrk2);

    //pierwsza tura
    rmp1.deliver_goods(1); //wytworzony produkt ląduje w buforze
    rmp2.deliver_goods(1);
    EXPECT_NE(rmp1.get_sending_buffer(), std::nullopt); //czy bufor jest niepusty? (powinien być)
    EXPECT_NE(rmp2.get_sending_buffer(), std::nullopt);
    rmp1.send_package();  //produkt wysłany do odbiorcy, usunięty z bufora
    rmp2.send_package();
    EXPECT_EQ(rmp1.get_sending_buffer(), std::nullopt); //czy bufor jest pusty? (powinien być)
    EXPECT_EQ(rmp2.get_sending_buffer(), std::nullopt); //czy bufor jest niepusty? (powinien być)

    //druga tura
    rmp1.deliver_goods(2);
    rmp2.deliver_goods(2);
    EXPECT_NE(rmp1.get_sending_buffer(), std::nullopt);
    EXPECT_EQ(rmp2.get_sending_buffer(), std::nullopt);
    rmp1.send_package();
    rmp2.send_package();
    EXPECT_EQ(rmp1.get_sending_buffer(), std::nullopt);
    EXPECT_EQ(rmp2.get_sending_buffer(), std::nullopt);

    //trzecia tura
    rmp1.deliver_goods(3);
    rmp2.deliver_goods(3);
    EXPECT_NE(rmp1.get_sending_buffer(), std::nullopt);
    EXPECT_NE(rmp2.get_sending_buffer(), std::nullopt);
    rmp1.send_package();
    rmp2.send_package();
    EXPECT_EQ(rmp1.get_sending_buffer(), std::nullopt);
    EXPECT_EQ(rmp2.get_sending_buffer(), std::nullopt);
}

TEST(NodeTest, Worker){
    Package::reset_IDs();
    Storehouse sh1(1);
    Worker wrk1(1, 2, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO)));
    Worker wrk2(2, 1, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::LIFO)));
    wrk1.receiver_preferences_.add_receiver(&sh1);
    wrk2.receiver_preferences_.add_receiver(&sh1);

    wrk1.receive_package(Package()); //id = 1
    wrk1.receive_package(Package()); //id = 2
    wrk1.receive_package(Package()); //id = 3

    wrk2.receive_package(Package()); //id = 4
    wrk2.receive_package(Package()); //id = 5
    wrk2.receive_package(Package()); //id = 6
    wrk2.receive_package(Package()); //id = 7

    //'usuwanie' paczki pierwszej: dla wrk1(FIFO) to paczka z id = 1, dla wrk2(LIFO) paczka z id = 4

    std::vector<ElementID> ids_wrk1;
    for(auto i = 1; i <= 6; i++){
        wrk1.send_package();
        wrk1.do_work(i);
        if(wrk1.get_sending_buffer().has_value()){
            ids_wrk1.push_back(wrk1.get_sending_buffer()->get_id());
        }
    }

    std::vector<ElementID> ids_wrk2;
    for(auto i = 1; i <= 4; i++){
        wrk2.send_package();
        wrk2.do_work(i); //długość pracy = 1 tura
        ids_wrk2.push_back(wrk2.get_sending_buffer()->get_id());
    }

    std::vector<ElementID> exp_wrk1 = {1,2,3};
    std::vector<ElementID> exp_wrk2 = {7,6,5,4};
    EXPECT_EQ(ids_wrk1, exp_wrk1);
    EXPECT_EQ(ids_wrk2, exp_wrk2);
}

TEST(NodeTest, WorkerWorkTime){
    Package::reset_IDs();
    //czy worker pracuje nad paczką odpowiednia ilośc tur?
    Storehouse sh1(1);
    Worker wrk1(1, 3, std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO)));
    wrk1.receiver_preferences_.add_receiver(&sh1);

    wrk1.receive_package(Package());
    wrk1.receive_package(Package());
    wrk1.receive_package(Package());
    wrk1.receive_package(Package());
    wrk1.receive_package(Package());
    wrk1.receive_package(Package());

    TimeOffset working_time = 0;
    for(auto i = 1; i <= 10; i++){
        working_time++;
        wrk1.send_package();
        wrk1.do_work(i);
        if(wrk1.get_sending_buffer()->get_id() == 1){break;}
    }

    EXPECT_EQ(working_time, wrk1.get_processing_duration());
}