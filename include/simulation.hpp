//
// Created by Wojciech Żyła on 15/01/2021.
//

#ifndef NET_SIMULATION_SIMULATION_HPP
#define NET_SIMULATION_SIMULATION_HPP

#include "reports.hpp"

extern void simulate(Factory& f, TimeOffset d, std::function<void (Factory&, Time)> rf);

#endif //NET_SIMULATION_SIMULATION_HPP
