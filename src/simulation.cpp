//
// Created by Wojciech Żyła on 15/01/2021.
//
#include "simulation.hpp"

void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf){
    if (!f.is_consistent()) throw std::logic_error("Niespójna sieć.");

    for (Time t = 1; t <= d; t++){
        f.do_deliveries(t);
        f.do_package_passing();
        f.do_work(t);
        rf(f, t);
    }
}

