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
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <cstring>

using namespace std;

#define SHM_SIZE 4096
void* gShmPtr;

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

int main() {
    const char *name = "OS_HW1_Raj";
    int bufSize, itemCnt, in, out;

    // Open shared memory block
    int shm_fd = shm_open(name, O_RDWR, 0666);
    if (shm_fd == -1) {
        cerr << "Shared memory failed to open" << endl;
        exit(1);
    }

    // Map shared memory
    gShmPtr = mmap(0, SHM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
    if (gShmPtr == MAP_FAILED) {
        cerr << "Map failed" << endl;
        exit(1);
    }

    // Read shared memory header values
    bufSize = GetBufSize();
    itemCnt = GetItemCnt();
    in = GetIn();
    out = GetOut();

    cout << "Consumer reading: bufSize = " << bufSize << endl;

    // Consume all produced items
    for (int i = 0; i < itemCnt; ++i) {
        int val = ReadAtBufIndex(out);
        cout << "Consuming Item " << i << " with value " << val << " at Index " << out << endl;
        out = (out + 1) % bufSize;
        SetOut(out);
    }

    // Remove shared memory
    if (shm_unlink(name) == -1) {
        cerr << "Error removing " << name << endl;
        exit(1);
    }

    return 0;
}

void SetIn(int val) {
    SetHeaderVal(2, val);
}

void SetOut(int val) {
    SetHeaderVal(3, val);
}

void SetHeaderVal(int i, int val) {
    void* ptr = static_cast<char*>(gShmPtr) + i * sizeof(int);
    memcpy(ptr, &val, sizeof(int));
}

int GetBufSize() {
    return GetHeaderVal(0);
}

int GetItemCnt() {
    return GetHeaderVal(1);
}

int GetIn() {
    return GetHeaderVal(2);
}

int GetOut() {
    return GetHeaderVal(3);
}

int GetHeaderVal(int i) {
    int val;
    void* ptr = static_cast<char*>(gShmPtr) + i * sizeof(int);
    memcpy(&val, ptr, sizeof(int));
    return val;
}

void WriteAtBufIndex(int indx, int val) {
    void* ptr = static_cast<char*>(gShmPtr) + 4 * sizeof(int) + indx * sizeof(int);
    memcpy(ptr, &val, sizeof(int));
}

int ReadAtBufIndex(int indx) {
    int val;
    void* ptr = static_cast<char*>(gShmPtr) + 4 * sizeof(int) + indx * sizeof(int);
    memcpy(&val, ptr, sizeof(int));
    return val;
}

int GetRand(int x, int y) {
    return x + rand() % (y - x + 1);
}
