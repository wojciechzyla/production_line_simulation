//
// Created by Wojciech Żyła on 20/12/2020.
//

#ifndef NET_SIMULATION_HELPERS_HPP
#define NET_SIMULATION_HELPERS_HPP

#include "types.hpp"
#include "storage_types.hpp"
#include <iostream>
#include <random>
#include <vector>
#include <map>

extern std::string queue_type_to_string(PackageQueueType type);

extern std::random_device rd;
extern std::mt19937 rng;

extern double default_probability_generator();

extern ProbabilityGenerator probability_generator;

#endif //NET_SIMULATION_HELPERS_HPP
