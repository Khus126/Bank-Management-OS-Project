# Bank Account Management with Concurrent Transactions

A beginner-friendly Operating Systems project developed using C and Linux.

## Project Overview

This project simulates a simple banking system where multiple child processes perform deposit and withdrawal operations on a shared bank account balance.

The project contains two implementations:

- **With Semaphore:** Uses process synchronization to protect the shared account balance.
- **Without Semaphore:** Demonstrates race conditions when multiple processes access shared data without synchronization.

## Technologies Used

- C Programming
- Linux / Ubuntu
- GCC Compiler
- System V Shared Memory
- POSIX Semaphores

## Operating System Concepts

- Process creation using `fork()`
- Parent and child processes
- Inter-Process Communication (IPC)
- Shared memory
- Race conditions
- Critical sections
- Process synchronization
- Semaphores
- Process waiting using `wait()`

## Project Structure

```text
Bank-Management/
├── with_semaphore.c
├── without_semaphore.c
├── README.md
└── .gitignore
```

## Compilation and Execution

### Without Semaphore

```bash
gcc without_semaphore.c -o bank_without
./bank_without
```

### With Semaphore

```bash
gcc with_semaphore.c -o bank_with -pthread
./bank_with
```

## Example Test

Initial Balance: Rs. 10,000

- Deposit: Rs. 2,000
- Withdrawal: Rs. 3,000
- Expected Final Balance: Rs. 9,000

Without semaphore, concurrent transactions may produce an incorrect balance due to race conditions.

With semaphore, the critical section is protected and the balance is updated correctly.

*Note: Race-condition outcomes depend on process scheduling and may vary between executions.*

## Project Objective

To understand how multiple processes access shared data and why process synchronization is necessary to prevent race conditions.

## Academic Project

Developed as part of the Operating Systems Laboratory.