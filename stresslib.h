#ifndef STRESSLIB_H
#define STRESSLIB_H

#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <csignal>

using namespace std;

extern "C" void assemblyStress(bool* run);
string getCpuName();
int getTemp();

#endif