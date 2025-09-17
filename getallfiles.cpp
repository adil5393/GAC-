#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include "GetFiles.h"

using namespace std;


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
    return 0;
}