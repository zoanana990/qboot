# Contex Switch

source code: [test_main.c](../test_code/test_main.c)

## Introduction
- In context switch target, i wil implement a scheduler which schedule multiple user tasks in a round-robin fashion 
  by carrying out the context switch operation
- Round-robin scheduling method is, time slices are assigned to each task equal portions and in circular order
- First will use systick handler to carry out the context switch operation between multiple tasks
- Later will we change the code using pendSV handler

## What is a task?
- A task is nothing but a piece of code, or you can all it a `C` function, which does a specific job when it is allowed
  to run on the CPU
- A task has its own stack to reate its local variables when it runs on the CPU. Also when scheduler decides to remove a task from CPU,
  scheduler first saves the context of the task in task's private stack
- So, in summary, a piece of code or a function is called a task when it is schedulable and never lose its 'state' unless it is deleted permanently

## In this test code...
- We wil create 4 user tasks (never returning C functions)

## Stack pointer selection
