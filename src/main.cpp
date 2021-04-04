#include <include/libstress.h>

#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <fstream>
#include <time.h>
#include <unistd.h>
#include <csignal>

int main(){

    bool run = true;
    std::string CPUNAME = get_cpu_name();
    int NUMCORES = sysconf(_SC_NPROCESSORS_ONLN);
    if(CPUNAME != ""){
        std::cout << "Model name:      " << CPUNAME << std::endl;
    } else {
        std::cerr << "Couldn't get CPU model name" << std::endl;
    }
    std::cout << "Number of CPUs:  " << NUMCORES << std::endl;
    std::cout << std::endl;

    bool monitor = true;
    std::string tempFilePath = get_temp_path();
    if(tempFilePath == ""){
        std::string answ;
        std::cerr << "ERROR : Failed to open temperature file, possibly missing sensor drivers!" << std::endl;
        std::cerr << "try : sudo apt install lm-sensors && sudo sensors-detect" << std::endl;
        std::cerr << std::endl;
        std::cout << "Continue stress test without monitoring temperature? [Y/n] : ";
        getline(std::cin, answ);
        if(answ.find('n') != std::string::npos || answ.find('N') != std::string::npos){
            exit(EXIT_SUCCESS);
        } else {
            monitor = false;
        }
    }
    std::cout << "Starting " << NUMCORES << " stress threads..." << std::endl;
    std::vector<std::thread> stressThreads(NUMCORES);
    for(int i = 0; i < NUMCORES; i++){
        stressThreads[i] = std::thread(stress, &run);
    }
    std::cout << "Started!" << std::endl << std::endl;

    int min_temp = std::numeric_limits<int>::max();
    int max_temp = 0;
    auto start = time(NULL);
    while(run){
        if(monitor){
            int temp = get_temp(tempFilePath);
            min_temp = std::min(min_temp, temp);
            max_temp = std::max(max_temp, temp); 
            std::cout << "CPU Temperature: "<< temp << "°C [MIN : " << min_temp << "°C | MAX : " << max_temp << "°C]" << std::endl;
        }
        std::cout << "Time elapsed: " << time(NULL) - start << " seconds. " << "CTRL + C to exit..." << std::endl;
        sleep(1);
        std::cout << "\r\033[A";
        if(monitor){
            std::cout << "\033[A";
        }
    }

    exit(EXIT_SUCCESS);
}
