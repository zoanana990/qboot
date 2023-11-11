# FreeRTOS

This document is used to study how [FreeRTOS](https://github.com/FreeRTOS) works, trace the code, and try to write a
simplified version

# Entry point

`main()` function:

```c
void main( void )
{
    status = xTaskCreate(task1_handler, 
                         "Task-1", 
                         200, 
                         "Hello world from Task-1", 
                         2, 
                         &task1_handle);

    /* Start the scheduler. */
    vTaskStartScheduler();

    while(1);
}
```

There are two APIs in the main function

1. `xTaskCreate`
2. `vTaskStartScheduler`

## `xTaskCreate`

### Code Flow:

- Call prvCreateTask
    - if success: put the task to ready list
    - fail: return error code (could not allocate required memory, -1)

### Function prototype and variable declaration

```c
BaseType_t xTaskCreate( TaskFunction_t pxTaskCode,
                    const char * const pcName,
                    const configSTACK_DEPTH_TYPE usStackDepth,
                    void * const pvParameters,
                    UBaseType_t uxPriority,
                    TaskHandle_t * const pxCreatedTask )
/* Sample function call */                    
status = xTaskCreate(task1_handler, 
                     "Task-1", 
                     200, 
                     "Hello world from Task-1", 
                     2, 
                     &task1_handle);
```

`TaskFunction_t` is declared as following:

```c
typedef void (* TaskFunction_t)( void * );
```

It is just a void function pointer, in main function, the sample function is shown as follows:

```c
static void task1_handler(void* parameters)
{
	while(1)
	{
		snprintf(msg,100,"%s\n", (char*)parameters);
		taskYIELD();
	}
}
```

Notice that, the unit of stack size is `unsigned long`, thus the stack size is `200 * 4 bytes`.

Let's see the `TaskHandle_t` definition

```c
typedef struct tskTaskControlBlock         * TaskHandle_t;


```

## `prvCreateTask`

### Code Flow