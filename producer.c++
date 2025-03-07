/*
CSC139
Fall 2018
First Assignment
Last Name, First Name
Section #
OSs Tested on: such as Linux, Mac, etc.
*/

#include <iostream>
#include <cstdlib>
#include <cstring>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <sys/types.h>
#include <wait.h>

using namespace std;

#define SHM_SIZE 4096
void* gShmPtr;

void Producer(int, int, int);
void InitShm(int, int);
void SetBufSize(int);
void SetItemCnt(int);
void SetIn(int);
void SetOut(int);
void SetHeaderVal(int, int);
int GetBufSize();
int GetItemCnt();
int GetIn();
int GetOut();
int GetHeaderVal(int);
void WriteAtBufIndex(int, int);
int ReadAtBufIndex(int);
int GetRand(int, int);

int main(int argc, char* argv[]) {
    pid_t pid;
    int bufSize, itemCnt, randSeed;
    
    if (argc != 4) {
        cerr << "Invalid number of command-line arguments" << endl;
        exit(1);
    }
    
    bufSize = atoi(argv[1]);
    itemCnt = atoi(argv[2]);
    randSeed = atoi(argv[3]);

    if (bufSize <= 0 || itemCnt <= 0 || randSeed <= 0) {
        std::cerr << "Error: Buffer size, item count, and random seed must be positive integers.\n";
        return EXIT_FAILURE;
    }
    
    InitShm(bufSize, itemCnt);
    
    pid = fork();
    if (pid < 0) {
        cerr << "Fork Failed" << endl;
        exit(1);
    } else if (pid == 0) {
        cout << "Launching Consumer" << endl;
        execlp("./consumer", "consumer", NULL);
    } else {
        cout << "Starting Producer" << endl;
        Producer(bufSize, itemCnt, randSeed);
        cout << "Producer done and waiting for consumer" << endl;
        wait(NULL);
        cout << "Consumer Completed" << endl;
    }
    return 0;
}

void InitShm(int bufSize, int itemCnt) {
    int in = 0, out = 0;
    const char *name = "OS_HW1_Raj";
    int shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    ftruncate(shm_fd, SHM_SIZE);
    gShmPtr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    
    if (gShmPtr == MAP_FAILED) {
        cerr << "Shared memory mapping failed" << endl;
        exit(1);
    }
    
    SetBufSize(bufSize);
    SetItemCnt(itemCnt);
    SetIn(in);
    SetOut(out);
}

void Producer(int bufSize, int itemCnt, int randSeed) {
    srand(randSeed);
    
    for (int i = 0; i < itemCnt; i++) {
        int in = GetIn();
        int val = GetRand(0, 3000);
        WriteAtBufIndex(in, val);
        cout << "Producing Item " << i << " with value " << val << " at Index " << in << endl;
        SetIn((in + 1) % bufSize);
    }
    cout << "Producer Completed" << endl;
}

void SetBufSize(int val) { SetHeaderVal(0, val); }
void SetItemCnt(int val) { SetHeaderVal(1, val); }
void SetIn(int val) { SetHeaderVal(2, val); }
void SetOut(int val) { SetHeaderVal(3, val); }

void SetHeaderVal(int i, int val) {
    void* ptr = (char*)gShmPtr + i * sizeof(int);
    memcpy(ptr, &val, sizeof(int));
}

int GetBufSize() { return GetHeaderVal(0); }
int GetItemCnt() { return GetHeaderVal(1); }
int GetIn() { return GetHeaderVal(2); }
int GetOut() { return GetHeaderVal(3); }

int GetHeaderVal(int i) {
    int val;
    void* ptr = (char*)gShmPtr + i * sizeof(int);
    memcpy(&val, ptr, sizeof(int));
    return val;
}

void WriteAtBufIndex(int indx, int val) {
    void* ptr = (char*)gShmPtr + 4 * sizeof(int) + indx * sizeof(int);
    memcpy(ptr, &val, sizeof(int));
}

int ReadAtBufIndex(int indx) {
    int val;
    void* ptr = (char*)gShmPtr + 4 * sizeof(int) + indx * sizeof(int);
    memcpy(&val, ptr, sizeof(int));
    return val;
}

int GetRand(int x, int y) {
    return x + rand() % (y - x + 1);
}



