#include "stresslib.h"

using namespace std;

int main(){

    bool run = true;
    string CPUNAME = getCpuName();
    int NUMCORES = sysconf(_SC_NPROCESSORS_ONLN);
    cout << "Model name:      " << CPUNAME << endl;
    cout << "Number of CPUs:  " << NUMCORES << endl;
    cout << endl;

    bool monitor = true;
    if(getTemp() == -1){
        char answ;
        cout << "Failed to open temperature file." << endl;
        cout << "Run stress test without temperature monitor? [Y/n] :";
        cin >> answ;
        if(answ == 'n' || answ == 'N'){
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
            int temp = getTemp();
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