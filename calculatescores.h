#ifndef CALCSCORES_H
#define CALCSCORES_H

#include<map>
#include <vector>
#include <string>
#include"split.h"
#include <algorithm>
#include<iostream>
#include "getrandomchoices.h"


using namespace std;
class CalculateScores{
    public:
    map<string,map<string,int>> subjectPriority;
    vector<string> days = {"monday","tuesday","wednesday","thursday","friday","saturday"};
    int numberofperiods;

    CalculateScores(map<string,map<string,int>> subjectPriority,int numberofperiods){
        this->subjectPriority = subjectPriority;
        this->numberofperiods = numberofperiods;

    }
    
    map<string,double> calculateindexscores(map<string,map<string,vector<string>>> Timetable){
        int totalScore;
        map<string,double> indexscores;
        for(const auto& kv: subjectPriority){
            totalScore=36;
            string currentclass = kv.first;
            for(const auto& subpri: kv.second){
                string currentsubject = subpri.first;
                int priority = subpri.second;
                if(priority<=0)continue;
                int nextpriroty;
                bool donewithpriroty=false;
                int err=0;
                while (true){
                    if(priority>days.size()){
                        nextpriroty = priority-days.size();
                        priority -=nextpriroty;
                    }
                    else{
                        donewithpriroty=true;
                    }
                    for(int periods=0;periods<numberofperiods;periods++){
                        double indexScore=0;
                        vector<string> checkifin;
                        for(string day: days){                    
                            checkifin.push_back(split(Timetable[currentclass][day][periods],'-')[1]);
                        }
                        auto cs = find(checkifin.begin(),checkifin.end(),currentsubject);
                        if(cs!=checkifin.end()){
                            for(string day: days){
                                if(split(Timetable[currentclass][day][periods],'-')[1]==currentsubject){
                                    indexScore++;
                                }
                            }
                            err += priority-indexScore;
                            //cout<<"Period--"<<periods<<"subject--"<<currentsubject<<" priroty--"<<priority<<" next pri--"<<nextpriroty<<" error--"<<err<<endl;
                        }
                    }
                    if(donewithpriroty){
                        break;
                    }
                    priority = nextpriroty;
                }
                totalScore -= err;
                // cout<<currentsubject<<"  "<<indexScore<<endl;
            }
            indexscores[currentclass] = totalScore;
            
        }
        return indexscores;
    }
    map<string,double> calculatePriorityScore(map<string,map<string,vector<string>>> Timetable){
        int prirotyScore;
        map<string,double> PriorityScore;
        for(const auto& kv:Timetable){
            string currentcls = kv.first;
            int subjectScore=0;
            for(const auto& subpri: subjectPriority[currentcls]){
                int subscore = 0;
                string sub = subpri.first;
                int pri = subpri.second;                  
                for(string day: days){
                    for(const auto& subject: Timetable[currentcls][day]){
                        if(split(subject,'-')[1]==sub){
                            subscore+=1;
                        }
                    }
                }
                subjectScore+=subscore;
            }
            PriorityScore[currentcls] = subjectScore; 
        }
        return PriorityScore;
    }
    map<int,double> PopulationScores(vector<map<string,map<string,vector<string>>>> populationSpace){
        map<int,double> populationScores;
        map<int,map<string,double>> populationindexclassscores;
        map<int,map<string,double>> populationpriorityclassscores;
        for(int i=0;i<populationSpace.size();i++){
            populationScores[i]=0;
        }
        for(int i=0;i<populationSpace.size();i++){
            double popscore = 0;
            map<string,double> indexScores = calculateindexscores(populationSpace[i]);
            int sum=0;
            for(const auto& kv:indexScores){
                sum+=kv.second;
            }
            populationScores[i] += sum;
            populationindexclassscores[i] = indexScores;//Classwise-Indexscores for each Populaiton
        }
        // for(int i=0;i<populationSpace.size();i++){
        //     double popscore = 0;
        //     map<string,double> priorityscore = calculatePriorityScore(populationSpace[i]);
        //     int sum=0;
        //     for(const auto& kv:priorityscore){
        //         sum+=kv.second;
        //     }
        //     populationScores[i] += sum;
        //     populationpriorityclassscores[i] = priorityscore;//Classwise-Priorityscores for each population
        // }
        return populationScores;

    }
    map<int,double> getdristribution(map<int,double> PopulationScores){
        double sum=0;
        map<int,double> probabilityDistribution;
        for(const auto& kv: PopulationScores){
            sum+=kv.second;
        }
        for(const auto& kv: PopulationScores){
            probabilityDistribution[kv.first]= kv.second/sum;
        }
        return probabilityDistribution;
    }
    ~CalculateScores(){

    }
};






#endif