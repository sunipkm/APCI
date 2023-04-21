#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <linux/types.h>
#include <unistd.h>
#include <signal.h>
#include <time.h>
#include <errno.h>
#include <string.h>
#include <stdint.h>

#include "apcilib.h"

#define IM_ARRAYSIZE(x) \
    (sizeof(x) / sizeof(x[0]))

int main(int argc, char *argv[])
{
    if (argc != 2) // show usage and exit
    {
        printf("\n\nUsage:\n\n");
        printf("sudo %s /dev/apci/<APCI Device File>\n\n", argv[0]);
        exit(0);
    }

    int fd = open(argv[1], O_RDWR); // open file

    if (fd < 0) // error
    {
        printf("Could not open %s, check module/permissions. Error: %s\n\n", argv[1], strerror(errno));
        exit(0);
    }

    // read the requested registers
    uint16_t regs[] = {0x68, 0x6c, 0x70, 0x74};
    unsigned int dev_idx = 1;
    int dev_bar = 2;
    for (int i = 0; i < IM_ARRAYSIZE(regs); i++)
    {
        uint32_t res = 0xffffffff;
        int status = apci_read32(fd, dev_idx, dev_bar, regs[i], &res);
        printf("Device [%u] Bar [%d] Reg 0x%02x> 0x%08x (%d)\n", dev_idx, dev_bar, regs[i], res, status);
    }

cleanup:
    close(fd);    
}