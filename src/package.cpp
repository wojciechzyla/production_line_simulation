//
// Created by Wojciech Żyła on 15/12/2020.
//
#include "package.hpp"
#include <iostream>

Package::Package() {
    if (!Package::freed_IDs_.empty()){
        ID_ = *freed_IDs_.begin();

        assigned_IDs_.insert(*freed_IDs_.begin());
        freed_IDs_.begin() = freed_IDs_.erase(freed_IDs_.begin());
    }
    else if (Package::assigned_IDs_.empty()){
        ID_= 1;

        assigned_IDs_.insert(ID_);
    }
    else {
        auto it = assigned_IDs_.end();
        it--;
        ID_ = *it;
        ID_++;
        assigned_IDs_.insert(ID_);
    }
}


void Package::reset_IDs() {
    Package::freed_IDs_.clear();
    Package::assigned_IDs_.clear();
}

std::set<ElementID> Package::assigned_IDs_;
std::set<ElementID> Package::freed_IDs_;