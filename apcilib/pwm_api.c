/**
 * @file pwm_api.c
 * @author Sunip K. Mukherjee (sunipkmukherjee@gmail.com)
 * @brief PWM on PORT B API impl
 * @version 0.1
 * @date 2023-04-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

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

#include "pwm_api.h"

#if PWM_NO_USE_DEBUG == 0
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
#else
int apci_write8_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned char data)
{
    return apci_write8(fd, device_index, bar, offset, data);
}

int apci_write16_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned short data)
{
    return apci_write16(fd, device_index, bar, offset, data);
}

int apci_write32_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned int data)
{
    return apci_write32(fd, device_index, bar, offset, data);
}

int apci_read8_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned char * _Nullable data)
{
    unsigned char _data = 0xff;
    int status = apci_read8(fd, device_index, bar, offset, &_data);
    if (data)
        *data = _data;
    return status;
}

int apci_read16_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned short * _Nullable data)
{
    unsigned short _data = 0xff;
    int status = apci_read16(fd, device_index, bar, offset, &_data);
    if (data)
        *data = _data;
    return status;
}

int apci_read32_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned int * _Nullable data)
{
    unsigned int _data = 0xff;
    int status = apci_read32(fd, device_index, bar, offset, &_data);
    if (data)
        *data = _data;
    return status;
}
#endif

int reset_device(int fd)
{
    return apci_write8_debug("Resetting FPGA", fd, 1, 1, 0xfc, 0x4);
}

int portb_set_output(int fd)
{
    const uint8_t turn_all_off = 0x9b;
    const uint8_t turn_on_b = turn_all_off ^ 0x2; // clear bit 2 to set PORT B to output
    return apci_write8_debug("Setting port B to output", fd, 1, 1, 0x3, turn_on_b);
}

int portb_set_input(int fd)
{
    const uint8_t turn_all_off = 0x9b;
    const uint8_t turn_on_b = turn_all_off ^ 0x2; // clear bit 2 to set PORT B to output
    return apci_write8_debug("Setting port B to input", fd, 1, 1, 0x3, turn_all_off);
}

int portb_stop_pwm(int fd, int fet_idx)
{
    if ((fet_idx < 0) || (fet_idx > 8))
        return -1;
    int status = -1;
    const uint8_t turn_all_off = 0x9b;
    const uint8_t turn_on_b = turn_all_off ^ 0x2; // clear bit 2 to set PORT B to output
    // 1. Check if port B is output and set it to output if needed
    uint8_t port_cfg = 0xff;
    status = apci_read8_debug("Reading port IO config", fd, 1, 1, 0x3, &port_cfg);
    if (port_cfg & 0x2)
    {
        return 0; // Port B is input
    }
    // 2. Disable PWM 
    int reg_ofst = 0x100 * (fet_idx + 9); // bit 8 + 1
    status = apci_write8_debug("Setting pin to PWM mode", fd, 1, 1, reg_ofst + 0x10, 0x0); // No pulse
    if (status)
        return status;
    uint8_t dio_mask = 1 << fet_idx;
    // check if port is already on
    uint8_t portb_dio = 0xff;
    status = apci_read8_debug("Reading port B DIO", fd, 1, 1, 0x1, &portb_dio); // get output settings
    if (status)
        return status;
    status = apci_write8_debug("Turning off port B DIO", fd, 1, 1, 0x1, portb_dio | dio_mask); // disable output (active LOW)
    if (status)
        return status;
    return 0;
}

int portb_update_pwm(int fd, int fet_idx, uint16_t on_time, uint16_t off_time)
{
    int status = -1;
    if ((fet_idx < 0) || (fet_idx > 8))
        return -1;
    status = portb_stop_pwm(fd, fet_idx);
    if (status)
        return status;
    int reg_ofst = 0x100 * (fet_idx + 9); // bit 8 + 1
    status = apci_write16_debug("Setting PWM LOW", fd, 1, 1, reg_ofst + 0x20, on_time); // 0x7fff * 8ns
    if (status)
        return status;
    status = apci_write16_debug("Setting PWM HIGH", fd, 1, 1, reg_ofst + 0x24, off_time); // 0xffff * 8 ns
    if (status)
        return status;
    status = apci_write8_debug("Setting pin to PWM mode", fd, 1, 1, reg_ofst + 0x10, 0x6); // high going pulse + pwm
    if (status)
        return status;
    return 0;
}

int portb_start_pwm(int fd, int fet_idx, uint16_t on_time, uint16_t off_time)
{
    if ((fet_idx < 0) || (fet_idx > 8))
        return -1;
    const uint8_t turn_all_off = 0x9b;
    const uint8_t turn_on_b = turn_all_off ^ 0x2; // clear bit 2 to set PORT B to output
    int status = -1;
    // 1. Check if port B is output and set it to output if needed
    uint8_t port_cfg = 0xff;
    status = apci_read8_debug("Reading port IO config", fd, 1, 1, 0x3, &port_cfg);
    if (port_cfg != turn_on_b)
    {
        status = portb_set_output(fd);
        if (status)
            return status;
    }
    // 2. Set frequency
    status = portb_update_pwm(fd, fet_idx, on_time, off_time);
    if (status)
        return status;
    // 3. Turn on the pin
    int go_mask = 1 << (fet_idx + 8);
    status = apci_write32_debug("Writing GO", fd, 1, 1, 0x50, go_mask);
    if (status)
        return status;
    return 0;
}

#ifdef _PWM_UNIT_TEST
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

    status = reset_device(fd); // apci_write8_debug("Resetting FPGA", fd, dev_idx, bar_reg, 0xfc, 0x4);

    if (status) // exit because could not write to register
    {
        if (errno == EFAULT)
            printf("Bad address! Check bar register.\n\n");
        goto cleanup;
    }

    sleep(1);

    status = portb_set_output(fd);
    sleep(1);

    status = apci_read8_debug("Reading DIO control status", fd, dev_idx, bar_reg, dio_ctrl, NULL);

    status = apci_read8_debug("Reading port B register", fd, dev_idx, bar_reg, 0x1, NULL);
    printf("Provide input to continue...");
    getchar();

    status = apci_write8_debug("Turning on channel 0 in port B", fd, dev_idx, bar_reg, 0x1, 0xfe); // active low
    printf("Provide input to continue...");
    getchar();

    status = apci_write8_debug("Turning off all channels in port B", fd, dev_idx, bar_reg, 0x1, 0xff);
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

    status = portb_start_pwm(fd, fet_idx, 0x7fff, 0xffff);

    printf("Provide input to turn OFF PWM...");
    getchar();

    status = portb_stop_pwm(fd, fet_idx);

    printf("Provide input to turn all ports to INPUT...");
    getchar();

    status = portb_set_input(fd);
    printf("Provide input to continue...");
    getchar();

    status = apci_read8_debug("Reading DIO control", fd, dev_idx, bar_reg, dio_ctrl, NULL);

cleanup:
    close(fd);
}
#endif