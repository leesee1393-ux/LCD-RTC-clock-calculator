# ATmega128A 기반 RTC/LCD/계산기 프로젝트

## 📋 프로젝트 개요

ATmega128A 마이크로컨트롤러를 사용하여 DS1307 RTC 모듈과 LCD를 연동한 시계 및 계산기 시스템을 구현합니다.

### 주요 기능
-  **RTC 시간 표시**: DS1307에서 I2C 통신으로 시간 데이터 수신
-  **시간 수정 기능**: 사용자가 직접 시간 설정 가능
-  **LCD 디스플레이**: 실시간 시간 및 계산 결과 표시
-  **계산기 모드**: 기본 사칙연산 지원

---

## 🛠️ 개발 환경

![개발환경](https://github.com/user-attachments/assets/c80f4981-6192-45ea-abed-67e608c34551)

### 사용 하드웨어 partlist

<img width="1506" height="538" alt="image" src="https://github.com/user-attachments/assets/0d795bed-5529-40c8-8c40-e02e69bded70" />

---

## 📚 DS1307 RTC 모듈

### datasheet 참고 자료

![DS1307 Datasheet](https://github.com/user-attachments/assets/9a6ea986-3d44-43d0-8e27-ea3814a45fad)

### 레지스터 맵

DS1307의 내부 레지스터 구조

![레지스터 주소표] <img width="1591" height="753" alt="image" src="https://github.com/user-attachments/assets/90a2eea1-d148-40b5-9f42-abfde8aab851" />


![레지스터 맵] <img width="1004" height="389" alt="image" src="https://github.com/user-attachments/assets/c6860731-868b-43e4-b9de-546ca7069be1" />


### 구성 회로도

<img width="1294" height="885" alt="image" src="https://github.com/user-attachments/assets/d541241a-e0b7-4d7b-902b-6f4d226ba3bd" />

### 제품 사진

<img width="1570" height="844" alt="image" src="https://github.com/user-attachments/assets/06f0de74-771c-4a1d-874c-e52f6b5846d9" />



### FSM

<img width="1689" height="986" alt="image" src="https://github.com/user-attachments/assets/15823331-e786-434c-a4b0-ab4ae725c40d" />


### I2C 통신 파형

실제 I2C 버스에서의 통신 신호는 다음과 같습니다:

![I2C 통신 파형](https://github.com/user-attachments/assets/b38ea032-cc3a-4377-a7f5-399717715e74)

#### 주요 레지스터 주소
- **초 (Seconds)**: 0x00
- **분 (Minutes)**: 0x01
- **시 (Hours)**: 0x02
- **요일 (Day)**: 0x03
- **일 (Date)**: 0x04
- **월 (Month)**: 0x05
- **년 (Year)**: 0x06

---

## 🖥️ LCD 디스플레이

### LCD 사양

![LCD 모듈] <img width="1639" height="733" alt="image" src="https://github.com/user-attachments/assets/90e177b3-aeed-4f66-8cfd-b6a9f87a350a" />


### 통신 파형

LCD와의 데이터 전송 시 나타나는 신호 파형입니다:

![LCD 통신 파형](https://github.com/user-attachments/assets/083ffecf-8c40-4304-b17d-a46b15b4912e)

#### 핀 구성
- **RS (Register Select)**: 명령/데이터 선택
- **RW (Read/Write)**: 읽기/쓰기 제어
- **E (Enable)**: 데이터 인에이블
- **D0-D7**: 데이터 버스

---

## 🔧 구현 사항

### 1. I2C 통신 초기화
```c
// I2C 초기화 코드 
void I2C_init(void)
{
	TWSR=0x00;   
	TWBR = (uint8_t)TWBR_VALUE; 
	TWCR = (1 << TWEN); 
}
```



### 2. DS1307 시간 읽기
```c
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
```
### 3.LCD 출력 함수
```c
// LCD txt 읽기
void LCD_write_string(char *string)
{
	uint8_t i;
	for(i= 0; string[i];i++)
		LCD_write_data(string[i]);	
}
```
### 동작 영상


<https://youtube.com/shorts/54PC0KObX_4>



### RTC_시간 대기모드

<https://youtu.be/y-6MvUBLur4>



## 보안점

### 1. 문제 현상

    RTC에서 시간 신호를 전달받았으나 LCD 화면에 시간이 업데이트되지 않음.
    LCD 화면에 시간이 아예 표시되지 않는 이슈 발생.

### 2. 원인 분석

    RTC 클럭 오류: RTC 모듈 자체의 하드웨어적 결함 또는 클럭 신호 불안정.
    LCD 명암도 문제: LCD 요구 스펙에 맞는 저항값(Vo 핀 전압)이 제공되지 않아 화면이 보이지 않음.

### 3. 조치 방안

    RTC 교체: 정상 작동하는 RTC 모듈로 교체하여 해결.
    가변 저항 조절: LCD의 Vo 핀에 연결된 가변 저항(Potentiometer)을 조절하여 최적의 명암도 설정.



