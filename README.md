RTC//LCD/계산기 기능 구현
======
**참여자:** 이세형 장태희

#프로젝트 목표
1. ATmega128a 를 이용
2. DS1307의 RTC 데이터를 I2C를 이용하여 가져와
3. LCD에 수정 기능이 탑재된 시계를 만들고,
4. 계산기 모드도 추가적으로 만든다.

개발환경
<img width="1193" height="400" alt="image" src="https://github.com/user-attachments/assets/c80f4981-6192-45ea-abed-67e608c34551" />

**DS1307 (I2C) RTC datasheet**
-----
<img width="1130" height="816" alt="image" src="https://github.com/user-attachments/assets/9a6ea986-3d44-43d0-8e27-ea3814a45fad" />




**레지스터 주소표**
<img width="1591" height="753" alt="image" src="https://github.com/user-attachments/assets/975196ba-d6eb-4013-90dc-50791e4951c2" />



<img width="1850" height="798" alt="image" src="https://github.com/user-attachments/assets/69fbd8c1-dfeb-4eda-a55b-20548e21ae93" />



**실제 통신 파형 그림**
<img width="1646" height="798" alt="image" src="https://github.com/user-attachments/assets/b38ea032-cc3a-4377-a7f5-399717715e74" />

**LCD**
-----
<img width="1888" height="641" alt="image" src="https://github.com/user-attachments/assets/b5cd7fbf-9794-4e69-8856-ac09c147d05d" />

LCD 통신 파형
<img width="1974" height="669" alt="image" src="https://github.com/user-attachments/assets/083ffecf-8c40-4304-b17d-a46b15b4912e" />

