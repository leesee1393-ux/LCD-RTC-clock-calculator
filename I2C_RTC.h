/*
 * I2C_RTC.h
 *
 * Created: 2026-07-01 오후 02:23:15
 *  Author: kccistc
 */ 


#ifndef I2C_RTC_H_
#define I2C_RTC_H_
#include <avr/io.h>


#define F_CPU 16000000UL
#define SLAVE_ADDR_DS1307 0x68
#define I2C_FREQ     100000UL  /* 100 kHz */
#define TWBR_VALUE   ((F_CPU / I2C_FREQ - 16) / 2)
#define I2C_SCL PD0
#define I2C_SDA PD1

#define SLAVE_ADDR   0x50      /* 7-bit Slave 주소 */



void I2C_init(void);
void I2C_start(void);
void I2C_transmit(uint8_t data);
uint8_t I2C_receive_NACK(void);
uint8_t I2C_receive_ACK(void);
void I2C_stop(void);

// DS1307 시간,date 읽기 함수 추가 선언 
void RTC_write_datetime(uint8_t *buf);
void RTC_set_date(uint8_t year, uint8_t month, uint8_t date);
void RTC_set_time(uint8_t hour, uint8_t min, uint8_t sec);
void RTC_read_datetime(uint8_t *buf);
void RTC_main(void);



// BCD <-> 10진수 변환 함수 선언




#endif /* I2C_RTC_H_ */