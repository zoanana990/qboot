/**
 * In this code, we will write a program to execute an SVC instruction from thread mode,
 * Implement the SVC handler to print the SVC number used
 * Also, increment the SVC number by 4 and return it to the thread mode code and print it
 *
 * Step
 * 1. Write a main function where you should execute the SVC instruction with an argument
 * 2. Implement a SVC handler
 * 3. In the SVC handler, extract the SVC number and print it using printf
 * 4. Implement the SVC handler to deal with add, subtract, multiply, division
 * */

#include <app/app.h>
#include <qubitas/printk.h>
#include <qubitas/type.h>

s32 add(s32 x, s32 y) {
    s32 res;
    __asm__ __vo("SVC #36");
    __asm__("mov R0, %0" : "=r" (res)::);
    return res;
}

s32 sub(s32 x, s32 y) {
    s32 res;
    __asm__ __vo("SVC #37");
    __asm__("mov R0, %0" : "=r" (res)::);
    return res;
}

s32 mul(s32 x, s32 y) {
    s32 res;
    __asm__ __vo("SVC #38");
    __asm__("mov R0, %0" : "=r" (res)::);
    return res;
}

s32 div(s32 x, s32 y) {
    s32 res;
    __asm__ __vo("SVC #39");
    __asm__("mov R0, %0" : "=r" (res)::);
    return res;
}

/* TODO: let the cmd_process do out of interrupt controller */
void svc_main(void) {
    s32 res;
    printk("SVC test start!\r\n");

    res = add(10, 25);
    printk("add(10, 25) = %d\r\n", res);

    res = sub(10, 25);
    printk("sub(10, 25) = %d\r\n", res);

    res = mul(10, 25);
    printk("mul(10, 25) = %d\r\n", res);

    res = div(10, 25);
    printk("div(10, 25) = %d\r\n", res);
    while(1);
}

#if 0
__attribute__((naked))void SVC_Handler(void) {
    /* 1. get the value of MSP
     * Note: MRS: move destination, special register
     * */
    __asm__("MRS R0, MSP");
    __asm__("B _SVC_Handler");
}

void _SVC_Handler(u32 *ptr_base_stack_frame) {

    printk("In kernel mode\r\n");

    u8 *ptr_return_addr = (u8 *)ptr_base_stack_frame[6];

    /* R0 is ptr_base_stack_frame */
    printk("ptr_base_stack_frame = %p, data = %#x\r\n", ptr_base_stack_frame, *ptr_base_stack_frame);
    printk("ptr_return_addr = %p, data = %#x\r\n", ptr_return_addr, *ptr_return_addr);

    /* 2. Decremenet the return address by 2 to point to opcode of the SVC instruction in the program memory */
    ptr_return_addr -= 2;
    printk("ptr_return_addr = %p, data = %#x\r\n", ptr_return_addr, *ptr_return_addr);

    u8 svc_number = *ptr_return_addr;
    printk("svc_number = %d\r\n", svc_number);

    s32 x, y, res;
    x = ptr_base_stack_frame[0];
    y = ptr_base_stack_frame[1];

    printk("x = %d, y = %d\r\n", x, y);

    switch(svc_number) {
        case 36:
            /* add */
            res = x + y;
            printk("res = %d\r\n", res);
            ptr_base_stack_frame[0] = res;
            break;
        case 37:
            /* sub */
            res = x - y;
            printk("res = %d\r\n", res);
            ptr_base_stack_frame[0] = res;
            break;
        case 38:
            /* mul */
            res = x * y;
            printk("res = %d\r\n", res);
            ptr_base_stack_frame[0] = res;
            break;
        case 39:
            /* div */
            res = x / y;
            printk("res = %d\r\n", res);
            ptr_base_stack_frame[0] = res;
            break;
        default:
            printk("return to the user space\r\n");
    }
}
#endif