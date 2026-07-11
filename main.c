#define F_CPU 16000000UL
#include "Text_LCD.h"
#include "I2C_RTC.h"
#include "keypad.h"
#include "button.h"
#include "cal.h"
#include "utils.h"
#include "rtc_ui.h"
////================================
#include <util/delay.h>
#include <avr/io.h>
#include <string.h>
#include <stdlib.h>

uint8_t MODE = 4;
uint16_t mscount = 0;
// 모드 정의
typedef enum { MODE_RTC, MODE_CALC } sys_mode_t;
sys_mode_t current_mode = MODE_RTC;

// 계산기 버퍼 (스크롤 지원을 위해 32로 증가)
#define CALC_BUF_SIZE 32
static char calc_buf[CALC_BUF_SIZE] = {0};
static uint8_t calc_len = 0;
static uint8_t cursor = 0; // 현재 커서 위치

// 전방 선언
void handle_keypad_input(uint8_t key);

// [1] S16 버튼 모드 전환 함수
void handle_mode_switch(uint8_t key) {
	if (key == ' ') {
		_delay_ms(150); // 디바운싱
		current_mode = (current_mode == MODE_RTC) ? MODE_CALC : MODE_RTC;
		LCD_clear();
		
		if (current_mode == MODE_RTC) {
			LCD_write_string("RTC Mode");
			} else {
			LCD_write_string("Calc Mode");
			LCD_goto_XY(1, 0);
			LCD_write_string("Input Expr...");
		}
		_delay_ms(800);
		LCD_clear();
	}
}

// [2] RTC 모드 실행 함수
void run_rtc_mode(void) {
    rtc_ui_update(); // LCD 갱신 및 커서 제어
    
    // 버튼 상태 읽기
    uint8_t b0 = get_button(BUTTON0, BUTTON0PIN);
    uint8_t b1 = get_button(BUTTON1, BUTTON1PIN);
    uint8_t b2 = get_button(BUTTON2, BUTTON2PIN);
    uint8_t b3 = get_button(BUTTON3, BUTTON3PIN);
    
    // 버튼 핸들러 호출
    rtc_ui_handle_buttons(b0, b1, b2, b3);
    
    _delay_ms(100);
}

// [3] 계산기 모드 실행 함수
void run_calc_mode(uint8_t key) {
		uint8_t input_detected = 0;  // 입력 감지 
	if (get_button(BUTTON0, BUTTON0PIN)) {
		init_cal(); calc_len = 0; cursor = 0; memset(calc_buf, 0, sizeof(calc_buf));
		LCD_clear(); LCD_goto_XY(1, 0); LCD_write_string("Cleared");
		_delay_ms(500); LCD_goto_XY(1, 0); LCD_write_string("               ");
		input_detected = 1; 
	}
	else if (get_button(BUTTON1, BUTTON1PIN)) {
		if (cursor > 0) {
			cursor--;
			for (uint8_t i = cursor; i < calc_len; i++) calc_buf[i] = calc_buf[i+1];
			calc_len--; calc_buf[calc_len] = '\0';
			update_calc_scroll_lcd(calc_buf, calc_len, cursor);
			input_detected = 1; 
		}
	}
	else if (get_button(BUTTON2, BUTTON2PIN)) {
		if (calc_len < CALC_BUF_SIZE - 1) {
			for (uint8_t i = calc_len; i > cursor; i--) calc_buf[i] = calc_buf[i-1];
			calc_buf[cursor] = '('; cursor++; calc_len++; calc_buf[calc_len] = '\0';
			update_calc_scroll_lcd(calc_buf, calc_len, cursor);
			input_detected = 1; 
		}
	}
	else if (get_button(BUTTON3, BUTTON3PIN)) {
		if (calc_len < CALC_BUF_SIZE - 1) {
			for (uint8_t i = calc_len; i > cursor; i--) calc_buf[i] = calc_buf[i-1];
			calc_buf[cursor] = ')'; cursor++; calc_len++; calc_buf[calc_len] = '\0';
			update_calc_scroll_lcd(calc_buf, calc_len, cursor);
			input_detected = 1; 
		}
	}
	
	// 키패드 입력이 있으면 플래그 설정
	if (key && key != ' ') {
		input_detected = 1;
	}
	handle_keypad_input(key);	
	 //입력이 감지되었으면 유휴 시간 카운터 초기화
	if (input_detected) {
		mscount = 0;
	}	



}

// [4] 키패드 입력 처리 함수 (계산기 모드 전용)
void handle_keypad_input(uint8_t key) {
	if (key && key != ' ') {
		if (key == '=') {
			calc_buf[calc_len] = '\0';
			int result = calculator(calc_buf);
			char res_str[8]; itoa(result, res_str, 10);
			LCD_goto_XY(1, 0); LCD_write_string("= "); LCD_write_string(res_str);
			_delay_ms(2000);
			LCD_goto_XY(1, 0); LCD_write_string("               ");
			cursor = 0;
		}
		else if ((key >= '0' && key <= '9') || key == '+' || key == '-' || key == '*' || key == '/') {
			if (calc_len < CALC_BUF_SIZE - 1) {
				for (uint8_t i = calc_len; i > cursor; i--) calc_buf[i] = calc_buf[i-1];
				calc_buf[cursor] = key; cursor++; calc_len++; calc_buf[calc_len] = '\0';
				update_calc_scroll_lcd(calc_buf, calc_len, cursor);
			}
		}
	}
}

// 메인 함수
int main(void) {
	LCD_init();
	I2C_init();
	init_keypad();
	init_button();
	 rtc_ui_init();

	// RTC 초기 시간 설정 (최초 1회만 실행 후 주석 처리)
	// RTC_set_date(26, 7, 1);
	// RTC_set_time(16, 10, 0);

	LCD_write_string("System Ready");
	_delay_ms(1000);
	LCD_clear();

	while (1) {
		uint8_t key = keypad_scan();

		// [1] S16 버튼 (' ' 키) 모드 전환
		if (key == ' ') {
			handle_mode_switch(key);
			continue; // 모드 전환 후 바로 다음 루프로
		}

		// [2] RTC 모드
		if (current_mode == MODE_RTC) {
			run_rtc_mode();
		}
		// [3] 계산기 모드
		else {
			run_calc_mode(key);
			   mscount++;
			   
			   	_delay_ms(100);
			    if (mscount >= 600) {
				    current_mode = MODE_RTC; // RTC 모드로 강제 전환
				    mscount = 0;          // 카운터 초기화
				    LCD_clear();             // 화면 정리
				    _delay_ms(1000);
				    LCD_clear();
				}
		}
	}
	return 0;
}
