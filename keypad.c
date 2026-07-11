/*
 * keypad.c
 *
 * Created: 2026-06-29 오후 02:11:26
 *  Author: kccistc
 */ 

#include "keypad.h"
uint8_t keypad_get_button(int row, int col);
uint8_t keypad_scan(void);

void init_keypad(void)
{
	KEYPAD_DDR = 0x0f; // row: 입력  col: 출력
	KEYPAD_PORT = 0xff; // 출력 모드
 
	
}
uint8_t keypad_scan(void)
{
	uint8_t data =0; // key값 저장 변수 data = 00000000
	
	for (int row =0; row<4 ;row++)
	{
		for(int col =0 ; col<4; col++)
		{
			 data = keypad_get_button(row,col);
			 if (data)  // if data >=1
				return data;
		}
	}
}

uint8_t keypad_get_button(int row, int col)

{
#if 0
	uint8_t keypad_char[4][4] = {
		{'+', '=','0', ' '},
		{'*','6','5','4'},
		{'-','9','8','7'},
		{'/','3','2','1'}				
	};	
#else
	uint8_t keypad_char[4][4] = {
		{'/','3','2','1'},
		{'*','6','5','4'},
		{'-','9','8','7'},
		{'+','=','0',' '}
	};
#endif		
	// ACTIVE =LOW로 동작 
	static int8_t prev_state[4][4]=
	{
		{ 1,1,1,1},  // 초기 버튼은 눌러지지 않은 상태로 한다.
		{ 1,1,1,1},
		{ 1,1,1,1},
		{ 1,1,1,1}
	};
	
	uint8_t currnt_state = 1;
	
	KEYPAD_PORT =0xff; 
	KEYPAD_PORT &= ~(1<<3- col);  // 3-col: COL4가 해당PA0에 연결되어 있기 때문에
								 // 해당col에 전류를 흘린다.
	for(int delay =0; delay <20 ; delay++)  // keypad check를 위한 delay
											// 0.625us X 20 = 1.25us
	currnt_state = (KEYPAD_PIN & (1<<(row+4))) >> (row+4);
	// 예) row 0 : PA4에 연결 
	// 76543210
	// 11101111  KEYPAD_PIN & (1<< (row+4))
	// 00001110  >> (row+4)
	if(currnt_state ==0 && prev_state[row][col] == 1)  // 처음 눌려진 상태 
	{
			prev_state[row][col] = 0;
			return 0;
	}
	else if(currnt_state == 1 && prev_state[row][col] == 0)
	{   // 이전에 버튼이 눌려지고 현재는 버튼을 땐 상태이며 버튼을 1번 눌렀다 땐것을 인정
		prev_state[row][col] = 1;  // prev_state를 초기화
		return keypad_char[row][col];
		return keypad_char[row][col];
	}
	
	return 0 ;
	
}