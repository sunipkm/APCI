/**
 * @file pwm_api.h
 * @author Sunip K. Mukherjee (sunipkmukherjee@gmail.com)
 * @brief PWM on PORT B API
 * @version 0.1
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

/**
 * @brief Set this to 0 to get register level debugging.
 * 
 */
#define PWM_NO_USE_DEBUG 1

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

int apci_write8_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned char data);

int apci_write16_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned short data);

int apci_write32_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned int data);

int apci_read8_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned char * _Nullable data);

int apci_read16_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned short * _Nullable data);

int apci_read32_debug(const char *msg, int fd, unsigned long device_index, int bar, int offset, unsigned int * _Nullable data);

#ifdef __cplusplus
}
#endif
#endif  // __PWM_API_H__
