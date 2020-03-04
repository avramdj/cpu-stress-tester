#include "stresslib.h"

#include <iostream>
#include <fstream>

int getTemp(){
    int temp;
    ifstream sysTemp;
    sysTemp.open("/sys/classs/thermal/thermal_zone0/temp");
    if(sysTemp.fail()){
        return -1;
    }
    sysTemp >> temp;
    temp /= 1000;
    return temp;
}
string getCpuName(){
    ifstream sysCpu;
    sysCpu.open("/proc/cpuinfo");
    string line;
    int p;
    string key = "model name	: ";
    while(getline(sysCpu, line)){
        if(line.find(key) != string::npos){
            break;
        }
    }
    string cpuName = line.substr(key.length());

    return cpuName;
}