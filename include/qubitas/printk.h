#ifndef __PRINTK_H__
#define __PRINTK_H__

#include <qubitas/type.h>

void put_c(char c);

int printk(char *format, ...);

int sprintk(char *buffer, char *format, ...);

int snprintk(char *buffer, size_t count, char *format, ...);


#define COLOR_BLACK         "\033[0;30;41m"
#define COLOR_RED           "\033[0;31m"
#define COLOR_GREEN         "\033[0;32m"
#define COLOR_YELLOW        "\033[0;33m"
#define COLOR_BLUE          "\033[0;34m"
#define COLOR_MAGENTA       "\033[0;35m"
#define COLOR_CYAN          "\033[0;36m"
#define COLOR_WHITE         "\033[0;37m"
#define COLOR_DEFAULT       "\033[0;39m"

#define DEFAULT             COLOR_WHITE
#define INFO                COLOR_DEFAULT  "[   INFO   ] "
#define ERROR               COLOR_RED      "[   ERROR  ] "
#define EXCEPTION           COLOR_CYAN     "[   EXCP   ] "
#define INTERRUPT           COLOR_MAGENTA  "[   INTR   ] "
#define CHECK_OK            COLOR_GREEN    "[    OK    ] "

#define pr_info(s, ...)     printk(INFO s, ##__VA_ARGS__)
#define pr_err(s)           printk(ERROR "%s", s)
#define pr_excp(s)          printk(EXCEPTION "%s", s)
#define pr_ok(s)            printk(CHECK_OK "%s", s)

#endif