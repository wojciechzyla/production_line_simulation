//
// Created by Wojciech Żyła on 15/12/2020.
//
#include "gtest/gtest.h"
#include "package.hpp"
#include "types.hpp"

TEST(PackageTest, UnqiueID){
    Package *p1 = new Package;
    Package *p2 = new Package;
    Package *p3 = new Package;
    Package *p4 = new Package;

    std::vector<ElementID> result;
    result.push_back(p1->get_id());
    result.push_back(p2->get_id());
    result.push_back(p3->get_id());
    result.push_back(p4->get_id());

    std::vector<ElementID> expected = {1,2,3,4};

    EXPECT_EQ(result, expected);
}

TEST(PackageTest, GetIDFromFreed){
    Package::reset_IDs();

    Package *p1 = new Package;
    Package *p2 = new Package;
    Package *p3 = new Package;
    Package *p4 = new Package;
    Package *p5 = new Package;
    Package *p6 = new Package;

    delete p5;
    delete p2;

    Package *p7 = new Package;
    Package *p8 = new Package;

    std::vector<ElementID> result;
    result.push_back(p1->get_id());
    result.push_back(p3->get_id());
    result.push_back(p4->get_id());
    result.push_back(p6->get_id());
    result.push_back(p7->get_id());
    result.push_back(p8->get_id());

    std::vector<ElementID> expected = {1,3,4,6,2,5};

    EXPECT_EQ(result, expected);
    Package::reset_IDs();
}