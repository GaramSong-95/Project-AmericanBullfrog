# Project-AmericanBullfrog
Cortex-M3 기반의 STM32F 보드와 LCD를 활용한 C++ 기반의 미니게임 프로젝트

![image](https://github.com/user-attachments/assets/e6a6f9b3-dc64-4717-a579-ddcf79b94cd1)


아래 프로젝트 설명은 버전1을 기반.
버전2는 추가적으로 차량에 부딪히면 옆으로 밀리는 로직 구현



## 1.프로젝트 개요
**본 프로젝트는 Cortex-M3 기반의 STM32F 보드와 LCD를 활용하여 C++로 개발한 피하기 형식의 미니게임입니다. 프레임버퍼 기술을 활용해 LCD에 그래픽을 직접 구현하였으며, 조이스틱과 버튼을 통해 개구리 캐릭터를 조작하는 구조입니다. 객체지향 프로그래밍의 장점을 살려 개구리, 자동차, 총알 등의 게임 요소를 클래스 단위로 구현하였으며, 게임 구조와 기능의 확장성을 고려해 설계하였습니다.**

## 2.개발 환경 및 도구


<p align="left">
  <img src="https://img.shields.io/badge/C++-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white" alt="C++">
  <img src="https://img.shields.io/badge/STM32-03234B?style=for-the-badge&logo=stmicroelectronics&logoColor=white" alt="STM32">
  <img src="https://img.shields.io/badge/Embedded%20C-008080?style=for-the-badge" alt="Embedded C">
  <img src="https://img.shields.io/badge/LCD%20Display-000000?style=for-the-badge" alt="LCD Display">
  <img src="https://img.shields.io/badge/Joystick-808080?style=for-the-badge" alt="Joystick">
  <img src="https://img.shields.io/badge/Interrupt-FF6347?style=for-the-badge" alt="Interrupt">
  <img src="https://img.shields.io/badge/Timer-FFD700?style=for-the-badge" alt="Timer">
  <img src="https://img.shields.io/badge/OOP%20Design-800080?style=for-the-badge" alt="OOP">
</p>

MCU: STM32F 시리즈 (Cortex-M3 기반)

언어: C++

입력 장치: 조이스틱, 버튼

출력 장치: LCD 디스플레이 (프레임버퍼 방식)

개발 방식: 타이머 및 인터럽트를 활용한 이벤트 기반 처리

지원 라이브러리: LCD 출력용 라이브러리 (사각형 등 도형 출력 가능)

## 3.주요 기능
개구리 이동: 조이스틱 조작을 통해 상/하/좌/우 방향으로 이동

차량 피하기: 상단까지 이동한 후 다시 하단으로 돌아오는 왕복 구조

단계별 난이도 상승: 왕복 성공 시 차량 증가 및 속도 상승 (최대 3대, 5단계)

보스 스테이지: 5단계 성공 시 총을 발사할 수 있는 보스 스테이지 진입

총알 기능: 최대 5발까지 발사 가능, 차량과 충돌 시 파괴

승리 조건: 보스 스테이지에서 3대의 차량을 모두 파괴

패배 조건: 차량과 충돌하거나, 보스 스테이지에서 차량 파괴 실패

## 4.기술 구현
### ■조이스틱 및 버튼 처리:

인터럽트를 활용하여 입력 이벤트 감지

방향 입력 시 개구리의 기존 위치를 지우고 새 좌표에 다시 그림

### ■자동차 이동:

타이머를 활용하여 주기적으로 자동차 이동

단계에 따라 이동 속도 증가 (한 번에 움직이는 거리 증가 방식)

### ■객체지향 구조:

개구리, 자동차, 총알을 각각 클래스로 구현

각 객체는 위치, 이동, 충돌 함수 등을 포함

게임 시작 시 객체 생성, 필요에 따라 소멸 및 재생성

### ■충돌 판정:

각 객체의 좌표 정보를 기반으로 충돌 체크

개구리/차, 총알/차 간의 충돌 시 게임 상태 변화

## 5.어려웠던 점 및 해결 방법
### ■충돌 함수 구현의 어려움:
충돌 판정이 단순히 점의 충돌이 아닌, 객체의 전체 영역을 고려해야 했기 때문에 정밀한 좌표 연산이 필요했습니다. 각 객체의 너비와 높이를 고려한 범위 계산을 통해 해결하였습니다.

### ■총알 객체의 순환 문제:
총알을 객체로 구현하면서 무한정 생성 시 메모리 관리 문제가 발생하였습니다. 이를 해결하기 위해 총알 발사 횟수를 5회로 제한하고, 재사용 없이 발사 횟수만 카운팅하는 구조로 단순화하였습니다.

## 6.프로젝트를 통해 배운 점
객체지향 프로그래밍에서의 캡슐화와 설계의 중요성을 체감했습니다.

충돌 판정, 입력 처리, 타이머 활용 등 펌웨어 프로그래밍의 다양한 기술을 실전에서 적용하면서 이해도를 높였습니다.

게임의 구조를 설계하고 구현하면서 복잡한 시스템을 체계적으로 분리하여 관리하는 능력을 키웠습니다.

실시간 이벤트 처리와 그래픽 출력 등 임베디드 시스템의 핵심 요소를 직접 구현하면서 많은 성장을 할 수 있었습니다.
