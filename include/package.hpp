//
// Created by Wojciech Żyła on 15/12/2020.
//

#ifndef NET_SIMULATION_PACKAGE_HPP
#define NET_SIMULATION_PACKAGE_HPP
#include "types.hpp"
#include <set>
#include "types.hpp"
#include <set>
#include <iostream>

class Package{
public:
    Package();
    Package(const Package&) = delete;
    Package(Package&& p) {
            ID_ = p.ID_;
            p.ID_ = 0;
    }
    Package& operator = (Package&& p){
        if(this != &p) {
            this->ID_ = p.ID_;
            p.ID_ = 0;
        }
        return *this;
    };
    Package(ElementID id):ID_(id) {};

    //static ElementID initialize();
    ElementID get_id() const {return ID_;};

    // Function used to reset assigned_IDs_ abd freed_IDs_ during tests.
    static void reset_IDs();
    ~Package() {if(ID_ != 0)assigned_IDs_.erase(ID_); if(ID_ != 0)freed_IDs_.insert(ID_);}
private:
    ElementID ID_;
    static std::set<ElementID> assigned_IDs_;
    static std::set<ElementID> freed_IDs_;
};
#endif //NET_SIMULATION_PACKAGE_HPP
