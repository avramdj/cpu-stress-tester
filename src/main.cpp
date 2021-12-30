#include <include/libstress.h>

#include <iostream>
#include <thread>
#include <vector>
#include <cassert>
#include <fstream>
#include <time.h>
#include <ctime>
#include <unistd.h>
#include <csignal>
#include <unistd.h>

using namespace std;

void help(void){
  cout << "Usage: stress [-h|-t timeout_seconds|-p]" << endl;
  exit(1);
}

int main(int argc, char **argv){

    bool run = true;
    int i;
    bool print_all = false;
    long long timeout = -1;
    long long elapsed;
    char timestring[100];
    time_t now;
    std::tm* tm_info;

    while((i = getopt(argc, argv, ":ht:p"))!= -1){
      switch((char)i){
      case 't':
        if(optarg != NULL){
          timeout = stoi(optarg);
          cout << "Timeout: "<< timeout << "s" << endl;
        }
        else
          help();
        break;
      case 'p':
          print_all = true;
          break;
      case 'h':
          help();
          break;
      case '?':
          help();
      }
    }
    string CPUNAME = get_cpu_name();
    int NUMCORES = sysconf(_SC_NPROCESSORS_ONLN);
    if(CPUNAME != ""){
        cout << "Model name:      " << CPUNAME << endl;
    } else {
        cerr << "Couldn't get CPU model name" << endl;
    }
    cout << "Number of CPUs:  " << NUMCORES << endl;
    cout << endl;

    bool monitor = true;
    string tempFilePath = get_temp_path();
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
        stressThreads[i] = thread(stress, &run);
    }
    cout << "Started!" << endl << endl;

    int min_temp = numeric_limits<int>::max();
    int max_temp = 0;
    auto start = time(NULL);
    while(run){
        now = time(nullptr);
        tm_info = localtime(&now);
        strftime(timestring, 100, "%Y-%m-%d %H:%M:%S", tm_info);
        if(monitor){
            int temp = get_temp(tempFilePath);
            min_temp = min(min_temp, temp);
            max_temp = max(max_temp, temp); 
            if(print_all){
              cout << "[" << timestring << "] ";
            }
            cout << "CPU Temperature: "<< temp << "°C [MIN : " << min_temp << "°C | MAX : " << max_temp << "°C]" << endl;
        }
        elapsed = now - start;
        if( timeout > 0 && elapsed >= timeout )
          break;
        if(!print_all){
          cout << "Time elapsed: " << elapsed << " seconds. " << "CTRL + C to exit..." << endl;
        }
        sleep(1);
        if(!print_all){
          cout << "\r\033[A";
          if(monitor){
              cout << "\033[A";
          }
       }
    }

    exit(EXIT_SUCCESS);
}
