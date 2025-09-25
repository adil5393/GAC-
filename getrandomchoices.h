#ifndef GETRANDOMCHOICES_H
#define GETRANDOMCHOICES_H


#include <iostream>
#include <vector>
#include <string>
#include <random>
#include <numeric>   // for std::discrete_distribution

using namespace std;

vector<int> random_choices(
    const vector<int>& keys,
    const vector<double>& weights,
    int n
) {
    if(keys.size() != weights.size()) {
        throw invalid_argument("keys and weights must have the same size");
    }

    random_device rd;
    mt19937 gen(rd());  // random number generator

    // std::discrete_distribution picks index according to weights
    discrete_distribution<> dist(weights.begin(), weights.end());

    vector<int> result;
    result.reserve(n);

    for(int i = 0; i < n; i++) {
        int idx = dist(gen);
        result.push_back(keys[idx]);
    }

    return result;
}
int getrandomint(int start, int end) {
    std::random_device rd;  // seed
    std::mt19937 gen(rd()); // Mersenne Twister RNG
    std::uniform_int_distribution<> dist(start, end);

    int random_number = dist(gen);

    return random_number;
}
double getrandom() {
    random_device rd;  
    mt19937 gen(rd());  // Mersenne Twister engine
    uniform_real_distribution<> dist(0.0, 1.0); // range [0,1)
    double r = dist(gen);
    return r;
}


#endif