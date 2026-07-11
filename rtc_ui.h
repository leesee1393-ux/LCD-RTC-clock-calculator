/*
 * rtc.h
 *
 * Created: 2026-07-01 오후 09:01:02
 *  Author: kccistc
 */ 


#ifndef RTC.UI_H_
#define RTC.UI_H_

#include <stdint.h>

// RTC 하위 모드 정의 (View 및 각 필드 설정 모드)
typedef enum {
	RTC_VIEW,
	RTC_SET_YEAR,
	RTC_SET_MONTH,
	RTC_SET_DATE,
	RTC_SET_HOUR,
	RTC_SET_MIN,
	RTC_SET_SEC
} rtc_sub_mode_t;

// 함수 선언
void rtc_ui_init(void);
void rtc_ui_update(void);
void rtc_ui_handle_buttons(uint8_t btn0, uint8_t btn1, uint8_t btn2, uint8_t btn3);



#endif /* RTC.UI_H_ */