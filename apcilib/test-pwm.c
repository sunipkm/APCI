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

#include "apcilib.h"

#define IM_ARRAYSIZE(x) \
    (sizeof(x) / sizeof(x[0]))

#define _Nullable

volatile sig_atomic_t done = 0;
void sig_handler(int sig)
{
    done = 1;
}

int apci_write8_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned char data)
{
    int status = 0;
    printf("%s... (Dev [%lu] Bar [%d] Reg [0x%x] Value [0x%02x])\t", msg, device_index, bar, offset, data);
    status = apci_write8(fd, device_index, bar, offset, data);
    if (status)
    {
        printf("Errno: %d, Error: %s\n\n", errno, strerror(errno));
    }
    else
    {
        printf("Success\n\n");
    }
    return status;
}

int apci_write16_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned short data)
{
    int status = 0;
    printf("%s... (Dev [%lu] Bar [%d] Reg [0x%x] Value [0x%04x])\t", msg, device_index, bar, offset, data);
    status = apci_write16(fd, device_index, bar, offset, data);
    if (status)
    {
        printf("Errno: %d, Error: %s\n\n", errno, strerror(errno));
    }
    else
    {
        printf("Success\n\n");
    }
    return status;
}

int apci_write32_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned int data)
{
    int status = 0;
    printf("%s... (Dev [%lu] Bar [%d] Reg [0x%x] Value [0x%08x])\t", msg, device_index, bar, offset, data);
    status = apci_write32(fd, device_index, bar, offset, data);
    if (status)
    {
        printf("Errno: %d, Error: %s\n\n", errno, strerror(errno));
    }
    else
    {
        printf("Success\n\n");
    }
    return status;
}

int apci_read8_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned char * _Nullable data)
{
    int status = 0;
    unsigned char _data = 0xff;
    printf("%s... (Dev [%lu] Bar [%d] Reg [0x%x])\t", msg, device_index, bar, offset);
    status = apci_read8(fd, device_index, bar, offset, &_data);
    if (status)
    {
        printf("Errno: %d, Error: %s\n\n", errno, strerror(errno));
    }
    else
    {
        if (data != NULL)
            *data = _data;
        printf("Value: 0x%02x\n\n", _data);
    }
    return status;
}

int apci_read16_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned short * _Nullable data)
{
    int status = 0;
    unsigned short _data = 0xff;
    printf("%s... (Dev [%lu] Bar [%d] Reg [0x%x])\t", msg, device_index, bar, offset);
    status = apci_read16(fd, device_index, bar, offset, &_data);
    if (status)
    {
        printf("Errno: %d, Error: %s\n\n", errno, strerror(errno));
    }
    else
    {
        if (data != NULL)
            *data = _data;
        printf("Value: 0x%04x\n\n", _data);
    }
    return status;
}

int apci_read32_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned int * _Nullable data)
{
    int status = 0;
    unsigned int _data = 0xff;
    printf("%s... (Dev [%lu] Bar [%d] Reg [0x%x])\t", msg, device_index, bar, offset);
    status = apci_read32(fd, device_index, bar, offset, &_data);
    if (status)
    {
        printf("Errno: %d, Error: %s\n\n", errno, strerror(errno));
    }
    else
    {
        if (data != NULL)
            *data = _data;
        printf("Value: 0x%08x\n\n", _data);
    }
    return status;
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

    int fet_idx = 0;                      // bit offset 8
    int reg_ofst = 0x100 * (fet_idx + 9); // bit 8 + 1
    const uint8_t turn_all_off = 0x9b;
    const uint8_t turn_on_b = turn_all_off ^ 0x2; // clear bit 2 to set PORT B to output
    const unsigned long dev_idx = 1;
    const int dio_ctrl = 0x3;
    const int bar_reg = 1; // somehow bar needs to be 1 for this

    int status = -1;

    status = apci_write8_debug("Resetting FPGA", fd, dev_idx, bar_reg, 0xfc, 0x4);

    if (status) // exit because could not write to register
    {
        if (errno == EFAULT)
            printf("Bad address! Check bar register.\n\n");
        goto cleanup;
    }

    sleep(1);

    status = apci_write8_debug("Setting port B to output", fd, dev_idx, bar_reg, dio_ctrl, turn_on_b);
    sleep(1);

    status = apci_read8_debug("Reading DIO control status", fd, dev_idx, bar_reg, dio_ctrl, NULL);

    status = apci_read8_debug("Reading port B register", fd, dev_idx, bar_reg, 0x1, NULL);
    printf("Provide input to continue...");
    getchar();

    status = apci_write8_debug("Turning on channel 0 in port B", fd, dev_idx, bar_reg, 0x1, 0xfe); // active low
    printf("Provide input to continue...");
    getchar();

    status = apci_write8_debug("Turning off all channels in port B", fd, dev_idx, bar_reg, 0x1, 0xff);
    printf("Provide input to continue...");
    getchar();

    status = apci_write16_debug("Setting PWM LOW", fd, dev_idx, bar_reg, reg_ofst + 0x20, 0x7fff); // 0x7fff * 8ns
    status = apci_write16_debug("Setting PWM HIGH", fd, dev_idx, bar_reg, reg_ofst + 0x24, 0xffff); // 0xffff * 8 ns

    printf("Provide input to turn on PWM...");
    getchar();

    status = apci_write8_debug("Turning on PWM", fd, dev_idx, bar_reg, reg_ofst + 0x10, 0x6); // PWM bit to high, Pgo to high
    status = apci_write32_debug("Setting GO", fd, dev_idx, bar_reg, 0x50, 1 << (fet_idx + 8)); // write to GO
    printf("Provide input to turn off PWM...");
    getchar();

    status = apci_write8_debug("Turning off PWM", fd, dev_idx, bar_reg, reg_ofst + 0x10, 0x0); // PWM bit to low, Pgo to high
    status = apci_write32_debug("Setting GO", fd, dev_idx, bar_reg, 0x50, 0x0); // write to GO
    status = apci_write8_debug("Turning off all FETs", fd, dev_idx, bar_reg, 0x1, 0xff); // active low

    printf("Provide input to turn all ports to INPUT...");
    getchar();

    status = apci_write8_debug("Setting port B to input", fd, dev_idx, bar_reg, dio_ctrl, turn_all_off);
    printf("Provide input to continue...");
    getchar();

    status = apci_read8_debug("Reading DIO control", fd, dev_idx, bar_reg, dio_ctrl, NULL);

cleanup:
    close(fd);
}