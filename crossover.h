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
    map<string, map<string,string>> subjectTeacherClass;
    vector<string> classes;

public:
    Crossover(
              int n,
              map<string,map<string,int>> subjectPriority,
              int numberofperiods,
              int totalpopulation,
            map<string, map<string,string>> subjectTeacherClass,
            vector<string> classes )
    {
        this->n = n;
        this->subjectPriority = subjectPriority;
        this->numberofperiods = numberofperiods;
        this->totalpopulation = totalpopulation;
        this-> subjectTeacherClass = subjectTeacherClass;
        this->classes = classes;
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
    map<string,map<string,vector<string>>> fixpriority(map<string,map<string,vector<string>>> child) 
    {
        for (const auto& [currentcls, subjPriorities] : subjectPriority) {
            map<string,int> actualCounts;

            // Count how many times each subject already appears
            for (const auto& [subject, pri] : subjPriorities) {
                int count = 0;
                for (const auto& day : days) {
                    for (int period = 0; period < numberofperiods; period++) {
                        const string& slot = child[currentcls][day][period];

                        if (slot.size() > 1) {
                            auto parts = split(slot, '-');
                            if (parts.size() > 1 && parts[1] == subject) {
                                count++;
                                if (count > pri) {
                                    // Remove excess
                                    child[currentcls][day][period] = "-";
                                }
                            }
                        }
                    }
                }
                actualCounts[subject] = count;
            }

            // Fix under-represented subjects
            for (const auto& [subject, count] : actualCounts) {
                int required = subjectPriority.at(currentcls).at(subject);
                if (count < required) {
                    int toAdd = required - count;

                    // Construct safe "teacher-subject" pair
                    string pair;
                    auto subjIt = subjectTeacherClass.find(subject);
                    if (subjIt != subjectTeacherClass.end()) {
                        auto clsIt = subjIt->second.find(currentcls);
                        if (clsIt != subjIt->second.end()) {
                            pair = clsIt->second + "-" + subject;
                        } else {
                            cerr << "[WARN] Teacher missing for class " << currentcls
                                << " subject " << subject << endl;
                            continue;
                        }
                    } else {
                        cerr << "[WARN] Subject " << subject
                            << " not found in subjectTeacherClass" << endl;
                        continue;
                    }

                    // Place missing occurrences
                    for (const auto& day : days) {
                        for (int period = 0; period < numberofperiods; period++) {
                            if (child[currentcls][day][period] == "-") {
                                // cout << "Fixing: class=" << currentcls
                                //      << " subject=" << subject
                                //      << " -> " << pair
                                //      << " on " << day << " period=" << period << endl;

                                child[currentcls][day][period] = pair;
                                toAdd--;
                            }
                            if (toAdd == 0) break;
                        }
                        if (toAdd == 0) break;
                    }
                }
            }
        }
        return child;
    } 
    map<string,map<string,vector<string>>> mutate(map<string,map<string,vector<string>>> child){
        string cls = classes[getrandomint(0,classes.size()-1)];
        string day = days[getrandomint(0,days.size()-1)];
        int p1 = getrandomint(0,numberofperiods-1);
        int p2 = getrandomint(0,numberofperiods-1);
        if (p1 != p2) {
            swap(child[cls][day][p1], child[cls][day][p2]);
        }
        return child;
    }



    // Create a child from two selected parents
    map<string,map<string,vector<string>>> childfrompopulation(vector<int>& keys,vector<double>& weights,vector<map<string,map<string,vector<string>>>> populationSpace,map<int,double> PopulationProbabilityDistribution) {
        map<string,map<string,vector<string>>> child;

        // Use references to avoid copying large maps
        for(string cls: classes){
            vector<int> parents= random_choices(keys,weights,n); 
            auto& parent1 = populationSpace[parents[0]];
            auto& parent2 = populationSpace[parents[1]];

            // Determine which parent is fitter
            if(PopulationProbabilityDistribution[parents[0]] < PopulationProbabilityDistribution[parents[1]]) {
                swap(parent1, parent2);
            }
            child[cls] = parent1[cls];
        }
        
        child = fixpriority(child);
        if(getrandom()<=0.01){
            child = mutate(child);
        }
        return child;
    }

    vector<map<string,map<string,vector<string>>>> newGeneration(vector<map<string,map<string,vector<string>>>> populationSpace,map<int,double> PopulationProbabilityDistribution) {
        // Compute population scores once per generation

        vector<map<string,map<string,vector<string>>>> newgen;
        // newgen.reserve(totalpopulation);

        // Precompute keys and weights for random selection
        vector<int> keys;
        vector<double> weights;
        for(const auto& kv: PopulationProbabilityDistribution) {
            keys.push_back(kv.first);
            weights.push_back(kv.second);
        }

        while(newgen.size() < totalpopulation) {
            // Select two parents for crossover
            newgen.push_back(childfrompopulation(keys,weights,populationSpace,PopulationProbabilityDistribution));
            // cout<<newgen.size()<<endl;
        }

        return newgen;
    }

    ~Crossover() {}
};

#endif
