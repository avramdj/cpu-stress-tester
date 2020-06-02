#include "libstress.h"

#include <iostream>
#include <fstream>

std::string get_temp_path(){
    std::string paths[] = {
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
        std::ifstream file;
        file.open(path);
        if(!file.fail()){
            return path;
        }
    }
    return "";
}

int get_temp(std::string tempFilePath){
    int temp;
    std::ifstream sysTemp;
    sysTemp.open(tempFilePath);
    if(sysTemp.fail()){
        return -1;
    }
    sysTemp >> temp;
    temp /= 1000;
    return temp;
}
std::string get_cpu_name(){
    std::ifstream sysCpu;
    sysCpu.open("/proc/cpuinfo");
    if(sysCpu.fail()){
        return "";
    }
    std::string line;
    int p;
    std::string key = "model name	: ";
    while(std::getline(sysCpu, line)){
        if(line.find(key) != std::string::npos){
            break;
        }
    }
    std::string cpuName = line.substr(key.length());

    return cpuName;
}