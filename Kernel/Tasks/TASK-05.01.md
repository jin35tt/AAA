### **Task ID: TASK-05.01**
### **Task: `Flow_OnHitGain` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `Flow_OnHitGain` 함수를 구현합니다. 이 함수는 플레이어의 공격 성공과 같은 이벤트에 의해 호출되며, `DT_FlowRules` 데이터 테이블을 참조하여 Flow 스택을 증가시키고, 스택 소멸 타이머를 리셋하며, 변경된 스택에 따른 보상을 즉시 적용하고, `STAT_OnFlowStackChanged_E` 이벤트를 전파합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `Flow_OnHitGain` 함수에 정확히 반영되어야 한다.
- [ ] `DT_FlowRules` 데이터 테이블에서 `OnHitGain` 및 `MaxStack` 값을 성공적으로 조회해야 한다.
- [ ] Flow 스택이 `OnHitGain` 만큼 증가하고 `MaxStack`을 초과하지 않도록 Clamp 되어야 한다.
- [ ] 기존의 Flow 소멸 타이머가 있다면 초기화하고, 새로운 타이머를 `TimeoutSeconds`에 맞춰 설정해야 한다.
- [ ] 스택 변경 후 `Flow_ApplyRewards` 함수가 즉시 호출되어 보상이 갱신되어야 한다.
- [ ] `STAT_OnFlowStackChanged_E` 이벤트가 정확한 페이로드와 함께 전파되어야 한다.

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
    1.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Flow_OnHitGain_GameThread)`를 선언한다.
    2.  **[데이터 조회]** `DT_FlowRules`에서 현재 활성화된 Flow 규칙(Row)을 찾는다.
        *   **실패 처리:** Row를 찾지 못하면 `ensureMsgf`로 에러를 기록하고 함수를 종료한다.
    3.  **[스택 계산]**
        a. `float PreviousStack = CurrentFlowStack;`
        b. `float NewStack = FMath::Clamp(PreviousStack + Row.OnHitGain, 0.0f, Row.MaxStack);`
        c. `CurrentFlowStack = NewStack;`
    4.  **[타이머 관리]**
        a. 기존에 실행 중인 `FlowTimeoutTimerHandle`이 있다면 `GetWorld()->GetTimerManager().ClearTimer()`로 초기화한다.
        b. `GetWorld()->GetTimerManager().SetTimer()`를 사용하여 `Row.TimeoutSeconds` 이후 `OnFlowTimeout` 함수(이후 `Flow_DecayTick`을 시작시킴)가 호출되도록 새로운 타이머를 설정한다.
    5.  **[이벤트 전파]**
        a. `FSTAT_FlowPayload Payload;`를 생성하고 `CurrentStack`, `MaxStack` 등의 값을 채운다.
        b. `STAT_OnFlowStackChanged_E.Broadcast(Payload);`를 호출한다.
    6.  **[즉시 보상 적용]** `Flow_ApplyRewards()` 함수를 호출하여 변경된 스택 레벨에 맞는 보너스 스탯을 즉시 적용/갱신한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  `I_STAT_Flow_If` 인터페이스를 상속받고, 위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`Flow_OnHitGain`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: Flow_OnHitGain
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Stack += DT_FlowRules.OnHitGain"
        - "Clamp to MaxStack"
      outputs: [ "STAT_OnFlowStackChanged_E" ]
    ```
*   `Mp00...기획서.md`: "스택 획득: 스킬이 적에게 명중할 때마다 `Flow.Stack` 태그 스택을 1씩 획득한다. (최대 5스택)"

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-05.01"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Flow_OnHitGain` 구현"
---