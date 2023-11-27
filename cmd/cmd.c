#include <qubitas/cmd.h>
#include <qubitas/type.h>
#include <qubitas/utils.h>
#include <qubitas/printk.h>
#include <qubitas/string.h>
#include <mm/mm.h>

#define MAX_TOKEN_COUNT             (10)

static s8 cmd_recv_buff[CMD_RCV_MAX_LEN];
static s8 *ptr_rcv_buff = cmd_recv_buff;

extern struct cmd cmd_table [];

void cmd_splitStrings(char *str, char *tokens[], int *count, const char *delimiter) {
    char *token = strtok(str, delimiter);

    *count = 0;
    while(token != NULL) {
        tokens[(*count)++] = token;
        token = strtok(NULL, delimiter);
    }
}

/* function to execute the callback base on the command line */
void cmd_process(void) {

    s8 exe_command[CMD_RCV_MAX_LEN];
    s8 *tokens[MAX_TOKEN_COUNT];
    s32 token_count;

    s32 n = cmd_getTableSize();

    strncpy(exe_command, cmd_recv_buff, sizeof(cmd_recv_buff));

    /* 1. split the command into tokens */
    cmd_splitStrings(exe_command, tokens, &token_count, " ");

    /* 2. Find the command in the table */
    for(int i = 0; i < n; i++) {
        if(strcmp(tokens[0], cmd_table[i].cmd) == 0) {
            if (token_count - 1 == cmd_table[i].argc) {
                cmd_table[i].callback(token_count - 1, tokens + 1);
            } else {
                pr_cmd("Argument count error for command, please call help\n");
            }
            return;
        }
    }

    pr_cmd("Command not found, please call help\r\n");
}
#if 1
/**
 * Arrow key:
 * UP: 0x1b 0x5b 0x42
 * DOWN: 0x1b 0x5b 0x42
 * RIGHT: 0x1b 0x5b 0x43
 * LEFT: 0x1b 0x5b 0x44
 * */
int cmd_detectArrowKey() {
    if (ptr_rcv_buff - cmd_recv_buff < 3)
        return -1;
    s8 *ptr_prev = ptr_rcv_buff - 1;

    if(*ptr_prev != 0x5b)
        return -1;
    ptr_prev--;

    if(*ptr_prev != 0x1b)
        return -1;

    switch(*ptr_rcv_buff) {
        case 'A':
            ptr_rcv_buff -= 3;
            memset(ptr_rcv_buff, 0x0, 3);
            break;
        case 'B':
            ptr_rcv_buff -= 3;
            memset(ptr_rcv_buff, 0x0, 3);
            break;
        case 'C':
            ptr_rcv_buff -= 3;
            memset(ptr_rcv_buff, 0x0, 3);
            break;
        case 'D':
            ptr_rcv_buff -= 3;
            memset(ptr_rcv_buff, 0x0, 3);
            break;
        default:
            ptr_rcv_buff -= 3;
            memset(ptr_rcv_buff, 0x0, 3);
            break;
    }
    return 0;
}
#endif
void cmd_receive(s8 data) {

    if (!data)
        return;

    switch (data) {
        /* detect the arrow key */
        case 'A':
        case 'B':
        case 'C':
        case 'D':
            if(cmd_detectArrowKey())
                goto put;
            break;
        case '\r':
            printk("\r\n");
            if(cmd_recv_buff == ptr_rcv_buff)
                goto put_char;

            /* Need to process the command */
            cmd_process();

            /* clear the buffer */
            memset(cmd_recv_buff, 0x00, CMD_RCV_MAX_LEN);
        put_char:
            ptr_rcv_buff = cmd_recv_buff;
            /* put the $ character */
            put_c('$');
            put_c(' ');
            break;

        case '\b':
            if(ptr_rcv_buff - cmd_recv_buff) {
                put_c(data);
                put_c(' ');
                put_c(data);
                ptr_rcv_buff--;
                *ptr_rcv_buff = '\0';
            }
            break;
        default:
        put:
            if (ptr_rcv_buff - cmd_recv_buff < CMD_RCV_MAX_LEN) {
                *ptr_rcv_buff = data;
                ptr_rcv_buff++;
                put_c(data);
            }
            break;
    }
}