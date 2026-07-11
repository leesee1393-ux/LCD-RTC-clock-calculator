/*
 * rtc_ui.c
 *
 * Created: 2026-07-01 오후 09:01:25
 *  Author: kccistc
 */ 
#include "rtc_ui.h"
#include "I2C_RTC.h"
#include "Text_LCD.h"
#include "utils.h" // bcd2dec, dec2bcd, bcd_to_str
#include <util/delay.h>
#include <string.h>

// 내부 상태 변수 (외부에서 직접 접근 불가)
static uint8_t rtc_buf[7]; // [0]Sec, [1]Min, [2]Hour, [3]Day, [4]Date, [5]Month, [6]Year
static rtc_sub_mode_t sub_mode = RTC_VIEW;

// 현재 선택된 필드의 10진수 값 가져오기
static uint8_t get_field_val(void) {
	switch(sub_mode) {
		case RTC_SET_YEAR:  return bcd2dec(rtc_buf[6]);
		case RTC_SET_MONTH: return bcd2dec(rtc_buf[5]);
		case RTC_SET_DATE:  return bcd2dec(rtc_buf[4]);
		case RTC_SET_HOUR:  return bcd2dec(rtc_buf[2]);
		case RTC_SET_MIN:   return bcd2dec(rtc_buf[1]);
		case RTC_SET_SEC:   return bcd2dec(rtc_buf[0]);
		default: return 0;
	}
}

// 수정된 값을 BCD로 변환하여 버퍼에 저장
static void set_field_val(uint8_t val) {
	uint8_t bcd = dec2bcd(val);
	switch(sub_mode) {
		case RTC_SET_YEAR:  rtc_buf[6] = bcd; break;
		case RTC_SET_MONTH: rtc_buf[5] = bcd; break;
		case RTC_SET_DATE:  rtc_buf[4] = bcd; break;
		case RTC_SET_HOUR:  rtc_buf[2] = bcd & 0x3F; break; // 24시간 모드 유지
		case RTC_SET_MIN:   rtc_buf[1] = bcd; break;
		case RTC_SET_SEC:   rtc_buf[0] = bcd & 0x7F; break; // CH 비트 0 유지
		default: break;
	}
}

void rtc_ui_init(void) {
	RTC_read_datetime(rtc_buf); // 초기화 시 하드웨어에서 시간 읽어옴
}

void rtc_ui_update(void) {
	char date_str[9], time_str[9], temp[3];

	// VIEW 모드일 때만 하드웨어에서 실시간으로 읽어옴 (설정 모드는 로컬 버퍼 사용)
	if (sub_mode == RTC_VIEW) {
		RTC_read_datetime(rtc_buf);
	}

	// 문자열 포맷팅
	bcd_to_str(rtc_buf[6], temp); memcpy(&date_str[0], temp, 2); date_str[2] = '-';
	bcd_to_str(rtc_buf[5], temp); memcpy(&date_str[3], temp, 2); date_str[5] = '-';
	bcd_to_str(rtc_buf[4], temp); memcpy(&date_str[6], temp, 2); date_str[8] = '\0';

	bcd_to_str(rtc_buf[2], temp); memcpy(&time_str[0], temp, 2); time_str[2] = '-';
	bcd_to_str(rtc_buf[1], temp); memcpy(&time_str[3], temp, 2); time_str[5] = '-';
	bcd_to_str(rtc_buf[0], temp); memcpy(&time_str[6], temp, 2); time_str[8] = '\0';

	// LCD 출력
	LCD_goto_XY(0, 0); LCD_write_string(date_str);
	LCD_goto_XY(1, 0); LCD_write_string(time_str);

	// 설정 모드일 때 커서 깜빡임 활성화 및 위치 지정
	if (sub_mode != RTC_VIEW) {
		LCD_write_command(0x0F); // Display ON, Cursor ON, Blink ON
		
		uint8_t row = 0, col = 0;
		switch(sub_mode) {
			case RTC_SET_YEAR:  row=0; col=0; break; // YY
			case RTC_SET_MONTH: row=0; col=3; break; // MM
			case RTC_SET_DATE:  row=0; col=6; break; // DD
			case RTC_SET_HOUR:  row=1; col=0; break; // HH
			case RTC_SET_MIN:   row=1; col=3; break; // MM
			case RTC_SET_SEC:   row=1; col=6; break; // SS
			default: break;
		}
		LCD_goto_XY(row, col);
		} else {
		LCD_write_command(0x0C); // Display ON, Cursor OFF, Blink OFF
	}
}

void rtc_ui_handle_buttons(uint8_t btn0, uint8_t btn1, uint8_t btn2, uint8_t btn3) {
	// [Button 3] RTC View <-> 설정 모드 전환
	if (btn3) {
		if (sub_mode == RTC_VIEW) {
			sub_mode = RTC_SET_YEAR; // 설정 모드 진입 (연도부터)
			} else {
			RTC_write_datetime(rtc_buf); // 설정 완료 시 DS1307에 일괄 저장
			sub_mode = RTC_VIEW;         // VIEW 모드로 복귀
		}
	}

	// VIEW 모드에서는 나머지 버튼 무시
	if (sub_mode == RTC_VIEW) return;

	// [Button 2] 다음 필드로 이동
	if (btn2) {
		sub_mode++;
		if (sub_mode > RTC_SET_SEC) sub_mode = RTC_SET_YEAR; // 순환
	}

	// [Button 0] 감소, [Button 1] 증가
	if (btn0 || btn1) {
		uint8_t val = get_field_val();
		
		if (btn0) { // 감소
			if (val == 0) {
				// 최소값 미만으로 내려가면 최대값으로 순환
				if (sub_mode == RTC_SET_MONTH) val = 12;
				else if (sub_mode == RTC_SET_DATE) val = 31;
				else if (sub_mode == RTC_SET_YEAR) val = 99;
				else val = 59;
				} else {
				val--;
			}
		}
		
		if (btn1) { // 증가
			uint8_t max_val = 99;
			if (sub_mode == RTC_SET_MONTH) max_val = 12;
			else if (sub_mode == RTC_SET_DATE) max_val = 31;
			else if (sub_mode == RTC_SET_HOUR) max_val = 23;
			else if (sub_mode == RTC_SET_MIN || sub_mode == RTC_SET_SEC) max_val = 59;

			if (val >= max_val) val = 0; // 최대값 초과 시 0으로 순환
			else val++;
		}
		
		set_field_val(val);
	}
}