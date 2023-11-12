#include <qubitas/cmd.h>
#include <qubitas/printk.h>
#include <qubitas/io.h>
#include <qubitas/string.h>

void help_cmd(int argc, char *argv[]);
void memrl_cmd(int argc, char *argv[]);
void memwl_cmd(int argc, char *argv[]);
void wenwen_cmd(int argc, char *argv[]);

struct cmd cmd_table [] = {
    {"help", "help, show all support commands\r\n", help_cmd, 0},
    {"memrl", "memrl <addr>, read the memory address\r\n", memrl_cmd, 1},
    {"memwl", "memwl <addr> <data>, write the data to memory address\r\n", memwl_cmd, 2},
    {"wen", "wen, wenwen character\r\n", wenwen_cmd, 1},
};

u32 cmd_getTableSize() {
    return sizeof(cmd_table) / sizeof(cmd_table[0]);
}

void help_cmd(int argc, char *argv[]) {
    int n = cmd_getTableSize();
    pr_info("%-10s %-60s\r\n", "Command", "Description");
    for (int i = 0; i < n; i++) {
        pr_info("%-10s %-60s\r", cmd_table[i].cmd, cmd_table[i].description);
    }
}

void memrl_cmd(int argc, char *argv[]) {

}

void memwl_cmd(int argc, char *argv[]) {

}

void wenwen_cmd(int argc, char *argv[]){
    /* This is a funny command */
    if(strcmp(argv[0], "pig") == 0) {
        printk(" ##   ##  #######  ##   ##  ##   ##  #######  ##   ##   ####     #####   ######    ####      ####\r\n");
        printk(" ##   ##   ##   #  ###  ##  ##   ##   ##   #  ###  ##    ##     ##   ##   ##  ##    ##      ##  ##\r\n");
        printk(" ##   ##   ## #    #### ##  ##   ##   ## #    #### ##    ##     #         ##  ##    ##     ##\r\n");
        printk(" ## # ##   ####    ## ####  ## # ##   ####    ## ####    ##      #####    #####     ##     ##\r\n");
        printk(" #######   ## #    ##  ###  #######   ## #    ##  ###    ##          ##   ##        ##     ##  ###\r\n");
        printk(" ### ###   ##   #  ##   ##  ### ###   ##   #  ##   ##    ##     ##   ##   ##        ##      ##  ##\r\n");
        printk(" ##   ##  #######  ##   ##  ##   ##  #######  ##   ##   ####     #####   ####      ####      #####\r\n");
    } else if(strcmp(argv[0], "maid") == 0) {
        printk("                                                                                _|                                                _|        _|\r\n");
        printk("_|      _|      _|    _|_|    _|_|_|    _|      _|      _|    _|_|    _|_|_|          _|_|_|    _|_|_|  _|_|_|  _|_|      _|_|_|        _|_|_|\r\n");
        printk("_|      _|      _|  _|_|_|_|  _|    _|  _|      _|      _|  _|_|_|_|  _|    _|  _|  _|_|      _|    _|  _|    _|    _|  _|    _|  _|  _|    _|\r\n");
        printk("  _|  _|  _|  _|    _|        _|    _|    _|  _|  _|  _|    _|        _|    _|  _|      _|_|  _|    _|  _|    _|    _|  _|    _|  _|  _|    _|\r\n");
        printk("    _|      _|        _|_|_|  _|    _|      _|      _|        _|_|_|  _|    _|  _|  _|_|_|      _|_|_|  _|    _|    _|    _|_|_|  _|    _|_|_|\r\n");
    }
}
