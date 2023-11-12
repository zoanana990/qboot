#ifndef __CMD_H__
#define __CMD_H__

#include <qubitas/type.h>

#define CMD_RCV_MAX_LEN              (128UL)

typedef void(*cmd_callback_t)(int argc, char *argv[]);

struct cmd {
    const char * const  cmd;
    const char * const  description;
    cmd_callback_t      callback;
    s8                  argc;
};

void cmd_receive(s8 data);
u32 cmd_getTableSize();
#endif