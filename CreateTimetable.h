#ifndef CREATETIMETABLE_H
#define CREATETIMETABLE_H

#include<iostream>
#include<vector>
#include<string>
#include<map>
#include <algorithm> 
#include<random>

using namespace std;

class CreateTimetable
{
private:
map<string,map<string,vector<string>>> timetable;
vector<string> days = {"monday","tuesday","wednesday","thursday","friday","saturday"};
map<string,vector<string>> classsubjectteacher;
int numberofperiods;
map<string,int> classIds;
map<string,vector<string>> subjectClasses;
map<string,map<string,int>> subjectPriority;
map<string, map<string,string>> subjectTeacherClass;

int subjectsperweek = days.size();
    /* data */
public:
    CreateTimetable(int numberofperiods,map<string,int> classIds,map<string,vector<string>> subjectClasses,map<string,map<string,int>> subjectPriority,map<string, map<string,string>> subjectTeacherClass){
        this->numberofperiods = numberofperiods;
        this->classIds = classIds;
        this->subjectClasses=subjectClasses;
        this->subjectPriority=subjectPriority;
        this->subjectTeacherClass = subjectTeacherClass;

        for(const auto& cls: classIds){
            string currentclass = cls.first;
            vector<string>tchsub;
            for(const auto& subject: subjectTeacherClass){
                string currentsubject = subject.first;
                string teacher = subjectTeacherClass[currentsubject][currentclass];
                string pair = teacher+"-"+currentsubject;
                for(int p=0;p<subjectPriority[currentclass][currentsubject];p++){
                    tchsub.push_back(pair);
                }
            }
            classsubjectteacher[currentclass] = tchsub;
        }
    }
    ~CreateTimetable(){

    }
    map<string,vector<string>> getAllpairs(){
        return classsubjectteacher;
    }
     map<string,map<string,vector<string>>> getclasswise(map<string,vector<string>> pairs){
        int numberofdays = days.size();
        for(auto& pair: pairs){
            string cls = pair.first;
            map<string,vector<string>> dayandpairs;
            int d = 0;
            random_device rd;
            mt19937 g(rd());
            shuffle(pair.second.begin(),pair.second.end(),g);
        
            for(const auto& tchpair: pair.second){
                dayandpairs[days[d]].push_back(tchpair);
                if(dayandpairs[days[d]].size()==numberofperiods){
                    d++;
                }
                timetable[cls]=dayandpairs;
            }
        }
        return timetable;
    }
    vector<map<string,map<string,vector<string>>>> createPopulation(int totalpopulation,map<string,vector<string>> pairs){
        vector<map<string,map<string,vector<string>>>> populationspace;
        for (int i=0;i<totalpopulation;i++){
            populationspace.push_back(getclasswise(pairs));
        }
        return populationspace;
    }
};





#endif
