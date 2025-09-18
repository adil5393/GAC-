#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include "GetFiles.h"
#include "CreateTimetable.h"

using namespace std;

// Print map<string, int>
void printMapStringInt(const map<string, int>& data, const string& title) {
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
                cout << val << " ";
            }
            cout << "\n";
        }
        cout << "\n";
    }
}


int main(){
    Getfiles timetable;
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
    map<string,map<string,vector<string>>> Timetable = pairs.getclasswise(allpairs);
    vector<map<string,map<string,vector<string>>>> populationSpace = pairs.createPopulation(3,allpairs);
    printMapStringVector(allpairs,"Timetable bairs");
    printNestedMap(Timetable);
    for(const auto& tt: populationSpace){
        printNestedMap(tt);
    }

    return 0;
    
}


