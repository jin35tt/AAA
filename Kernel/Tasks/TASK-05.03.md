### **Task ID: TASK-05.03**
### **Task: `Flow_DecayTick` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `Flow_DecayTick` 주기 함수를 구현합니다. 이 함수는 `Flow_OnHitGain`에서 설정한 유예 시간이 만료된 후, 1초마다 호출되어 `DT_FlowRules`에 정의된 `DecayPerSecond` 값만큼 Flow 스택을 점진적으로 감소시킵니다. 스택이 0에 도달하면 스스로 멈춥니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `Flow_DecayTick` 관련 로직(시작 함수 포함)에 정확히 반영되어야 한다.
- [ ] 1초마다 `CurrentFlowStack`이 `DecayPerSecond` 값만큼 감소해야 한다.
- [ ] 스택이 감소할 때마다 `Flow_ApplyRewards`가 호출되고 `STAT_OnFlowStackChanged_E` 이벤트가 전파되어야 한다.
- [ ] `CurrentFlowStack`이 0 이하로 떨어지면, 주기적인 호출 타이머가 자동으로 중지되어야 한다.

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
    *   **참고:** `Flow_OnHitGain`에서 설정한 `FlowTimeoutTimerHandle`의 콜백 함수(`OnFlowTimeout`)가 아래의 `StartFlowDecay`를 호출하는 구조를 가정한다.

    1.  **[Decay 시작 함수]** `void StartFlowDecay()`:
        a. `DT_FlowRules`에서 규칙(Row)을 조회한다. 실패 시 `ensure` 후 종료한다.
        b. `if (Row.DecayPerSecond > 0)` 이면:
            i.   `GetWorld()->GetTimerManager().SetTimer(FlowDecayTimerHandle, this, &ThisClass::Flow_DecayTick, 1.0f, true);` // 1초마다 `Flow_DecayTick` 호출 시작

    2.  **[Decay 주기 함수]** `void Flow_DecayTick()`:
        a. **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Flow_DecayTick_GameThread)`를 선언한다.
        b. **[데이터 조회]** `DT_FlowRules`에서 규칙(Row)을 조회한다. 실패 시 타이머를 중지하고 종료한다.
        c. **[스택 계산]**
            i.   `float PreviousStack = CurrentFlowStack;`
            ii.  `float NewStack = FMath::Max(0.0f, PreviousStack - Row.DecayPerSecond);`
            iii. `CurrentFlowStack = NewStack;`
        d. **[이벤트 전파]** `STAT_OnFlowStackChanged_E` 이벤트를 새로운 스택 값과 함께 전파한다.
        e. **[보상 갱신]** `Flow_ApplyRewards()` 함수를 호출하여 보너스 스탯을 갱신한다.
        f. **[자기-종료 로직]**
            i.   `if (CurrentFlowStack <= 0.0f)` 이면:
                1. `GetWorld()->GetTimerManager().ClearTimer(FlowDecayTimerHandle);` // 타이머 중지

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, `Flow_DecayTick` 함수와 이를 시작시키는 로직(예: `OnFlowTimeout` 함수 내)의 `// TODO` 부분을 완성하십시오.

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
    - step: Flow_DecayTick
      where: "GameThread_TimeSliced"
      authority: "ServerOnly"
      effects:
        - "Every second: Stack -= DecayPerSecond; if TimeoutSeconds elapsed → Reset"
        - "Schedule Flow_ApplyRewards"
    ```
*   `Mp00...기획서.md`: "일정 시간(예: 3초) 내에 다음 스킬을 명중시키지 못하거나, 적에게 피격당하면 모든 스택이 즉시 0으로 초기화된다." (이 Task는 타임아웃 '이후'의 점진적 감소를 다룸)

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-05.03"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Flow_DecayTick` 구현"
---