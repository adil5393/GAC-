#ifndef SCORESHOULDBE_H
#define SCORESHOULDBE_H

#include<map>
#include<string>
#include<vector>

using namespace std;

map<string,double>  priorityScore(map<string,map<string,int>> subjectPriority,map<string,int> classids){
    map<string,double> priorityScores;
    for(const auto& cls: classids){
        double sum=0;
        string currentclass = cls.first;
        for(const auto& kv:subjectPriority[currentclass]){
            sum+=kv.second;
        }
        priorityScores[currentclass] = sum;
    }
    return priorityScores;
}
// map<string,double> overlapscore(){

// }
map<string,double> indexScore(map<string,map<string,int>> subjectPriority,map<string,int> classids,int numberofperiods){
    map<string,double> indexScores;
    for(const auto& cls: classids){
        double sum =0;
        string currentclass = cls.first;
        for(const auto& kv:subjectPriority[currentclass]){
            sum+=kv.second;
        }
        indexScores[currentclass]=sum;
    }
    return indexScores;
}



#endif