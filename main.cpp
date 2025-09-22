#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <sstream>

#include "GetFiles.h"
#include "CreateTimetable.h"
#include "scoreshouldbe.h"
#include "crossover.h"


using namespace std;

// Print map<string, int>
void printMapStringInt(const map<int, double>& data, const string& title) {
    cout << "=== " << title << " ===" << endl;
    for (const auto& [key, value] : data) {
        cout << key << " -> " << value << endl;
    }
    cout << endl;
}

// Print map<string, vector<string>>
void printMapStringVector(const map<string, vector<string>>& data, const string& title) {
    cout << "=== " << title << " ===" << endl;
    for (const auto& [key, vec] : data) {
        cout << key << " -> [ ";
        for (const auto& val : vec) {
            cout << val << " ";
        }
        cout << "]" << endl;
    }
    cout << endl;
}

// Print map<string, map<string,int>>
void printNestedMapStringInt(const map<string, map<string,int>>& data, const string& title) {
    cout << "=== " << title << " ===" << endl;
    for (const auto& [outerKey, innerMap] : data) {
        cout << outerKey << " -> {" << endl;
        for (const auto& [innerKey, innerValue] : innerMap) {
            cout << "   " << innerKey << " : " << innerValue << endl;
        }
        cout << "}" << endl;
    }
    cout << endl;
}

// Print map<string, map<string,string>>
void printNestedMapStringString(const map<string, map<string,string>>& data, const string& title) {
    cout << "=== " << title << " ===" << endl;
    for (const auto& [outerKey, innerMap] : data) {
        cout << outerKey << " -> {" << endl;
        for (const auto& [innerKey, innerValue] : innerMap) {
            cout << "   " << innerKey << " : " << innerValue << endl;
        }
        cout << "}" << endl;
    }
    cout << endl;
}
void printNestedMap(const map<string, map<string, vector<string>>>& data) {
    for (const auto& outerPair : data) {
        cout << "Outer Key: " << outerPair.first << "\n";

        for (const auto& innerPair : outerPair.second) {
            cout << "  Inner Key: " << innerPair.first << " -> ";

            for (const auto& val : innerPair.second) {
                cout << val << "||";
            }
            cout << "\n";
        }
        cout << "\n";
    }
}
pair<int, double> findMaxValue(const std::map<int, double>& mymap) {
    auto max_it = std::max_element(
        mymap.begin(), mymap.end(),
        [](const auto& a, const auto& b) {
            return a.second < b.second;  // compare by value
        }
    );

    if (max_it != mymap.end()) {
        return {max_it->first, max_it->second}; // return key,value pair
    }
    return {-1, -1.0}; // return invalid if map is empty
}


int main(){

    int totalClasses=5;
    int totalpopulation = 1000;
    Getfiles timetable(totalClasses);
    timetable.setIds();   

    int numberofperiods = timetable.getnumberofperiods();

    map<string,int> classIds = timetable.getclassid();
    map<string,int> subjectId = timetable.getsubjectid();
    map<string,int> teacherId = timetable.getteacherid();

    map<string,vector<string>> subjectClasses = timetable.getSubjectClasses();
    map<string,map<string,int>> subjectPriority = timetable.getSubjectPriority();
    map<string, map<string,string>> subjectTeacherClass = timetable.getSubjectTeacherClass();

    CreateTimetable pairs(numberofperiods,classIds,subjectClasses,subjectPriority,subjectTeacherClass);
    map<string,vector<string>> allpairs = pairs.getAllpairs();
    //map<string,map<string,vector<string>>> Timetable = pairs.getclasswise(allpairs);
    vector<map<string,map<string,vector<string>>>> populationSpace = pairs.createPopulation(totalpopulation,allpairs);


    // printMapStringVector(allpairs,"Timetable bairs");
    // printNestedMap(Timetable);
    // for(const auto& tt: populationSpace){
    //     printNestedMap(tt);
    // }
    //printNestedMap(Timetable);
    CalculateScores firstscores(subjectPriority,numberofperiods,populationSpace);
    map<int,double> fscores= firstscores.PopulationScores();
    map<int,double> probadist = firstscores.getdristribution(fscores);
    auto result = findMaxValue(probadist);
    std::cout << "Key: " << result.first 
              << " Value: " << result.second << std::endl;
    Crossover fcrossover(populationSpace,2,subjectPriority,numberofperiods,totalpopulation);
    for(int i = 0;i<=100;i++){
        populationSpace = fcrossover.newGeneration(populationSpace,probadist);
        firstscores=CalculateScores(subjectPriority,numberofperiods,populationSpace);
        fscores = firstscores.PopulationScores();
        result = findMaxValue(fscores);
        
        std::cout << "Key: " << result.first 
                << " Value: " << result.second << std::endl;
        if(result.second==0){break;}
    }
    printNestedMap(populationSpace[result.first]);
    // printMapStringInt(populationscores,"Index Scores");
    // map<string,double> priorityscores = priorityScore(subjectPriority,classIds);
    
    // printMapStringInt(PopulationProbabilityDistribution,"Population Probability Dist");
    //Crossover crossover(populationSpace,2,subjectPriority,numberofperiods,totalpopulation);
    // vector<int> bestkeys = crossover.getKeysandWeights();
    // for(const auto& bestofthebest: bestkeys){
    //     cout<<bestofthebest<<"  "<<endl;
    // }
    return 0;
}


