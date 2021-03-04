//
// Created by Wojciech Żyła on 20/12/2020.
//

#ifndef NET_SIMULATION_STORAGE_TYPES_HPP
#define NET_SIMULATION_STORAGE_TYPES_HPP

#include <list>
#include "types.hpp"
#include "package.hpp"
#include <iostream>

enum class PackageQueueType {
    LIFO, FIFO
};


class IPackageStockpile{
public:
    using const_iterator = std::list<Package>::const_iterator;
    virtual void push(Package&&) = 0;
    virtual int size() = 0;
    virtual bool empty() = 0;
    virtual const_iterator begin() const = 0;
    virtual const_iterator end() const = 0;
    virtual const_iterator cbegin() const = 0;
    virtual const_iterator cend() const = 0;
    virtual ~IPackageStockpile() {};
};


class IPackageQueue : public IPackageStockpile{
public:
    virtual Package pop() = 0;
    virtual PackageQueueType get_queue_type() = 0;
    virtual ~IPackageQueue() {};
};

class PackageQueue : public IPackageQueue{
public:
    PackageQueue(PackageQueueType type): queue_type_(type){}
    const_iterator begin() const override {return products_.cbegin();}
    const_iterator end() const override {return products_.cend();}
    const_iterator cbegin() const override {return products_.cbegin();}
    const_iterator cend() const override {return products_.cend();}
    bool empty() override {return products_.empty();}
    int size() override {return products_.size();}
    void push(Package&& item) override {products_.emplace_back(std::move(item));}
    Package pop() override;
    PackageQueueType get_queue_type() override {return queue_type_;}
private:
    std::list<Package> products_;
    PackageQueueType queue_type_;
};
#endif //NET_SIMULATION_STORAGE_TYPES_HPP
