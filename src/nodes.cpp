//
// Created by Wojciech Żyła on 25/12/2020.
//
#include "nodes.hpp"
#include <algorithm>

void ReceiverPreferences::add_receiver(IPackageReceiver *r) {
    double receivers_number = preferences_t_.size();
    if (receivers_number > 0){
        double new_probability = 1/(receivers_number+1);
        for (auto &p: preferences_t_){
            p.second = new_probability;
        }
        preferences_t_.emplace(std::make_pair(r, new_probability));
    } else{
        preferences_t_.emplace(std::make_pair(r,1.0));
    }
}

void ReceiverPreferences::remove_receiver(IPackageReceiver *r) {
    if( preferences_t_.find(r) != preferences_t_.cend()){
        double receivers_number = preferences_t_.size();
        double new_probability = 1/(receivers_number - 1);
        for (auto &p: preferences_t_){
            p.second = new_probability;
        }
        preferences_t_.erase(r);
    }
}

IPackageReceiver * ReceiverPreferences::choose_receiver() {
    double probability = pg_();
    if(probability < 0.0 || probability > 1) throw std::logic_error("Probability out of range [0,1]");
    double previous_prob = 0.0;
    IPackageReceiver *result = preferences_t_.cbegin()->first;
    if (!(probability >= previous_prob && probability < previous_prob + preferences_t_.cbegin()->second)){
        for(auto it = preferences_t_.cbegin(); it != preferences_t_.cend(); it++){
            if(probability > previous_prob && probability <= previous_prob + it->second){
                result = it->first;
                break;
            }
            else{
                previous_prob += it->second;
            }
        }
    }
    return result;
}

void Ramp::deliver_goods(Time t) {
    if(t == 1){
        previous_time_ = 1;
        push_package(Package());
    }
    else if(t-previous_time_ == di_){
        previous_time_ = t;
        push_package(Package());
    }
}

void Worker::do_work(Time t) {
    current_time_ = t;
    if(pd_ == 1){
        previous_time_ = t;
        Package&& p = qe_->pop();
        if(p.get_id() != 0) push_package(std::move(p));
    }
    else{
        if(t == 1){
            previous_time_ = t;
            Package&& p = qe_->pop();
            if(p.get_id() != 0) processing_buffer_.emplace(std::move(p));
        }
        else if(t-previous_time_ == pd_-1){
            previous_time_ = t;
            if(processing_buffer_.has_value()){
                get_sending_buffer().swap(processing_buffer_);
                processing_buffer_.reset();
            }
            Package&& p = qe_->pop();
            if(p.get_id() != 0) processing_buffer_.emplace(std::move(p));
        }
    }
}