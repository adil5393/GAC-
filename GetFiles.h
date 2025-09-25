#ifndef GETFILES_H
#define GETFILES_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include "split.h"



using namespace std;

class Getfiles{
    inline static const string folder ="demo/";
    inline static const vector<string> csvFiles = {
        "structure","classId","subjectId","teacherId",
        "subjectClass","subjectPriority","subjectTeacher","subjectTeachersClass"
    };
    inline static const string ext = ".csv";
    public:
    int numberofperiods;
    map<string,int> classIds;
    map<string,int> subjectId;
    map<string,int> teacherId;

    map<string,vector<string>> subjectClasses ;
    map<string,map<string,int>> subPriority;
    map<string,map<string,string>> subjectTeacher;
    map<string,map<string,string>> subjectTeacherClass;
    vector<string> classes;
    Getfiles(int totalClasses){
        ifstream file(folder+csvFiles[1]+ext);
        string line;
        if(checkopen(file)){
            while(getline(file,line) && classes.size()<totalClasses){
                vector<string> currentline = split(line,',');
                classes.push_back(currentline[0]);
            }
        }
        file.close();
    }

    bool checkopen(ifstream& filename){
            if(!filename.is_open()){
                return false;
            }
            else{
                string line;
                getline(filename,line);
            }
            return true;
    }

    map<string,map<string,string>> getSubjectTeacherClass(){
        ifstream file(folder+csvFiles[7]+ext);
        if(!file.is_open()){
            return subjectTeacherClass;
            cout<<"Cant open file "<<csvFiles[7]<<endl;
        }
        string line;
        getline(file,line);
        vector<string> classList = split(line,',');
        while(getline(file,line)){
            vector<string> currentline = split(line,',');
            map<string,string> classandteacher;
            for(int i=1;i<classList.size();i++){
                classandteacher[classList[i]] = currentline[i];
            }
            subjectTeacherClass[currentline[0]]=classandteacher;
        }
        return subjectTeacherClass;
    }
    map<string,map<string,int>> getSubjectPriority(){
        ifstream file(folder+csvFiles[5]+ext);
        if(!file.is_open()){
            return subPriority;
            cout<<"Cant open file "<<csvFiles[4]<<endl;
        }
        string line;
        getline(file,line);
        vector<string> subjectlist = split(line,',');
        while(getline(file,line)){
            vector<string> currentline = split(line,',');
            auto it = find(classes.begin(),classes.end(),currentline[0]);
            if(it!=classes.end()){
                map<string,int> priorities;
                for(int i=1;i<subjectlist.size();i++){
                    priorities[subjectlist[i]]=stoi(currentline[i]);
                }
                subPriority[currentline[0]]=priorities;
            }
        }
        return subPriority;
        
    }

    map<string,vector<string>> getSubjectClasses(){
        ifstream file(folder+csvFiles[4]+ext);
        if(!file.is_open()){
            return subjectClasses;
            cout<<"Cant open file "<<csvFiles[4]<<endl;
        }
        else{
            string line;
            getline(file,line);
            vector<string> subjectNames = split(line,',');
            while(getline(file,line)){
                vector<string> currentline = split(line,',');
                vector<string> selectedSubjects;
                for(int i=1;i<subjectNames.size();i++){
                    if(currentline[i] == "1"){
                        selectedSubjects.push_back(subjectNames[i]);
                    }
                }
                subjectClasses[currentline[0]]=selectedSubjects;
            }
        }
        return subjectClasses;
    }

    void setIds(){
        string line;
        ifstream file(folder+csvFiles[0]+ext);
        if(checkopen(file)){
            while(getline(file,line)){
                numberofperiods = stoi(split(line,',')[1]);
            }
            file.close();
        }
        else{cout<<"can;t open "<<csvFiles[0]<<endl;}

        file.open(folder+csvFiles[1]+ext);
        if(checkopen(file)){
            while(getline(file,line)){
                vector<string> tmp = split(line,',');
                if(find(classes.begin(), classes.end(), tmp[0]) != classes.end()){
                    classIds[tmp[0]] = stoi(tmp[1]);
                }
                
            }
            file.close();
        }
        else{cout<<"can;t open "<<csvFiles[1]<<endl;}
        file.open(folder+csvFiles[2]+ext);
        if(checkopen(file)){
            while(getline(file,line)){
                vector<string> tmp = split(line,',');
                subjectId[tmp[0]]=stoi(tmp[1]);
            }
            file.close();
        }
        else{cout<<"can;t open "<<csvFiles[2]<<endl;}
        file.open(folder+csvFiles[3]+ext);
        if(checkopen(file)){
            while(getline(file,line)){
                vector<string> tmp = split(line,',');
                teacherId[tmp[0]]=stoi(tmp[1]);
            }
            file.close();
        }
        else{cout<<"can;t open "<<csvFiles[3]<<endl;}
    }
    int getnumberofperiods(){
        return numberofperiods;
    }
    map<string,int> getclassid(){
        return classIds;
    }
    map<string,int> getsubjectid(){
        return subjectId;
    }
    map<string,int> getteacherid(){
        return teacherId;
    }
    vector<string> getClasses(){
        return classes;
    }
    vector<string> getclasslist(){
        return classes;
    }
};


#endif
