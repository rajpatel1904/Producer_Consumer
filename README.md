# Producer-Consumer with Shared Memory in C++

## 📝 Project Overview

This project implements the classic **Producer-Consumer** problem using **shared memory** for inter-process communication (IPC) in C++. The producer process generates random data items and writes them into a shared buffer, while the consumer process reads and processes these items. This approach simulates real-time data exchange between two processes with synchronized access to a shared resource.

## 🚀 Key Features

- **Shared Memory**: Utilizes shared memory (`mmap()`) for inter-process communication to share data between the producer and consumer processes.
- **Circular Buffer**: Implements a circular buffer mechanism where the producer writes and the consumer reads in a synchronized way, preventing buffer overflow and underflow.
- **Random Data Generation**: The producer generates random integers and writes them into the shared memory buffer.
- **Synchronization**: The producer and consumer processes are synchronized using buffer indices to prevent simultaneous read/write conflicts.

## 💻 Technical Details

- **Forking Processes**: The producer process forks a child consumer process using `fork()`.
- **Shared Memory**: Shared memory is allocated using `shm_open()` and mapped using `mmap()` to allow both processes to access the same memory space.
- **Buffer Management**: A circular buffer with two pointers (`in` and `out`) tracks where the producer writes and where the consumer reads. The buffer size and item count are configurable through command-line arguments.
- **Random Item Generation**: The producer generates items with random values between 0 and 3000 using the `rand()` function and writes them to the buffer.
- **Consumer Process**: The consumer reads items from the buffer in a FIFO (First-In-First-Out) order and processes them until all items are consumed.

## 🗂 Files

- **` producer.c++`**: Contains the producer logic for generating random items and writing them to the shared memory buffer.
- **` consumer.c++`**: Contains the consumer logic for reading and processing items from the shared memory buffer.
- **`Makefile`**: *(Optional)* A Makefile can simplify the build process.

## ⚠️ Error Handling

- The program checks for errors during shared memory creation, mapping, and deletion.
- If shared memory or the fork process fails, the program will output an error message and terminate.

## 🌱 Future Improvements

- **Semaphore-based Synchronization**: Implement semaphores or mutexes for better synchronization between the producer and consumer processes.
- **Multiple Producers/Consumers**: Extend the project to support multiple producer and consumer processes interacting with the same buffer.
- **Timeout Mechanism**: Add a timeout mechanism to handle cases where the consumer or producer might hang.
