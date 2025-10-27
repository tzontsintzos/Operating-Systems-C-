# Pizzeria Order & Delivery System

A multi-threaded simulation of a pizzeria ordering and delivery system implemented in C using POSIX threads. This project demonstrates concurrent programming concepts, resource management, and thread synchronization.

## Authors

- Ioannis Tsintzos (3200211)
- Anastasia Gardeli (3200025)
- Konstantinos Mitsanas (3200103)

**Course:** Operating Systems - Spring Semester 2023

## Overview

This project simulates a pizzeria that handles multiple concurrent customer orders with limited resources (cooks, ovens, packers, and deliverers). Each customer order is processed by a separate thread, and resource access is synchronized using mutexes and condition variables to prevent race conditions.

## Technologies Used

- **Language:** C
- **Threading:** POSIX Threads (pthread)
- **Synchronization:** Mutexes and Condition Variables
- **Timing:** `clock_gettime()` for precise time measurements
- **Random Number Generation:** Thread-safe `rand_r()`

## Repository Structure

```
Operating-Systems-C-/
├── p3200211-p3200025-p3200103-pizzeria/
│   ├── p3200211-p3200025-p3200103.c       # Main source code (495 lines)
│   ├── p3200211-p3200025-p3200103.h       # Header file with configurations
│   ├── p3200211-p3200025-p3200103.pdf     # Project documentation (Greek)
│   ├── test-res1.sh                       # Compilation and test script
│   └── a.out                              # Compiled executable
└── README.md                               # This file
```

## System Architecture

### Resource Configuration

The pizzeria has the following limited resources:
- **Cooks:** 2
- **Ovens:** 15
- **Packers:** 2
- **Deliverers:** 10

### Order Processing Pipeline

Each customer order goes through the following stages:

1. **Payment Processing**
   - Duration: 1-3 seconds
   - Failure probability: 10%
   - Failed orders are rejected

2. **Order Preparation (Cook)**
   - Customers order 1-3 random pizzas
   - 60% probability for plain pizza (€10)
   - 40% probability for special pizza (€12)
   - Preparation time: 1 second per pizza

3. **Baking (Oven)**
   - Duration: 10 seconds
   - Requires oven availability

4. **Packing (Packer)**
   - Duration: 1 second per pizza
   - Requires packer availability

5. **Delivery (Deliverer)**
   - Duration: 5-15 seconds (random)
   - Requires deliverer availability

### Thread Synchronization

- One thread per customer order
- Resource pools managed by counter variables
- Mutexes protect access to shared resources
- Condition variables signal resource availability
- Threads block when resources are unavailable and wake when signaled

## Compilation and Execution

### Compile

```bash
gcc -Wall -pthread p3200211-p3200025-p3200103.c -o pizzeria
```

Or use the provided test script:

```bash
cd p3200211-p3200025-p3200103-pizzeria
bash test-res1.sh
```

### Run

```bash
./pizzeria <number_of_customers> <random_seed>
```

**Parameters:**
- `number_of_customers`: Total number of customer orders to simulate (positive integer)
- `random_seed`: Seed for random number generation (positive integer)

**Example:**

```bash
./pizzeria 100 1000
```

This simulates 100 customer orders with random seed 1000.

## Output Statistics

The program tracks and displays the following statistics:

- **Order Times:**
  - Average order completion time
  - Maximum order completion time

- **Delivery Times:**
  - Average delivery time
  - Maximum delivery time

- **Order Success:**
  - Number of successful orders
  - Number of unsuccessful orders (payment failures)

- **Pizza Statistics:**
  - Total plain pizzas ordered
  - Total special pizzas ordered

- **Revenue:**
  - Total income generated

## Key Features

- Concurrent order processing using POSIX threads
- Thread-safe resource management with mutexes
- Realistic simulation with configurable timing parameters
- Payment failure simulation
- Comprehensive performance statistics
- Race condition prevention through proper synchronization
- Scalable design supporting configurable resource limits

## Implementation Details

### Main Files

**p3200211-p3200025-p3200103.c:**
- `pizzeria()`: Main thread function handling complete order lifecycle
- `charge_customer()`: Payment processing with failure probability
- `main()`: Initialization, thread creation, and statistics reporting

**p3200211-p3200025-p3200103.h:**
- Resource limits and timing constants
- Pricing and probability configurations
- Mutex and condition variable declarations
- Helper function prototypes

## Learning Objectives

This project demonstrates:
- Multi-threaded programming in C
- POSIX thread API (pthread_create, pthread_join)
- Mutex locks for critical sections
- Condition variables for thread synchronization
- Resource pool management patterns
- Deadlock prevention
- Performance measurement and analysis

## License

Academic project for educational purposes.
