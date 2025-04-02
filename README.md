# IPC Job Processing System - Unit Testing Guide

## 📌 Overview
This repository contains a C++ implementation of an **Inter-Process Communication (IPC)** based job processing system. The system uses **pipes** for communication between a **main process (job manager)** and multiple **worker processes**. This guide explains how to set up, compile, and run unit tests for the system using **Google Test (gtest)**.

---

## 🛠 Setup Instructions
### 1️⃣ Install Google Test (gtest)
Before running the tests, install **gtest** on your system:
```bash
sudo apt update && sudo apt install libgtest-dev
```

Then, compile the gtest source files:
```bash
cd /usr/src/gtest
sudo cmake CMakeLists.txt
sudo make
sudo cp lib/*.a /usr/lib
```

---

## 🏗 Compilation & Execution
### 2️⃣ Compile the Test Suite
Run the following command to compile the test suite:
```bash
g++ -std=c++17 -lgtest -lgtest_main -pthread -o test_ipc test_ipc.cpp
```

### 3️⃣ Run the Tests
Execute the test binary:
```bash
./test_ipc
```

---

## ✅ Test Cases
This test suite ensures that:
| **Test Name**                        | **Description**                                      |
|--------------------------------------|------------------------------------------------------|
| WorkerReceivesAndProcessesJob       | Ensures a worker receives and completes a job       |
| MultipleJobsAssignedCorrectly       | Ensures jobs are assigned and processed correctly   |
| SelectHandlesMultipleWorkers        | Tests non-blocking read handling using `select()`  |
| CleanupWorksProperly                | Ensures proper cleanup of worker processes          |

If all tests pass, you should see output similar to:
```
[==========] Running 4 tests from 1 test suite.
[----------] Global test environment set-up.
[----------] 4 tests from IPCJobProcessingTest
[ RUN      ] WorkerReceivesAndProcessesJob
[       OK ] WorkerReceivesAndProcessesJob (1001 ms)
[ RUN      ] MultipleJobsAssignedCorrectly
[       OK ] MultipleJobsAssignedCorrectly (3002 ms)
[ RUN      ] SelectHandlesMultipleWorkers
[       OK ] SelectHandlesMultipleWorkers (2001 ms)
[ RUN      ] CleanupWorksProperly
[       OK ] CleanupWorksProperly (0 ms)
[----------] 4 tests from IPCJobProcessingTest (6004 ms total)
[==========] 4 tests passed. (6004 ms total)
[  PASSED  ] 4 tests.
```

---

## 🚀 Next Steps
- 🛠 **Extend Tests**: Add more worker scalability tests.
- 📈 **Performance Benchmarking**: Measure job processing efficiency.
- 🔄 **Error Handling Tests**: Simulate failures for robustness.

**Contributions & Suggestions Welcome!** 

Aditya Agrawal
123adiagrawal@gmail.com

