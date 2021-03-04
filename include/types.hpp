//
// Created by Wojciech Żyła on 18/12/2020.
//

#ifndef NET_SIMULATION_TYPES_HPP
#define NET_SIMULATION_TYPES_HPP
#include <functional>

using ElementID = int;
using Time = int;
using TimeOffset = int;
using ProbabilityGenerator = std::function<double()>;
#endif //NET_SIMULATION_TYPES_HPP
