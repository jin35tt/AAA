### **Task ID: TASK-06.01**
### **Task: `Overload_Start` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `I_STAT_Overloadable_If::RequestOverload` 인터페이스 함수를 구현합니다. 이 함수는 `FOverloadParameters`를 입력받아 과부하 상태 진입을 시도합니다. 비용 지불이 가능할 경우, 관련 버프와 페널티를 적용하고, `State.Overload.Active` 태그를 부여하며, 과부하 종료 타이머를 설정하고, `STAT_OnOverloadStateChanged_E` 이벤트를 전파합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `RequestOverload` 함수에 정확히 반영되어야 한다.
- [ ] 이미 과부하 상태일 경우, 함수가 중복 실행되지 않아야 한다.
- [ ] `FOverloadParameters`에 명시된 비용을 지불할 수 있는지 먼저 검증해야 한다.
- [ ] 검증 통과 시, 비용이 차감되고 모든 버프/페널티 Modifier가 `Source.Overload` 태그와 함께 적용되어야 한다.
- [ ] `State.Overload.Active` 태그가 부여되고, 정확한 `Duration`으로 종료 타이머가 설정되어야 한다.
- [ ] `STAT_OnOverloadStateChanged_E` 이벤트가 `bIsOverloading = true` 상태로 전파되어야 한다.

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
    1.  **[함수 시그니처]** `void RequestOverload(const FOverloadParameters& InParams)`
    2.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Overload_Start_GameThread)`를 선언한다.
    3.  **[중복 실행 방지]** `if (bIsOverloading)` 이면, 경고 로그를 남기고 즉시 함수를 종료한다.
    4.  **[비용 검증]** `CanAffordCost(InParams.Costs)`와 같은 헬퍼 함수를 호출하여 비용 지불이 가능한지 확인한다.
        *   **실패 처리:** 지불 불가능하면 함수를 종료한다.
    5.  **[상태 시작]** `bIsOverloading = true;`
    6.  **[비용 지불]** `ApplyModifiers(InParams.Costs)`를 호출하여 비용을 실제로 차감한다.
    7.  **[버프/패널티 적용]** `ApplyModifiers(InParams.Buffs)`와 `ApplyModifiers(InParams.Penalties)`를 호출한다. 이때 모든 Modifier의 `SourceTag`는 `Source.Overload`로 설정한다.
    8.  **[태그 부여]** 캐릭터의 태그 컴포넌트에 `State.Overload.Active` 태그를 추가한다.
    9.  **[종료 타이머 설정]**
        a. `GetWorld()->GetTimerManager().SetTimer(OverloadEndTimerHandle, this, &ThisClass::Overload_End, InParams.Duration, false);`
        b. `Overload_End` 함수에 필요한 정보(예: `InParams.BurnoutDebuffTag`)를 전달할 수 있도록 람다 또는 `FTimerDelegate`를 사용한다.
    10. **[이벤트 전파]**
        a. `FSTAT_OverloadPayload Payload; Payload.bIsOverloading = true; Payload.TimeRemaining = InParams.Duration;`
        b. `STAT_OnOverloadStateChanged_E.Broadcast(Payload);`
    11. **[스탯 최종 갱신]** 영향을 받은 모든 스탯을 재계산하고 관련 `STAT_OnStatChanged_E` 이벤트를 전파한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  `I_STAT_Overloadable_If` 인터페이스를 상속받고, 위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`RequestOverload`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: Overload_Start
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Apply Costs and Buffs"
        - "Set State.Overload.Active"
      outputs: [ "STAT_OnOverloadStateChanged_E" ]
    ```
*   `Mp00...기획서.md`: "SKIL 시스템의 요청을 받아 안정적으로 스탯을 변경하고, 타이머를 관리하며, 상태를 원래대로 복구시키는 책임을 진다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-06.01"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Overload_Start` (RequestOverload) 구현"
---