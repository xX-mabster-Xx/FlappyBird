#pragma once

#include <vector>
#include <algorithm>
#include <cmath>

#include <random>
#include <chrono>
#include <fstream>


class Neuron{
public:
    double value, activated_value, derived_value;
    void activate() {
        //activated_value = 1 / (1 + exp(-value));
        activated_value = (exp(2 * value) - 1)/(exp(2 * value) + 1);
    }
    void derive() {
        derived_value = activated_value * (1 - activated_value);
    }
    void setValue(double d) {
        value = d;
        activate();
        derive();
    }
};

class NeuroNet{

public:
    using adjasency_graph_t = std::vector < std::vector <std::pair <size_t, double> > >;

    size_t inputLayerSize; // first layer size
    size_t outputLayerSize;
    size_t n; // num of neurons
    double E = 0.7, A = 0.3; //
    std::vector <Neuron> neu;//neurons
    adjasency_graph_t g;//graph
    std::vector <size_t> pref;

    //std::vector < std::vector <std::pair <size_t, double> > > g2;//last deltas of sinapses
    //std::vector <double> delta;
    NeuroNet(const std::vector <size_t> & layers);
    void consts(double e, double a);
    void getAns();
    void clear_values();
//    void abs_mutation(double mut);
    void rel_mutation(double mut);
};

std::ostream & operator<< (std::ostream & out, NeuroNet net);
