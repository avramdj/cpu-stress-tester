#include "stresslib.h"

#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <csignal>

using namespace std;

int main(){

    bool run = true;
    string CPUNAME = getCpuName();
    int NUMCORES = sysconf(_SC_NPROCESSORS_ONLN);
    if(CPUNAME != ""){
        cout << "Model name:      " << CPUNAME << endl;
    } else {
        cerr << "Couldn't get CPU model name" << endl;
    }
    cout << "Number of CPUs:  " << NUMCORES << endl;
    cout << endl;

    bool monitor = true;
    string tempFilePath = getTempFile();
    if(tempFilePath == ""){
        string answ;
        cerr << "ERROR : Failed to open temperature file, possibly missing sensor drivers!" << endl;
        cerr << "try : sudo apt install lm-sensors && sudo sensors-detect" << endl;
        cerr << endl;
        cout << "Continue stress test without monitoring temperature? [Y/n] : ";
        getline(cin, answ);
        if(answ.find('n') != string::npos || answ.find('N') != string::npos){
            exit(EXIT_SUCCESS);
        } else {
            monitor = false;
        }
    }
    cout << "Starting " << NUMCORES << " stress threads..." << endl;
    vector<thread> stressThreads(NUMCORES);
    for(int i = 0; i < NUMCORES; i++){
        stressThreads[i] = thread(assemblyStress, &run);
    }
    cout << "Started!" << endl << endl;

    int minT = numeric_limits<int>::max();
    int maxT = 0;
    auto start = time(NULL);
    while(run){
        if(monitor){
            int temp = getTemp(tempFilePath);
            minT = min(minT, temp);
            maxT = max(maxT, temp); 
            cout << "CPU Temperature: "<< temp << "°C [MIN : " << minT << "°C | MAX : " << maxT << " °C]" << endl;
        }
        cout << "Time elapsed: " << time(NULL) - start << " seconds. " << "CTRL + C to exit..." << endl;
        sleep(1);
        cout << "\r\033[A";
        if(monitor){
            cout << "\033[A";
        }
    }

    exit(EXIT_SUCCESS);
}