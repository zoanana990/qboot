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

```shell

```

## Directory architecture

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
- [ ] task and thread
  - [ ] task
  - [ ] thread
- [ ] scheduler
    - [ ] Round Robin

## Reference

- [task](https://www.codeinsideout.com/blog/stm32/task-scheduler/#run-the-first-task)
- [malloc](https://github.com/exsourcode/csapp/tree/master/code/vm)