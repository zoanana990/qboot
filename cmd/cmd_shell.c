#include <qubitas/cmd.h>
#include <qubitas/printk.h>
#include <qubitas/io.h>
#include <qubitas/string.h>
#include <qubitas/utils.h>
#include <mm/heap.h>
#include <app/app.h>

void help_cmd(int argc, char *argv[]);
void memrl_cmd(int argc, char *argv[]);
void memwl_cmd(int argc, char *argv[]);

void wenwen_cmd(int argc, char *argv[]);

void heap_test_cmd(int argc, char *argv[]);

void svc_test_cmd(int argc, char *argv[]);

struct cmd cmd_table [] = {
        {"help",  "help, show all support commands\r\n",                       help_cmd,      0},
        {"memrl", "memrl <addr>, read the memory address\r\n",                 memrl_cmd,     1},
        {"memwl", "memwl <addr> <data>, write the data to memory address\r\n", memwl_cmd,     2},
        {"mtest", "mtest, test the heap behavior\r\n",                         heap_test_cmd, 0},
        {"wen",   "wen, wenwen character\r\n",                                 wenwen_cmd,    1},
        {"svc",   "svc, test code\r\n",                                        svc_test_cmd,  0},
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
    u32 addr = strtoul(argv[0], NULL, 16);
    printk("DATA: %#x\r\n", io_read(addr));
}

void memwl_cmd(int argc, char *argv[]) {
    u32 addr = strtoul(argv[0], NULL, 16);
    u32 data = strtoul(argv[1], NULL, 16);
    io_write(addr, data);
    printk("Done\r\n");
}

void heap_test_cmd(int argc, char *argv[]) {
    printk("Malloc test start\n");
    int *segment1 = kalloc(sizeof(int) * 10);
    printk("segment 1 = %p\n", segment1);
    int *segment2 = kalloc(sizeof(int) * 10);
    printk("segment 2 = %p\n", segment2);
    int *segment3 = kalloc(sizeof(int) * 10);
    printk("segment 3 = %p\n", segment3);
    kfree(segment2);
    kfree(segment1);
    kfree(segment3);
    int *segment4 = kalloc(sizeof(int) * 20);
    printk("segment 4 = %p\n", segment4);
    int *segment5 = kalloc(sizeof(int) * 10);
    printk("segment 5 = %p\n", segment5);
    int *segment6 = kalloc(sizeof(int) * 20);
    printk("segment 6 = %p\n", segment6);
    kfree(segment4);
    kfree(segment5);
    int *segment7 = kalloc(sizeof(int) * 20);
    printk("segment 7 = %p\n", segment7);
    kfree(segment6);
    kfree(segment7);
    printk("Malloc test complete\n");
}

void svc_test_cmd(int argc, char *argv[]) {
    svc_main();
}

void wenwen_cmd(int argc, char *argv[]) {
    /* This is a funny command */
    if (strcmp(argv[0], "pig") == 0) {
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
