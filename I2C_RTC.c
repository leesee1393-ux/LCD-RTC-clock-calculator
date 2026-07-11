/*
 * I2C_RTC.c
 *
 * Created: 2026-07-01 오후 02:25:36
 *  Author: kccistc
 */ 
#include "I2C_RTC.h"
#include "utils.h" // 모듈화된 유틸리티 함수(bcd2dec, dec2bcd, bcd_to_str 등) 포함
#include <util/delay.h>
#include <avr/io.h>

// 전역 변수 (RTC_main에서 사용 중이므로 유지)
uint8_t dt[7]; 
char date_str[9]; 
char time_str[9]; 
char temp[3];

static void twi_wait(void)
{
	while (!(TWCR & (1 << TWINT)));
}

void I2C_init(void)
{
	TWSR=0x00;   
	TWBR = (uint8_t)TWBR_VALUE; 
	TWCR = (1 << TWEN); 
}

void I2C_start(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
	twi_wait();		
}

void I2C_transmit(uint8_t data)
{
	TWDR = data;
	TWCR = (1 << TWINT) | (1 << TWEN); 
	twi_wait();
}

void I2C_stop(void)
{
	TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
	_delay_us(10);
}

uint8_t I2C_receive_ACK(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
	twi_wait();
	return TWDR;
}

uint8_t I2C_receive_NACK(void)
{
	TWCR = (1 << TWINT) | (1 << TWEN);
	twi_wait();
	return TWDR;
}



// 날짜 설정 함수 (YY, MM, DD)
void RTC_set_date(uint8_t year, uint8_t month, uint8_t date)
{
	I2C_start();
	I2C_transmit(0xD0);      
	I2C_transmit(0x04);      
	I2C_transmit(dec2bcd(date)); // utils.h의 dec2bcd 사용
	I2C_transmit(dec2bcd(month));
	I2C_transmit(dec2bcd(year));
	I2C_stop();
}

// 시간 설정 함수 (HH, MM, SS)
void RTC_set_time(uint8_t hour, uint8_t min, uint8_t sec)
{
	I2C_start();
	I2C_transmit(0xD0);
	I2C_transmit(0x00);      
	I2C_transmit(dec2bcd(sec) & 0x7F);   
	I2C_transmit(dec2bcd(min));
	I2C_transmit(dec2bcd(hour) & 0x3F);  
	I2C_stop();
}

// DS1307에서 시간 읽기
void RTC_read_datetime(uint8_t *buf)
{
	I2C_start();
	I2C_transmit(0xD0);
	I2C_transmit(0x00);      
	I2C_start();             
	I2C_transmit(0xD1);      
	
	buf[0] = I2C_receive_ACK();  
	buf[1] = I2C_receive_ACK();  
	buf[2] = I2C_receive_ACK();  
	buf[3] = I2C_receive_ACK();  
	buf[4] = I2C_receive_ACK();  
	buf[5] = I2C_receive_ACK();  
	buf[6] = I2C_receive_NACK(); 
	I2C_stop();
}


void RTC_main(void)
{
   RTC_read_datetime(dt);
   
   uint8_t sec   = bcd2dec(dt[0] & 0x7F);
   uint8_t min   = bcd2dec(dt[1]);
   uint8_t hour  = bcd2dec(dt[2] & 0x3F);
   uint8_t date  = bcd2dec(dt[4]);
   uint8_t month = bcd2dec(dt[5]);
   uint8_t year  = bcd2dec(dt[6]);

   bcd_to_str(dt[6], temp);
   date_str[0] = temp[0]; date_str[1] = temp[1];
   date_str[2] = '-';
   bcd_to_str(dt[5], temp);
   date_str[3] = temp[0]; date_str[4] = temp[1];
   date_str[5] = '-';
   bcd_to_str(dt[4], temp);
   date_str[6] = temp[0]; date_str[7] = temp[1];
   date_str[8] = '\0';
   
   bcd_to_str(dt[2], temp);
   time_str[0] = temp[0]; time_str[1] = temp[1];
   time_str[2] = '-';
   bcd_to_str(dt[1], temp);
   time_str[3] = temp[0]; time_str[4] = temp[1];
   time_str[5] = '-';
   bcd_to_str(dt[0], temp);
   time_str[6] = temp[0]; time_str[7] = temp[1];
   time_str[8] = '\0';

   LCD_goto_XY(0, 0);
   LCD_write_string(date_str);
   LCD_goto_XY(1, 0);
   LCD_write_string(time_str);

   _delay_ms(100); 
}
void RTC_write_datetime(uint8_t *buf)
{
	I2C_start();
	I2C_transmit(0xD0);      // Write Address
	I2C_transmit(0x00);      // Seconds register부터 쓰기
	
	I2C_transmit(buf[0] & 0x7F); // CH bit = 0 (오실레이터 작동)
	I2C_transmit(buf[1]);        // Minutes
	I2C_transmit(buf[2] & 0x3F); // Hours (24시간 모드)
	I2C_transmit(buf[3]);        // Day
	I2C_transmit(buf[4]);        // Date
	I2C_transmit(buf[5] & 0x1F); // Month
	I2C_transmit(buf[6]);        // Year
	I2C_stop();
}