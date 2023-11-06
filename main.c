#include <qubitas/utils.h>
#include <qubitas/uart.h>
#include <qubitas/crc.h>
#include <qubitas/printk.h>
#include <qubitas/gpio.h>

int main()
{
    usart_init();
    crc_init();
    button_init();
    char str[] = "\r\n\r\nQubitas Bootloader @Copyright 2023.10.10\r\n\r\n";
    usart_send_data((u8 *)str, sizeof(str));
    while(1)
    {
        usart_receive_data();
    }
}