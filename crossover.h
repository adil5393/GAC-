#ifndef CROSSOVER_H
#define CROSSOVER_H

#include <vector>
#include <string>
#include <map>
#include <iostream>
#include "getrandomchoices.h"
#include "calculatescores.h"

using namespace std;

class Crossover {
private:
    vector<string> days = {"monday","tuesday","wednesday","thursday","friday","saturday"};
    map<string,map<string,int>> subjectPriority;
    int numberofperiods;
    int n;
    int totalpopulation;
    map<int,double> populationscores;

public:
    Crossover(vector<map<string,map<string,vector<string>>>> populationSpace,
              int n,
              map<string,map<string,int>> subjectPriority,
              int numberofperiods,
              int totalpopulation)
    {
        this->n = n;
        this->subjectPriority = subjectPriority;
        this->numberofperiods = numberofperiods;
        this->totalpopulation = totalpopulation;
    }

    // Precomputed random selection of parent indices
    vector<int> getKeysandWeights(map<int,double> PopulationProbabilityDistribution) {
        vector<int> keys;
        vector<double> weights;
        for(const auto& kv: PopulationProbabilityDistribution){
            keys.push_back(kv.first);
            weights.push_back(kv.second);
        }
        return random_choices(keys, weights, n);
    }

    // Create a child from two selected parents
    map<string,map<string,vector<string>>> childfrompopulation(const vector<int>& key,vector<map<string,map<string,vector<string>>>> populationSpace,map<int,double> PopulationProbabilityDistribution) {
        map<string,map<string,vector<string>>> child;

        // Use references to avoid copying large maps
        auto& parent1 = populationSpace[key[0]];
        auto& parent2 = populationSpace[key[1]];

        // Determine which parent is fitter
        if(PopulationProbabilityDistribution[key[0]] < PopulationProbabilityDistribution[key[1]]) {
            swap(parent1, parent2);
        }

        // Random crossover point at day level
        int crossoverpoint = getrandomint(0, days.size()-1);
        vector<string> days1(days.begin(), days.begin() + crossoverpoint);
        vector<string> days2(days.begin() + crossoverpoint, days.end());

        // Build child
        for(const auto& kv: parent1) {
            string currentclass = kv.first;
            map<string,vector<string>> inner;

            for(const auto& day: days1) inner[day] = parent1.at(currentclass).at(day);
            for(const auto& day: days2) inner[day] = parent2.at(currentclass).at(day);

            child[currentclass] = inner;
        }

        return child;
    }

    vector<map<string,map<string,vector<string>>>> newGeneration(vector<map<string,map<string,vector<string>>>> populationSpace,map<int,double> PopulationProbabilityDistribution) {
        // Compute population scores once per generation

        vector<map<string,map<string,vector<string>>>> newgen;
        newgen.reserve(totalpopulation);

        // Precompute keys and weights for random selection
        vector<int> keys;
        vector<double> weights;
        for(const auto& kv: PopulationProbabilityDistribution) {
            keys.push_back(kv.first);
            weights.push_back(kv.second);
        }

        while(newgen.size() < totalpopulation) {
            // Select two parents for crossover
            vector<int> selected = random_choices(keys, weights, n);
            newgen.push_back(childfrompopulation(selected,populationSpace,PopulationProbabilityDistribution));
        }

        return newgen;
    }

    ~Crossover() {}
};

#endif
