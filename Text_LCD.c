/*
 * Text_LCD.c
 *
 * Created: 2026-07-01 오전 10:51:29
 *  Author: kccistc
 */ 
#include "Text_LCD.h"

void LCD_pulse_enable(void)
{
	PORT_CONTROL |= 1<<E_PIN;  //E를 high
	_delay_us(50);
	PORT_CONTROL &= ~(1<<E_PIN); // E를 LOW로
	_delay_us(50);
}

void LCD_write_data(uint8_t data)
{
	PORT_CONTROL |= 1<<RS_PIN;
	
	if(MODE==4)
	{
		 PORT_DATA = (PORT_DATA & 0x0F) | (data & 0xF0);
		 LCD_pulse_enable();
		 
		 PORT_DATA = (PORT_DATA & 0x0F) | ((data << 4) & 0xF0);
		 LCD_pulse_enable();
	}
	_delay_ms(2);
}

void LCD_write_command(uint8_t command)
{
	PORT_CONTROL &= ~(1<<RS_PIN);
	
	if(MODE==4)
	{
        PORT_DATA = (PORT_DATA & 0x0F) | (command & 0xF0);
        LCD_pulse_enable();
        
        PORT_DATA = (PORT_DATA & 0x0F) | ((command << 4) & 0xF0);
        LCD_pulse_enable();
	}
	_delay_ms(2);
}
void LCD_clear(void)
{
	LCD_write_command(COMMAND_CLEAR_DISPLAY);
	_delay_ms(2);
	
	
}
void LCD_init(void)
{
	_delay_ms(50); // 전원 인가 후 LCD 내부 초기화 대기 (40ms 이상 필요)
	
	DDR_DATA |= 0xF0;   // PB4~7 출력 설정
	PORT_DATA = 0x00;
	
	   DDR_CONTROL |= (1<<RS_PIN) | (1<<RW_PIN) | (1<<E_PIN); // PC0~2 출력 설정
	   PORT_CONTROL &= ~(1<<RW_PIN); // R/W = 0 (쓰기 모드)

	if(MODE == 4)
	{
		// [중요] 4비트 모드 전환을 위한 표준 초기화 시퀀스
		
		// 1. 0x03 전송 (4비트 모드 지시) - 3번 반복
        PORT_DATA = (PORT_DATA  & 0x0F) | 0x30;
        LCD_pulse_enable();
        _delay_ms(5); // 4.1ms 이상 대기
        
        PORT_DATA = (PORT_DATA  & 0x0F) | 0x30;
        LCD_pulse_enable();
        _delay_ms(1); // 100us 이상 대기
        
        PORT_DATA = (PORT_DATA  & 0x0F) | 0x30;
        LCD_pulse_enable();
        _delay_ms(1);
        
        PORT_DATA = (PORT_DATA  & 0x0F) | 0x20;
        LCD_pulse_enable();
        _delay_ms(1);
		
		// 이제부터 4비트 모드 명령어 전송 가능
		LCD_write_command(COMMAND_4_BIT_MODE); // 0x28 (2라인, 5x8 도트, 4비트)
	}
	
	// Display ON, Cursor OFF, Blink OFF (0x0C)
	uint8_t command = 0x08 | (1<<COMMAND_DISPLAY_ON_OFF_BIT);
	LCD_write_command(command);
	LCD_clear();
	// Entry Mode Set: 커서 오른쪽 이동, 화면 스크롤 없음 (0x06)
	LCD_write_command(0x06);
	
}
void LCD_write_string(char *string)
{
	uint8_t i;
	for(i= 0; string[i];i++)
		LCD_write_data(string[i]);	
}
void LCD_goto_XY(uint8_t row, uint8_t col)
{
	col %=16;
	row %=2;
	
	uint8_t address = (0x40 *row) + col;
	uint8_t command = 0x80+address;
	
	LCD_write_command(command);
}