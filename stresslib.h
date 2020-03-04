#ifndef STRESSLIB_H
#define STRESSLIB_H

#include <iostream>

using namespace std;

extern "C" void assemblyStress(bool* run);
string getCpuName();
int getTemp();

#endif