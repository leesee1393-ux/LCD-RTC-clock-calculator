/*
Text_LCD.h
*/
#ifndef TEXT_LCD_H_
#define TEXT_LCD_H_

#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/io.h>
#include <stdint.h>

#define PORT_DATA			PORTB
#define PORT_CONTROL		PORTC
#define DDR_DATA			DDRB
#define DDR_CONTROL			DDRC

#define RS_PIN				0
#define RW_PIN				1
#define E_PIN				2

#define COMMAND_CLEAR_DISPLAY 0x01 // 오타 수정 (DISPALY -> DISPLAY)
#define COMMAND_4_BIT_MODE	  0x28
#define COMMAND_DISPLAY_ON_OFF_BIT 2
#define COMMAND_CURSOR_ON_OFF_BIT 1
#define COMMAND_BLINK_ON_OFF_BIT 0

extern uint8_t MODE;

void LCD_pulse_enable(void);
void LCD_write_data(uint8_t data);
void LCD_write_command(uint8_t command);
void LCD_clear(void);
void LCD_init(void);
void LCD_write_string(char *string);	 // 오타 수정 (wirte -> write)
void LCD_goto_XY(uint8_t row, uint8_t col);

#endif /* TEXT_LCD_H_ */