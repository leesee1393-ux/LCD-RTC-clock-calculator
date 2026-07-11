# ATmega128A 기반 RTC/LCD/계산기 프로젝트

## 📋 프로젝트 개요

ATmega128A 마이크로컨트롤러를 사용하여 DS1307 RTC 모듈과 LCD를 연동한 시계 및 계산기 시스템을 구현합니다.

### 주요 기능
- ✅ **RTC 시간 표시**: DS1307에서 I2C 통신으로 시간 데이터 수신
- ✅ **시간 수정 기능**: 사용자가 직접 시간 설정 가능
- ✅ **LCD 디스플레이**: 실시간 시간 및 계산 결과 표시
- ✅ **계산기 모드**: 기본 사칙연산 지원

---

## 🛠️ 개발 환경

![개발환경](https://github.com/user-attachments/assets/c80f4981-6192-45ea-abed-67e608c34551)

### 사용 하드웨어
| 구성요소 | 모델/사양 |
|---------|----------|
| MCU | ATmega128A |
| RTC 모듈 | DS1307 (I2C 인터페이스) |
| 디스플레이 | 16x2 Character LCD |

---

## 📚 DS1307 RTC 모듈

### datasheet 참고 자료

![DS1307 Datasheet](https://github.com/user-attachments/assets/9a6ea986-3d44-43d0-8e27-ea3814a45fad)

### 레지스터 맵

DS1307의 내부 레지스터 구조는 다음과 같습니다:

![레지스터 주소표](https://github.com/user-attachments/assets/975196ba-d6eb-4013-90dc-50791e4951c3)

![레지스터 상세](https://github.com/user-attachments/assets/69fbd8c1-dfeb-4eda-a55b-20548e21ae94)

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

![LCD 모듈](https://github.com/user-attachments/assets/b5cd7fbf-9794-4304-b17d-a46b15b4912e)

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
// I2C 초기화 코드 예시
void I2C_Init(void) {
    TWSR = 0x00;
    TWBR = ((F_CPU / SCL_CLOCK) - 16) / 2;
    TWCR = (1 << TWEN);
}
