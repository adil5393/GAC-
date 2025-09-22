#ifndef SPLIT_H
#define SPLIT_H

#include<string>
#include<vector>
#include<sstream>


using namespace std;


vector<string> split(const string& line, char delimiter) {
    
    vector<string> parts;
    stringstream ss(line);
    string item;
    
    while (getline(ss, item, delimiter)) {
        parts.push_back(item);
    }
    return parts;
}

#endif