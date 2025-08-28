### **Task ID: TASK-05.02**
### **Task: `Flow_OnDamageTakenReset` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `Flow_OnDamageTakenReset` 함수를 구현합니다. 이 함수는 플레이어가 피해를 입었을 때 호출되며, 현재 활성화된 Flow 스택을 즉시 0으로 초기화하고, 모든 관련 타이머를 중지시키며, 적용 중이던 보너스 스탯을 제거하고, `STAT_OnFlowStackChanged_E` 이벤트를 전파합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `Flow_OnDamageTakenReset` 함수에 정확히 반영되어야 한다.
- [ ] `CurrentFlowStack` 값이 0으로 설정되어야 한다.
- [ ] 활성화된 모든 Flow 관련 타이머(예: `FlowTimeoutTimerHandle`, `FlowDecayTimerHandle`)가 중지되어야 한다.
- [ ] `Flow_ApplyRewards()` 함수가 호출되어 기존에 적용되던 모든 Flow 보너스 스탯이 제거되어야 한다.
- [ ] `STAT_OnFlowStackChanged_E` 이벤트가 전파되어야 한다.

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
    1.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Flow_OnDamageTakenReset_GameThread)`를 선언한다.
    2.  **[조건 확인]** `if (CurrentFlowStack <= 0)` 이면, 이미 스택이 없으므로 아무 작업도 하지 않고 즉시 함수를 종료한다.
    3.  **[상태 저장]** `float PreviousStack = CurrentFlowStack;`
    4.  **[스택 초기화]** `CurrentFlowStack = 0.0f;`
    5.  **[타이머 초기화]**
        a. `GetWorld()->GetTimerManager().ClearTimer(FlowTimeoutTimerHandle);`
        b. `GetWorld()->GetTimerManager().ClearTimer(FlowDecayTimerHandle);` // Decay Tick도 중지
    6.  **[이벤트 전파]**
        a. `FSTAT_FlowPayload Payload;`를 생성하고 `CurrentStack` (0.0f), `MaxStack` 등의 값을 채운다.
        b. `STAT_OnFlowStackChanged_E.Broadcast(Payload);`를 호출한다.
    7.  **[보상 제거]** `Flow_ApplyRewards()` 함수를 호출한다. (스택이 0이므로, 이전에 적용된 모든 보너스 Modifier가 제거된다.)

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`Flow_OnDamageTakenReset`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: Flow_OnDamageTakenReset
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Stack -= DT_FlowRules.OnDamageTakenLoss or reset to 0"
      outputs: [ "STAT_OnFlowStackChanged_E" ]
    ```
*   `Mp00...기획서.md`: "스택 소멸: ...적에게 피격당하면 모든 스택이 즉시 0으로 초기화된다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-05.02"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Flow_OnDamageTakenReset` 구현"
---