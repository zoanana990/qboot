#include <utils.h>
#include <uart.h>
#include <crc.h>

int main()
{
    usart_init();
    crc_init();
    char str[] = "\r\nQubitas Bootloader @Copyright 2023.10.10\r\n";
    usart_send_data((u8 *)str, sizeof(str));
    while(1)
    {
        usart_receive_data();
    }
}