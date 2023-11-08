# Qubitas Bootloader

This is a simple bootloader for exercise

## Introduction

- The device we use is [STM32F746ZE](https://www.st.com/en/evaluation-tools/nucleo-f746zg.html)
- This project is refine
  the [bootloader course](https://www.udemy.com/course/stm32f4-arm-cortex-mx-custom-bootloader-development/learn/lecture/10026148#overview),
  simplied the code and support multi-devices

## Features
- [x] `printk`
- [ ] `scanf`: implement by DMA
  - [ ] usart interrupt
  - [ ] usart DMA
  - [ ] scanf
- [ ] context switch
- [ ] some important data structure
- [ ] malloc
- [ ] task and thread
  - [ ] task
  - [ ] thread
- [ ] scheduler
  - [ ] Round Robin
  - [ ] CFS