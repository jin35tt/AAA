### **Task ID: TASK-06.02**
### **Task: `Overload_Tick` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `Overload_Tick` 함수를 구현합니다. 이 함수는 과부하가 활성화된 동안 주기적으로(예: 0.1초마다) 호출되어, 남은 지속 시간을 갱신하고 `STAT_OnOverloadStateChanged_E` 이벤트를 전파하여 UI 등이 실시간으로 업데이트될 수 있도록 합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `Overload_Tick` 함수 및 관련 타이머 설정 로직에 정확히 반영되어야 한다.
- [ ] `Overload_Start` 함수에서 `Overload_Tick`을 주기적으로 호출하는 타이머가 설정되어야 한다.
- [ ] `Overload_Tick` 함수는 남은 시간을 정확히 차감하고, `bIsOverloading = true` 상태와 함께 `STAT_OnOverloadStateChanged_E` 이벤트를 전파해야 한다.
- [ ] `Overload_End` 함수에서 이 주기적인 타이머가 반드시 중지되어야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`, `Z1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 함수는 `GameThread`의 타이머에 의해 호출되므로 준수)
    - [ ] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEAK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (해당 없음)
    - [x] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (본 함수는 매우 가벼운 연산이므로 준수)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    *   **참고:** `Overload_Start` 함수 내부에 이 타이머를 시작하는 로직이 추가되어야 한다.
    
    1.  **[타이머 시작 로직]** (`Overload_Start` 함수 내부):
        a. `float TickInterval = 0.1f;` // UI 업데이트 주기
        b. `OverloadTimeRemaining = InParams.Duration;` // 남은 시간 초기화
        c. `GetWorld()->GetTimerManager().SetTimer(OverloadTickTimerHandle, this, &ThisClass::Overload_Tick, TickInterval, true);` // 0.1초마다 `Overload_Tick` 호출 시작

    2.  **[주기 함수]** `void Overload_Tick()`:
        a. **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Overload_Tick_GameThread)`를 선언한다.
        b. **[가드 로직]** `if (!bIsOverloading)` 이면, `GetWorld()->GetTimerManager().ClearTimer(OverloadTickTimerHandle);`를 호출하고 즉시 함수를 종료한다.
        c. **[시간 차감]** `OverloadTimeRemaining -= GetWorld()->GetTimerManager().GetTimerRate(OverloadTickTimerHandle);`
        d. **[이벤트 전파]**
            i.   `FSTAT_OverloadPayload Payload;`
            ii.  `Payload.bIsOverloading = true;`
            iii. `Payload.TimeRemaining = OverloadTimeRemaining > 0 ? OverloadTimeRemaining : 0;`
            iv.  `STAT_OnOverloadStateChanged_E.Broadcast(Payload);`

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`Overload_Tick`** 함수를 만들고, `Overload_Start` 함수에 이 타이머를 시작하는 로직을 추가하십시오.

---
### **6. 예외 처리 프로토콜 (Exception Handling Protocol)**
*   본 Task는 **`B-Type`** 프로토콜을 따릅니다.
*   Task 수행 중 문제 발생 시, 즉시 실행을 중단하고 `2.2. 예외 처리 프로토콜`의 **상태 전이 규칙**에 따라 상태를 변경하고 보고해야 합니다.

### **7. 참조 소스 (Reference from Source)**
*   **핵심 청사진:** `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`
*   **요구사항 컨텍스트:** `Mp00_ProjectAAA 핵심 전투 재미 요소 기획서.md`

### **8. 소스 요구사항 매핑 (Source Requirement Mapping)**
*   `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`:
    ```yaml
    - step: Overload_Tick
      where: "GameThread_TimeSliced"
      authority: "ServerOnly"
      effects:
        - "Decrement TimeRemaining; maintain Buffs/Penalties"
      outputs: [ "STAT_OnOverloadStateChanged_E" ]
    ```
*   `Mp00...기획서.md`: "(과부하) ...총 지속시간" (지속시간을 UI 등에 표시하기 위한 주기적인 업데이트 필요)

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-06.02"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Overload_Tick` 구현"
---