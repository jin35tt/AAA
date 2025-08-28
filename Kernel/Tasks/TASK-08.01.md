### **Task ID: TASK-08.01**
### **Task: `Counterplay_OpenWindow` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `Counterplay_OpenWindow` 로직을 구현합니다. (예: `I_STAT_Counterable_If::EnterVulnerableState` 인터페이스 함수 내). 이 함수는 `DT_CounterplayWindows`를 참조하여 특정 카운터 창을 열고, 캐릭터를 '취약' 상태로 만들며, 지정된 시간 후 자동으로 실패 처리되는 타이머를 설정하고, `STAT_OnCounterplayWindow_E` 이벤트를 전파합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 관련 함수에 정확히 반영되어야 한다.
- [ ] 이미 카운터 창이 열려있을 경우, 함수가 중복 실행되지 않아야 한다.
- [ ] `DT_CounterplayWindows`에서 `WindowID`에 해당하는 `Duration`과 `TriggerTag`를 성공적으로 조회해야 한다.
- [ ] 캐릭터의 내부 카운터 상태가 `ECounterState::Vulnerable`로 설정되고, `TriggerTag`가 저장되어야 한다.
- [ ] 조회된 `Duration`으로 타임아웃 타이머가 설정되어야 하며, 만료 시 `Counterplay_Resolve`를 '실패' 상태로 호출해야 한다.
- [ ] `STAT_OnCounterplayWindow_E` 이벤트가 '창 열림' 상태와 남은 시간을 담아 전파되어야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 함수는 `GameThread`에서 실행되므로 준수)
    - [ ] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEAK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (해당 없음)
    - [ ] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (해당 없음)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[함수 시그니처]** `void EnterVulnerableState(FName WindowID)`
    2.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Counterplay_OpenWindow_GameThread)`를 선언한다.
    3.  **[중복 실행 방지]** `if (CurrentCounterState != ECounterState::None)` 이면 즉시 함수를 종료한다.
    4.  **[규칙 조회]** `DT_CounterplayWindows`에서 `WindowID`에 해당하는 Row를 찾는다. 실패 시 `ensure` 후 종료한다.
    5.  **[상태 설정]**
        a. `CurrentCounterState = ECounterState::Vulnerable;`
        b. `CurrentCounterTriggerTag = Row.TriggerTag;`
        c. `CurrentCounterWindowID = WindowID;`
    6.  **[타임아웃 타이머 설정]**
        a. `FTimerDelegate TimeoutDelegate = FTimerDelegate::CreateUObject(this, &ThisClass::OnCounterWindowTimeout, WindowID);`
        b. `GetWorld()->GetTimerManager().SetTimer(CounterWindowTimerHandle, TimeoutDelegate, Row.Duration, false);`
    7.  **[이벤트 전파]**
        a. `FSTAT_CounterplayWindowPayload Payload;`
        b. `Payload.WindowID = WindowID;`, `Payload.State = ECounterState::Vulnerable;`, `Payload.TimeRemaining = Row.Duration;`
        c. `STAT_OnCounterplayWindow_E.Broadcast(Payload);`
    8.  **[타임아웃 콜백 함수]** `void OnCounterWindowTimeout(FName TimedOutWindowID)`:
        a. `if (CurrentCounterWindowID == TimedOutWindowID && CurrentCounterState == ECounterState::Vulnerable)` 조건 확인
        b. `Counterplay_Resolve(ECounterResult::Fail_Timeout);` // 실패 상태로 다음 단계 호출

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, `I_STAT_Counterable_If` 인터페이스의 `EnterVulnerableState` 함수와 `OnCounterWindowTimeout` 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: Counterplay_OpenWindow
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Spawn window by DT_CounterplayWindows(TriggerTag, TargetQuery)"
      outputs: [ "STAT_OnCounterplayWindow_E" ]
    ```
*   `Mp00...기획서.md`: "보스는 특정 패턴 ... 시전 시, 자신에게 ... '약점 상태' 태그를 일시적으로 보유한다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-08.01"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Counterplay_OpenWindow` (EnterVulnerableState) 구현"
---