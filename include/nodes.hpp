//
// Created by Wojciech Żyła on 25/12/2020.
//

#ifndef NET_SIMULATION_NODES_HPP
#define NET_SIMULATION_NODES_HPP
#include "types.hpp"
#include "helpers.hpp"
#include "storage_types.hpp"
#include <memory>
#include <map>
#include <utility>

enum class ReceiverType{
    WORKER,STOREHOUSE
};

class IPackageReceiver{
public:
    virtual void receive_package(Package&& p) = 0;
    virtual ElementID get_id() const = 0;
    virtual ReceiverType get_receiver_type() const = 0;
    virtual IPackageStockpile::const_iterator begin() const = 0;
    virtual IPackageStockpile::const_iterator end() const = 0;
    virtual IPackageStockpile::const_iterator cbegin() const = 0;
    virtual IPackageStockpile::const_iterator cend() const = 0;
    virtual ~IPackageReceiver() {};
};

class Storehouse : public IPackageReceiver{
public:
    Storehouse(ElementID id, std::unique_ptr<IPackageStockpile> d = std::make_unique<PackageQueue>(PackageQueue(PackageQueueType::FIFO))):
    id_(id), d_(std::move(d)) {};
    IPackageStockpile::const_iterator begin() const override {return (*d_).cbegin();}
    IPackageStockpile::const_iterator end() const override {return (*d_).cend();}
    IPackageStockpile::const_iterator cbegin() const override {return (*d_).cbegin();}
    IPackageStockpile::const_iterator cend() const override {return (*d_).cend();}
    ReceiverType get_receiver_type() const override {return ReceiverType::STOREHOUSE;}

    ElementID get_id() const override {return id_;}
    void receive_package(Package&& p) override {(*d_).push(std::move(p));}
private:
    ElementID id_;
    std::unique_ptr<IPackageStockpile> d_;
};

class ReceiverPreferences{
public:
    ReceiverPreferences(ProbabilityGenerator pg = probability_generator): pg_(std::move(pg)) {};
    using preferences_t = std::map<IPackageReceiver*, double>;
    using const_iterator = preferences_t::const_iterator;
    const_iterator begin() const {return preferences_t_.cbegin();}
    const_iterator end() const {return preferences_t_.cend();}
    const_iterator cbegin() const {return preferences_t_.cbegin();}
    const_iterator cend() const {return preferences_t_.cend();}
    void add_receiver(IPackageReceiver* r);
    void remove_receiver(IPackageReceiver* r);
    IPackageReceiver* choose_receiver();
    const preferences_t& get_preferences() const {return preferences_t_;};
private:
    ProbabilityGenerator pg_;
    preferences_t preferences_t_;
};

class PackageSender{
public:
    ReceiverPreferences receiver_preferences_;
    PackageSender() = default;
    PackageSender(PackageSender&&) = default;

    void send_package(){
        if(buffer_.has_value()){
            IPackageReceiver* receiver = receiver_preferences_.choose_receiver();
            receiver -> receive_package(reinterpret_cast<Package&&>(*get_sending_buffer()));
            buffer_.reset();
        }
    }
    std::optional<Package>& get_sending_buffer() {return buffer_;}
    const std::optional<Package>& get_sending_buffer() const {return buffer_;}
protected:
    void push_package(Package&& p){
        buffer_.emplace(std::move(p));
    }
private:
    std::optional<Package> buffer_;
};

class Ramp : public PackageSender{
public:
    Ramp(ElementID id, TimeOffset di) : id_(id), di_(di) {};
    void deliver_goods (Time t);
    TimeOffset get_delivery_interval() const {return di_;}
    ElementID get_id() const {return id_;}
private:
    Time previous_time_;
    ElementID id_;
    TimeOffset di_;
};

class Worker : public PackageSender, public IPackageReceiver{
public:
    Worker(ElementID id, TimeOffset pd, std::unique_ptr<IPackageQueue> qe):
            id_(id), pd_(pd), qe_(std::move(qe)){};
    void do_work(Time t);
    TimeOffset get_processing_duration() const {return pd_;}
    Time get_package_processing_time() const {return current_time_-previous_time_+1;}
    IPackageStockpile::const_iterator begin() const override {return (*qe_).cbegin();}
    IPackageStockpile::const_iterator end() const override {return (*qe_).cend();}
    IPackageStockpile::const_iterator cbegin() const override {return (*qe_).cbegin();}
    IPackageStockpile::const_iterator cend() const override {return (*qe_).cend();}
    void receive_package(Package&& p) override {(*qe_).push(std::move(p));}
    ElementID get_id() const override {return id_;}
    ReceiverType get_receiver_type() const override {return ReceiverType::WORKER;}
    IPackageQueue* get_queue() const {return &*qe_;}
    const std::optional<Package>& get_processing_buffer() const {return processing_buffer_;}
private:
    ElementID id_;
    TimeOffset pd_;
    Time previous_time_;
    Time current_time_;
    std::optional<Package> processing_buffer_;
    std::unique_ptr<IPackageQueue> qe_;
};
#endif //NET_SIMULATION_NODES_HPP
