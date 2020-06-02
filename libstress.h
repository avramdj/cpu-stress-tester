#ifndef LIB_STRESS_H
#define LIB_STRESS_H

#include <iostream>

extern "C" void stress(bool* run);
std::string get_cpu_name();
std::string get_temp_path();
int get_temp(std::string tempFilePath);

#endif