//
// Created by adria on 09.01.2021.
//

#include <sstream>
#include <iostream>
#include <vector>
#include <sstream>
#include "reports.hpp"

void generate_structure_report(const Factory& f, std::ostream& os){
    std::vector<std::list<int>> heler_vector;
    os<<"\n== LOADING RAMPS ==";
    std::list<ElementID> ids;
    for (auto it = f.ramp_cbegin(); it != f.ramp_cend(); it++) {
        ids.push_back(it->get_id());
    }
    ids.sort();
    for (auto id: ids){
        os << "\n\nLOADING RAMP #" << id << "\n  Delivery interval: " << f.find_ramp_by_id(id)->get_delivery_interval()<< "\n  Receivers:";
        NodeCollection<Ramp>::const_iterator ramp = f.find_ramp_by_id(id);
        heler_vector = sorted_workers_stores(ramp);
        for(auto w:heler_vector[1]){
            os<<"\n    "<<"worker #" << w;
        }
    }
    heler_vector.clear();
    ids.clear();

    os<<"\n\n\n== WORKERS ==";

    for (auto it = f.worker_cbegin(); it != f.worker_cend(); it++){
        ids.push_back(it->get_id());
    }
    ids.sort();
    for (auto i: ids){
        os<<"\n\nWORKER #"<<i<<"\n  Processing time: "<<f.find_worker_by_id(i)->get_processing_duration()<<"\n  Queue type: "<<queue_type_to_string(f.find_worker_by_id(i)->get_queue()->get_queue_type())<<"\n  Receivers:";
        NodeCollection<Worker>::const_iterator worker = f.find_worker_by_id(i);
        heler_vector = sorted_workers_stores(worker);

        for(auto s:heler_vector[0]){
            os<<"\n    storehouse #" << s;
        }
        for(auto w:heler_vector[1]){
            os<<"\n    worker #" << w;
        }
        heler_vector.clear();
    }
    ids.clear();

    os<<"\n\n\n== STOREHOUSES ==";
    for (auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++){
        ids.push_back(it->get_id());
    }
    ids.sort();
    for (auto i: ids){
        os<<"\n\nSTOREHOUSE #"<<i;
    }
    os<<"\n\n";
    ids.clear();
}

void generate_simulation_turn_report(const Factory& f, std::ostream& os, Time t){
    os<<"=== [ Turn: "<<t<<" ] ===\n\n";
    os<<"== WORKERS ==";
    std::list<ElementID> ids;
    for (auto it = f.worker_cbegin(); it != f.worker_cend(); it++){
        ids.push_back(it->get_id());
    }
    ids.sort();
    for (auto id: ids){
        os<<"\n\nWORKER #"<<id;

        if(f.find_worker_by_id(id)->get_processing_buffer().has_value()){
            os<<"\n  PBuffer: #"<<f.find_worker_by_id(id)->get_processing_buffer()->get_id()<<" (pt = "<<f.find_worker_by_id(id)->get_package_processing_time()<<")\n";
        }
        else{
            os<<"\n  PBuffer: (empty)"<<"\n";
        }

        os<<"  Queue: ";
        if(f.find_worker_by_id(id)->end() == f.find_worker_by_id(id)->begin()) os<<"(empty)";
        else{
            os<<"#"<<f.find_worker_by_id(id)->begin()->get_id();
            auto sec_itt = f.find_worker_by_id(id)->cbegin();
            sec_itt++;
            for (auto itt = sec_itt; itt != f.find_worker_by_id(id)->end(); itt++){
                os<<", #"<<itt->get_id();
            }
        }

        if(f.find_worker_by_id(id)->get_sending_buffer().has_value()){
            os<<"\n  SBuffer: #"<<f.find_worker_by_id(id)->get_sending_buffer()->get_id();
        }
        else{
            os<<"\n  SBuffer: (empty)";
        }
    }
    ids.clear();
    os<<"\n\n\n== STOREHOUSES ==";

    for(auto it = f.storehouse_cbegin(); it != f.storehouse_cend(); it++){
        ids.push_back(it->get_id());
    }
    ids.sort();
    for (auto id: ids){
        os<<"\n\nSTOREHOUSE #"<<f.find_storehouse_by_id(id)->get_id()<<"\n  Stock: ";
        if(f.find_storehouse_by_id(id)->begin() == f.find_storehouse_by_id(id)->end()) os<<"(empty)";
        else{
            os<<"#"<<f.find_storehouse_by_id(id)->begin()->get_id();
            auto sec_itt = f.find_storehouse_by_id(id)->begin();
            sec_itt++;
            for (auto itt = sec_itt; itt != f.find_storehouse_by_id(id)->end(); itt++){
                os<<", #"<<itt->get_id();
            }
        }
    }
    os<<"\n\n";
}