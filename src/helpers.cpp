//
// Created by Wojciech Żyła on 20/12/2020.
//

#include "helpers.hpp"
#include "types.hpp"

std::string queue_type_to_string(PackageQueueType type){
    if (type == PackageQueueType::FIFO){return "FIFO";}
    else{return "LIFO";}
}

std::random_device rd;
std::mt19937 rng(rd());

double default_probability_generator() {
    // Generuj liczby pseudolosowe z przedziału [0, 1); 10 bitów losowości.
    return std::generate_canonical<double, 10>(rng);
}

ProbabilityGenerator probability_generator = default_probability_generator;