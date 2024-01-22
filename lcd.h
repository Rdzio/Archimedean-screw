#ifndef lcd
#define lcd

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/i2c.h"
#include "pico/binary_info.h"

#define MAX_LINES      2
#define MAX_CHARS      16

void i2c_write_byte(uint8_t val);

void lcd_toggle_enable(uint8_t val);

void lcd_send_byte(uint8_t val, int mode);

void lcd_clear(void);

/*! \brief Set cursor in given postion 
 *  \ingroup lcd
 *
 * \param line line (0-1)
 * \param position positopon (0-15) 
 */
void lcd_set_cursor(int line, int position);

static void inline lcd_char(char val);

/*! \brief Display text on the lcd
 *  \ingroup lcd
 *
 * \param s text
 */
void lcd_string(const char *s);

/*! \brief Initialise I2C and LCD HW
 *  \ingroup lcd
 *
 */
void lcd_init();

#endif