#include "neuro_net.h"
#include <fstream>
#include <vector>


double random1(double min, double max);

double random2(double c, double sigm);

NeuroNet::NeuroNet(const std::vector <size_t> & layers) {
    size_t t = layers.size();
    pref.assign(t + 1, 0);
    for (size_t i = 0; i < t; ++i) {
        pref[i + 1] = pref[i] + layers[i] + 1;
    }
    --pref.back();
    n = pref.back();
    inputLayerSize = pref[1];
    outputLayerSize = pref.back() - pref[pref.size() - 2];
    neu.resize(n);
    g.resize(n);
    //delta.resize(n);
    //g2.resize(n);
    for (size_t i = 0; i < t - 1; ++i) {
        for (size_t j = pref[i]; j < pref[i + 1]; ++j) {
            for (size_t q = pref[i + 1]; q < pref[i + 2] - (i + 3 != pref.size()); ++q) {
                g[j].push_back({q, random1(-5, 5)});
                //g2[j].push_back({q, 0});
            }
        }
    }
}
void NeuroNet::consts(double e, double a) {
    E = e;
    A = a;
}
void NeuroNet::getAns() {
    neu[inputLayerSize - 1].value = 1; //bias
    for (size_t i = 0; i < inputLayerSize; ++i) {
        for (const auto & x : g[i]) {
            neu[x.first].value += neu[i].value * x.second;
        }
    }
    size_t j = 2;
    for (size_t i = inputLayerSize; i < g.size(); ++i) {
        neu[i].activate();
        bool is_bias_neuron = (j != pref.size() - 1 && i == pref[j] - 1);
        if (is_bias_neuron) {
            neu[i].activated_value = 1;
            ++j;
        }
        for (const auto & x : g[i]) {
            neu[x.first].value += neu[i].activated_value * x.second;
        }
    }
}
void NeuroNet::clear_values() {
    for (size_t i = inputLayerSize; i < n; ++i) {
        neu[i].value = 0;
    }
}
//void NeuroNet::abs_mutation(double mut) {
//    for (auto & x: g) {
//        for (auto & y: x) {
//            y.second += random1(-mut, mut);
//        }
//    }
//}
void NeuroNet::rel_mutation(double mut) {
    for (auto & x: g) {
        for (auto & y: x) {
            if (random1(0, 100) < 20)
                y.second *= (1 + random2(0, mut)) ;
        }
    }
}

std::ostream & operator<< (std::ostream & out, NeuroNet net) {
   for (auto & x : net.g) {
       for (auto & y : x) {
           out << y.second << " ";
       }
       out << "\n";
   }
   return out;
}
