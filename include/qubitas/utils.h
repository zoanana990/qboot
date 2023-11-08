#ifndef __UTIL_H__
#define __UTIL_H__

#define NULL                ((void *) 0)
#define TRUE                (1)
#define FALSE               (0)
#define MASK(bit)           (~(~0U << (bit)))

#define INT_MAX             (0x7fffffff)
#define INT_MIN             (0x80000000)

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
#define INFO                COLOR_YELLOW "[INFO] "
#define WARNING             COLOR_RED "[WARNING] "
#define DEBUG               COLOR_GREEN "[DEBUG] "


int atoi(const char *nptr);

#endif