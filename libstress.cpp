#include "libstress.h"

#include <iostream>
#include <fstream>

string getTempFile(){
    string paths[] = {
                    "/sys/class/hwmon/hwmon0/temp1_input",
                    "/sys/class/hwmon/hwmon1/temp1_input",
                    "/sys/class/hwmon/hwmon2/temp1_input",
                    "/sys/class/hwmon/hwmon3/temp1_input",
                    "/sys/class/hwmon/hwmon4/temp1_input",
                    "/sys/class/hwmon/hwmon5/temp1_input",
                    "/sys/class/hwmon/hwmon6/temp1_input",
                    "/sys/class/hwmon/hwmon7/temp1_input",
                    "/sys/class/hwmon/hwmon8/temp1_input",
                    "/sys/class/thermal/thermal_zone0/temp"};
    for(auto path : paths){
        ifstream file;
        file.open(path);
        if(!file.fail()){
            return path;
        }
    }
    return "";
}

int getTemp(string tempFilePath){
    int temp;
    ifstream sysTemp;
    sysTemp.open(tempFilePath);
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
    if(sysCpu.fail()){
        return "";
    }
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