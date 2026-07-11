#include "utils.h"
#include "Text_LCD.h" // LCD 함수 사용을 위해 포함

uint8_t bcd2dec(uint8_t data)
{
	uint8_t high, low;
	low = data & 0x0F;
	high = (data >> 4) * 10;
	return (high + low);
}

uint8_t dec2bcd(uint8_t data)
{
	uint8_t high, low;
	high = (data / 10) << 4;
	low = data % 10;
	return (high + low);
}

void bcd_to_str(uint8_t bcd, char *str)
{
	str[0] = ((bcd >> 4) & 0x0F) + '0';
	str[1] = (bcd & 0x0F) + '0';
	str[2] = '\0';
}

// 스크롤 뷰포트 방식 LCD 갱신
void update_calc_scroll_lcd(char *buf, uint8_t len, uint8_t cursor) {
	uint8_t start = 0;
	
	// 커서가 16번 인덱스 이후라면 시작점을 조정하여 커서가 항상 화면 내에 보이게 함
	if (cursor >= 16) {
		start = cursor - 15;
	}
	
	LCD_goto_XY(0, 0);
	
	// 시작점부터 16글자 또는 남은 글자만큼 출력
	for (uint8_t i = 0; i < 16; i++) {
		if (start + i < len) {
			LCD_write_data(buf[start + i]);
			} else {
			LCD_write_data(' '); // 빈 공간은 공백으로 채워 잔상 제거
		}
	}
}