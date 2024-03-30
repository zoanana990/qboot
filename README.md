# Qubitas Bootloader

```text
   ###    #     #  ######    #####   #######     #      #####
  #   #   #     #  #     #     #        #        #     #     #
 #     #  #     #  #     #     #        #       ###    #
 #     #  #     #  ######      #        #       # #     #####
 #     #  #     #  #     #     #        #      #####         #
  #   #   #     #  #     #     #        #      #   #   #     #
   ###     #####   ######    #####      #     ##   ##   #####
     ###     AN OPEN SOURCE TUTORIAL OPERATING SYSTEM IN 2023
```
This is a simple bootloader for exercise

## Introduction

- The device we use is [STM32F746ZE](https://www.st.com/en/evaluation-tools/nucleo-f746zg.html)
- This project is refine
  the [bootloader course](https://www.udemy.com/course/stm32f4-arm-cortex-mx-custom-bootloader-development/learn/lecture/10026148#overview),
  simplied the code and support multi-devices

## Getting Start

### Environment Setup
TODO

## Directory architecture
```txt
.
├── app                // architecture level test code
│    ├── cs            // prototype os
│    └── svc           // user space -> kernel space
├── arch               // for arm cm7 architecture
│    └── arm
│         └── init
├── boot               // for bootloader usage
├── build              // the compiled image
├── cmd                // command line usage, will move to app
├── doc                // development document
│    ├── FreeRTOS
│    ├── Qubitas
│    └── cortex_m7
│        └── img
├── driver             // driver code for STM32F746
├── include
│    ├── app
│    ├── arch
│    │    └── arm
│    ├── ds
│    ├── kernel
│    ├── mm
│    ├── net
│    └── qubitas
├── kernel             // kernel source code
├── lib                // some gcc code like printk, etc.
├── mk                 // sub-level makefile 
└── mm                 // memory management, e.g. heap, memset etc.
```

## Features
- [x] `printk`
- [ ] `scanf`: implement by DMA
    - [x] usart interrupt
    - [ ] usart DMA
    - [ ] scanf
- [x] command shell
- [ ] Timer interrupt
- [ ] context switch
  - [ ] Stack initialization
- [x] list api
- [x] memory management
  - [x] `malloc`
  - [x] `free`
- [x] task and thread
  - [x] task
- [x] scheduler
    - [x] Round Robin

## Reference
- [tenok](https://hackmd.io/@shengwen/tenok#VI-Linux-%E9%A2%A8%E6%A0%BC%E7%9A%84-Linked-list-%E5%AF%A6%E4%BD%9C)
- [task](https://www.codeinsideout.com/blog/stm32/task-scheduler/#run-the-first-task)
- [malloc](https://github.com/exsourcode/csapp/tree/master/code/vm)
