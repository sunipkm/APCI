/**
 * @file pwm_api.h
 * @author Sunip K. Mukherjee (sunipkmukherjee@gmail.com)
 * @brief PWM Control API for mPCIe-DIO-24A with mPCIe-IDIO-8 driver board.
 * PORT B of the mPCIe-DIO-24A maps to the 8 FETs on the mPCIe-IDIO-8 driver board.
 * @version 1.0
 * @date 2023-04-26
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#ifndef __PWM_API_H__

#include <stdint.h>

#ifndef _Nullable
/**
 * @brief Indicate a pointer can be NULL.
 * 
 */
#define _Nullable
#endif

#ifndef PWM_NO_USE_DEBUG
/**
 * @brief Set this to 0 to get register level debugging (-DPWM_NO_USE_DEBUG=0).
 * 
 */
#define PWM_NO_USE_DEBUG 1
#endif

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Reset a device.
 * 
 * @param fd Device descriptor.
 * @return int 0 on success, -1 on failure.
 */
int reset_device(int fd);

/**
 * @brief Set PORT B to output.
 * 
 * @param fd Device descriptor.
 * @return int 0 on success, -1 on failure.
 */
int portb_set_output(int fd);

/**
 * @brief Set PORT B to input.
 * 
 * @param fd Device descriptor.
 * @return int 0 on success, -1 on failure.
 */
int portb_set_input(int fd);

/**
 * @brief Turn off any existing output.
 * 
 * @param fd Device descriptor. 
 * @param fet_idx FET index [0 -- 7].
 * @return int 0 on success, -1 on failure.
 */
int portb_stop_pwm(int fd, int fet_idx);

/**
 * @brief Update PORT B DIO PWM frequency. This will turn off any existing output.
 * 
 * @param fd Device descriptor
 * @param fet_idx DIO index [0 -- 7]
 * @param on_time On time of PWM pulse. Duration = on_time * 8 ns
 * @param off_time Off time of PWM pulse. Duration = off_time * 8 ns
 * @return int 0 for success, -1 for failure.
 */
int portb_update_pwm(int fd, int fet_idx, uint16_t on_time, uint16_t off_time);

/**
 * @brief Set port B to output if not set, turn off any existing output, set PWM pulse profile and turn on the pulse.
 * 
 * @param fd Device descriptor
 * @param fet_idx DIO index [0 -- 7]
 * @param on_time On time of PWM pulse. Duration = on_time * 8 ns
 * @param off_time Off time of PWM pulse. Duration = off_time * 8 ns
 * @return int 0 for success, -1 for failure.
 */
int portb_start_pwm(int fd, int fet_idx, uint16_t on_time, uint16_t off_time);

/**
 * @brief Set the clock divider for the PWM. 
 * The PWM clock is 125 MHz, and the divider is 8 bits wide. 
 * The PWM clock is divided by `clkdiv`. 
 * 
 * clkdiv = 0 means the PWM clock is divided by 256. 
 * clkdiv = 1 means the PWM clock is not divided.
 * clkdiv = [2 -- 255] means the PWM clock is divided by [2 -- 255].
 * 
 * @param fd Device descriptor
 * @param clkdiv Clock divider
 * @return int 0 for success, -1 for failure.
 */
int set_clkdiv(int fd, uint8_t clkdiv);

/**
 * @brief Get the current clock divider set on the PWM.
 * Read {@link set_clkdiv} for more information.
 * 
 * @param fd Device descriptor
 * @param clkdiv Pointer to clock divider storage.
 * @return int 0 for success, -1 for failure.
 */
int get_clkdiv(int fd, uint8_t *clkdiv);

/**
 * @brief Write 8 bits to a device register.
 * 
 * @param msg Debug message to print if -DPWM_NO_USE_DEBUG=0 is passed to the compiler.
 * @param fd Device descriptor
 * @param device_index Device index (usually 1)
 * @param bar BAR register (usually 1)
 * @param offset Register offset (register address)
 * @param data 8-bits of data to write
 * @return int 0 for success, -1 for failure.
 */
int apci_write8_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned char data);

/**
 * @brief Write 16 bits to a device register.
 * 
 * @param msg Debug message to print if -DPWM_NO_USE_DEBUG=0 is passed to the compiler.
 * @param fd Device descriptor
 * @param device_index Device index (usually 1)
 * @param bar BAR register (usually 1)
 * @param offset Register offset (register address)
 * @param data 16-bits of data to write
 * @return int 0 for success, -1 for failure.
 */
int apci_write16_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned short data);

/**
 * @brief Write 32 bits to a device register.
 * 
 * @param msg Debug message to print if -DPWM_NO_USE_DEBUG=0 is passed to the compiler.
 * @param fd Device descriptor
 * @param device_index Device index (usually 1)
 * @param bar BAR register (usually 1)
 * @param offset Register offset (register address)
 * @param data 32-bits of data to write
 * @return int 0 for success, -1 for failure.
 */
int apci_write32_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned int data);

/**
 * @brief Read 8 bits from a device register.
 * 
 * @param msg Debug message to print if -DPWM_NO_USE_DEBUG=0 is passed to the compiler.
 * @param fd Device descriptor
 * @param device_index Device index (usually 1)
 * @param bar BAR register (usually 1)
 * @param offset Register offset (register address)
 * @param data Pointer to 8-bits of data to read into. Can be NULL.
 * @return int 0 for success, -1 for failure.
 */
int apci_read8_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned char * _Nullable data);

/**
 * @brief Read 16 bits from a device register.
 * 
 * @param msg Debug message to print if -DPWM_NO_USE_DEBUG=0 is passed to the compiler.
 * @param fd Device descriptor
 * @param device_index Device index (usually 1)
 * @param bar BAR register (usually 1)
 * @param offset Register offset (register address)
 * @param data Pointer to 16-bits of data to read into. Can be NULL.
 * @return int 0 for success, -1 for failure.
 */
int apci_read16_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned short * _Nullable data);

/**
 * @brief Read 32 bits from a device register.
 * 
 * @param msg Debug message to print if -DPWM_NO_USE_DEBUG=0 is passed to the compiler.
 * @param fd Device descriptor
 * @param device_index Device index (usually 1)
 * @param bar BAR register (usually 1)
 * @param offset Register offset (register address)
 * @param data Pointer to 32-bits of data to read into. Can be NULL.
 * @return int 0 for success, -1 for failure.
 */
int apci_read32_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned int * _Nullable data);

#ifdef __cplusplus
}
#endif
#endif  // __PWM_API_H__
