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
#include <signal.h>

#include "pwm_api.h"

#define IM_ARRAYSIZE(x) \
    (sizeof(x) / sizeof(x[0]))

volatile sig_atomic_t done = 0;
void sig_handler(int sig)
{
    done = 1;
}

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

    int status = -1;

    status = reset_device(fd); // apci_write8_debug("Resetting FPGA", fd, dev_idx, bar_reg, 0xfc, 0x4);

    if (status) // exit because could not write to register
    {
        if (errno == EFAULT)
            printf("Bad address! Check bar register.\n\n");
        goto cleanup;
    }

    // printf("Provide input to continue...");
    // getchar();

    // status = apci_write16_debug("Setting PWM LOW", fd, dev_idx, bar_reg, reg_ofst + 0x20, 0x7fff); // 0x7fff * 8ns
    // status = apci_write16_debug("Setting PWM HIGH", fd, dev_idx, bar_reg, reg_ofst + 0x24, 0xffff); // 0xffff * 8 ns

    // printf("Provide input to turn on PWM...");
    // getchar();

    // status = apci_write8_debug("Turning on PWM", fd, dev_idx, bar_reg, reg_ofst + 0x10, 0x6); // PWM bit to high, Pgo to high
    // status = apci_write32_debug("Setting GO", fd, dev_idx, bar_reg, 0x50, 1 << (fet_idx + 8)); // write to GO
    // printf("Provide input to turn off PWM...");
    // getchar();

    // status = apci_read8_debug("Reading OUTPUT status", fd, dev_idx, bar_reg, 0x1, NULL); // read DIO control to check whether PWM is indeed on
    // status = apci_write8_debug("Turning off PWM", fd, dev_idx, bar_reg, reg_ofst + 0x10, 0x0); // PWM bit to low, Pgo to high
    // status = apci_write8_debug("Turning off all FETs", fd, dev_idx, bar_reg, 0x1, 0xff); // active low

    printf("Provide input to turn ON PWM...");
    getchar();

    status = portb_start_pwm(fd, 0, 0x7fff, 0xffff);

    printf("Provide input to turn OFF PWM...");
    getchar();

    status = portb_stop_pwm(fd, 0);

    printf("Provide input to turn all ports to INPUT...");
    getchar();

    status = portb_set_input(fd);
cleanup:
    close(fd);
}