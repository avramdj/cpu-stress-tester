#include <sys/sysinfo.h>
#include <pthread.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#define READ_BUF (8)
#define SLEEP_TM (250000) //microseconds
#define STOI_2(buf) ((buf[0]-'0')*10 + buf[1]-'0')
#define STOI_3(buf) ((buf[0]-'0')*100 + (buf[1]-'0')*10 + buf[2]-'0')
#define assert(cond) osAssert(cond, __LINE__)

void osAssert(bool cond, int LINE);
void* threadLoop(void* arg);
void signalStop(int signum);
char* get_cpuname();
int get_cpucores();
void assemblyStress(bool* run);

typedef struct{
    int tid;
} t_arg;

bool run;

int main(int argc, char** argv){

    int NUMCORES = get_nprocs();
    char* CPUNAME = get_cpuname();
    int CPUCORES = get_cpucores();
    assert(CPUNAME != NULL);
    printf("CPU model: %s", CPUNAME);
    printf("Number of CPU cores/threads: %d/%d\n\n", CPUCORES, NUMCORES);

    printf("Starting stress test...\n");
    run = true;
    assert(signal(SIGINT, signalStop) != SIG_ERR);
    int NUMTHREADS = NUMCORES;
    pthread_t* tids = malloc(sizeof(pthread_t)*NUMTHREADS);
    t_arg* args = malloc(sizeof(t_arg)*NUMTHREADS);
    for(int i = 0; i < NUMTHREADS; i++){
        args[i].tid = i+1;
        pthread_create(&tids[i], NULL, threadLoop, &args[i]);
    }
    printf("Started %d threads! Torturing CPU...\n", NUMTHREADS);

    int tempFd = open("/sys/class/thermal/thermal_zone0/temp", O_RDONLY);
    assert(tempFd != -1);
    char buf[READ_BUF];
    int len;
    time_t start = time(NULL);
    printf("\n");
    int max = 0;
    int min = __INT_MAX__;
    while(run){
        assert((len = read(tempFd, buf, READ_BUF)) != -1);
        assert(lseek(tempFd, 0, SEEK_SET) != -1);
        int tmp = len == 6 ? STOI_2(buf) : STOI_3(buf);
        max = tmp > max ? tmp : max;
        min = min > tmp ? tmp : min;
        printf("CPU Temperature: %d°C [MIN : %d°C | MAX : %d°C]\n", tmp, min, max);
        printf("Time elapsed: %d seconds\n", (int)(time(NULL)-start));
        printf("Press CTRL+C to exit...");
        fflush(stdout);
        printf("\r\033[A\033[A");
        usleep(SLEEP_TM);
    }
    printf("\n\n\nCleaning up...\n");
    close(tempFd);
    for(int i = 0; i < NUMTHREADS; i++){
        pthread_join(tids[i], (void**)0);
    }

    exit(EXIT_SUCCESS);
}
char* get_cpuname(){
    FILE* cmd = fopen("/proc/cpuinfo", "r");
    assert(cmd != NULL);
    char* buffer = malloc(sizeof(char)*BUFSIZ);
    while(fgets(buffer, BUFSIZ, cmd) != NULL){
        if(!strncmp(buffer, "model name", 10)){
            buffer = strchr(buffer, ':');
            assert(buffer != NULL);
            buffer += 2;
            break;
        }
    }
    return buffer;
}
int get_cpucores(){
    FILE* cmd = fopen("/proc/cpuinfo", "r");
    assert(cmd != NULL);
    char* buffer = malloc(sizeof(char)*BUFSIZ);
    while(fgets(buffer, BUFSIZ, cmd) != NULL){
        if(!strncmp(buffer, "cpu cores", 9)){
            buffer = strchr(buffer, ':');
            assert(buffer != NULL);
            buffer += 2;
            break;
        }
    }
    int numcores = atoi(buffer);
    return numcores;
}
void signalStop(int signum){
    run = false;
}
void* threadLoop(void* arg){
    int tid = ((t_arg*)arg)->tid;
    float n = 1.5;
    assemblyStress(&run);
    return (void*)0;
}
void osAssert(bool cond, int LINE){
    if(!cond){
        fprintf(stderr, "Error on line %d", LINE);
        exit(EXIT_FAILURE);
    }
}