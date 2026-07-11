/*
 * utils.h
 *
 * Created: 2026-07-01 오후 07:55:43
 *  Author: kccistc
 */ 


#ifndef UTILS_H_
#define UTILS_H_
#include <stdint.h>

uint8_t bcd2dec(uint8_t data);
uint8_t dec2bcd(uint8_t data);
void bcd_to_str(uint8_t bcd, char *str);

// [수정] 커서 기반 스크롤 출력 함수 선언
void update_calc_scroll_lcd(char *buf, uint8_t len, uint8_t cursor);



#endif /* UTILS_H_ */