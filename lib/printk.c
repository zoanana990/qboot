#include <qubitas/printk.h>
#include <qubitas/uart.h>

typedef void (*out_fct_type)(char character, void* buffer, size_t idx, size_t maxlen);

static inline void _out_char(char character, void *buffer, size_t idx, size_t max_len)
{
    if(character)
        usart_send_data(&character);
}

static int _vsnprintf(out_fct_type out, char* buffer, const size_t maxlen, const char* format, va_list va)
{
    return 0;
}

/**************************************
 * EXPORT API
 **************************************/
int printk(char *format, ...)
{
    va_list va;
    va_start(va, format);
    char buffer[1];
    const int ret = _vsnprintf(_out_char, buffer, -1, format, va);
    return 0;
}

int sprintk(char *buffer, char *format, ...)
{
    return 0;
}

int snprintk(char *buffer, size_t count, char *format, ...)
{
    return 0;
}