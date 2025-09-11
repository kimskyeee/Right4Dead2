# Right 4 Dead 2
Right4Dead는 FPS 게임 **Left 4 Dead 2**의 플레이어와 무기 시스템을 언리얼 엔진으로 구현한 프로젝트입니다.

🎥 아래의 사진을 클릭시 전체 게임 플레이 영상을 보실 수 있습니다.

[![Video Label](http://img.youtube.com/vi/5d-Z1wNdabQ/0.jpg)](https://youtu.be/5d-Z1wNdabQ)


<br>

## 사용 기술
- Unreal Engine 5.5
- C++ / Blueprint 혼합 구조
- Rider

<br>

## 프로젝트 개요
| 항목 | 내용 |
| --- | --- |
| **장르** | 호러 서바이벌 FPS 게임 |
| **플랫폼** | PC |
| **맵 컨셉** | 좀비를 처지하며 목적지까지 콜라를 배달, 피난처로 이동합니다. |
| **개발 내용** | 플레이어, 전투 피드백 UI, 무기별 공격 로직, 무기 슬롯 시스템 |

<br>

## 구현 상세 설명
각 기능의 더 자세한 설명은 [여기](https://kimskye.notion.site/Right-4-Dead-2-1ceb7f13b7a68097a582f736a828d96c)서 확인하실 수 있습니다.

---

## 1. 무기 슬롯 시스템
무기 탐지 → 줍기 → 장착 → 해제/버리기 → 무기 전환까지의 전체 루프를 구조화했습니다.

### 무기 분류
| 슬롯 번호 | 무기 |
| --- | --- |
| **1번 슬롯** | PrimaryWeapon : 총기류 |
| **2번 슬롯** | MeleeWeapon : 근접무기(도끼) |
| **3번 슬롯** | ThrownWeapon : 투척무기(파이프폭탄) |
| **기타** | 4번슬롯에는 회복아이템이 포함되어있습니다. | 

<br>

### 무기 교체

- `TraceForWeapon()`에서 카메라 전방 300cm 캡슐 트레이스로 무기 액터 탐색후 무기 장
- Weapon SlotType을 기준으로 슬롯에 등록하고 무기 줍기 몽타주 실행
  
https://github.com/user-attachments/assets/3c8160b0-1d96-4810-9000-438296df2d3b


<br>

### 동일 슬롯 무기 교체

https://github.com/user-attachments/assets/54b24535-b0e7-4d0f-86e2-f7dbef61c1c7

<br>

---

## 2. 무기별 공격 로직
무기별로 공격 로직을 설계했습니다.

### 밀쳐내기

무기의 유무와 관계없이 **우클릭 시**, 전방에 원기둥을 생성해 영역을 계산합니다.
이후 영역 내의 좀비들은 밀쳐내고 데미지를 적용합니다.

<img width="912" height="816" alt="스크린샷 2025-06-12 142950" src="https://github.com/user-attachments/assets/e3f0d3d5-00dc-4cbc-8f8c-1aeca6647aee" />

1. `SpawnCylinder()` 를 통해 실린더 메쉬를 생성하고 Overlap 감지
2. Overlap액터 판단
   
   2-1. 좀비가 맞았는가?
   
   2-2. 맞은 좀비가 플레이어 전방 기준 좌우 45도 내에 있는가? (내적으로 각도계산)

 3. 해당하는 좀비에게 데미지 적용

<br>

### 근접무기: 휘두르기

- 플레이어의 **뷰포트 중심**으로, 중앙에 위치한 좀비를 타격합니다.
- **다중 피격 부위 감지**: 여러 부위에 맞았을 때, 맞은 부위를 저장한 후 좀비의 우선순위 본(Bone)으로 판정 후 데미지를 적용합니다.

https://github.com/user-attachments/assets/556a7699-f57c-4d64-aac6-754ad6b9deea


- 근접무기에 **적중했을 때**, 화면에 피 튀는 효과를 순차적으로 출력하는 시각적 피드을 제공합니다.

https://github.com/user-attachments/assets/49b204f1-bff5-420a-9850-171beefb2384

<br>

### 투척무기: 폭탄 폭발

- `SuggestProjectileVelocity_CustomArc`과 `PredictProjectilePath`로 카메라 방향 기반 투척 궤적을 계산합니다.
- 이후 일정 반경 내의 좀비를 유인 후 폭발 시 데미지를 적용합니다.

https://github.com/user-attachments/assets/2a8f8ddc-6005-41e0-a359-ee9048479a6c

<br>

### 주무기: 총 발사

- Hit Scan 방식으로, Timer 기반 자동연사를 지원합니다.

https://github.com/user-attachments/assets/3dafe6b0-a61b-4c58-ba0f-4e4eaf56f630

<br>

---

## 3. 피격 방향 인디케이터 UI

- 플레이어가 피격 당했을때, 피격 방향을 알려주는 Indicator UI를 제작하였습니다.
- `GetPlayerCameraManager`에서 Forward 벡터 확보하여 각도를 계산합니다.

https://github.com/user-attachments/assets/cec32120-50c9-4bb0-8972-6b7293b26700

- 회전각 계산 방식
  
| 연산 | 벡터 | 목적 | 의미 |
| --- | --- | --- | --- |
| **내적** | `PlayerForward` · `공격방향` | 사잇각의 cos(θ) 계산 | **공격이 전방/후방인지** 판단 |
| **외적** | `PlayerForward × 공격방향` | 방향성(+/-) 계산 | **공격이 좌/우 어느 쪽에서 왔는지** 판단 |

---

<br>

## 트러블 슈팅
- **무기 슬롯 중복 저장 문제, 무기 탐지(E) 정확도 향상** 등을 해결했습니다.
- 프로젝트 관련 트러블 슈팅은 [여기](https://www.notion.so/kimskye/Right-4-Dead-2-1ceb7f13b7a68097a582f736a828d96c?source=copy_link#20bb7f13b7a6804f9e25fe27a82f0ad4)에서 확인하실 수 있습니다.



