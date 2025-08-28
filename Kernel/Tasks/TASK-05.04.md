### **Task ID: TASK-05.04**
### **Task: `Flow_ApplyRewards` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `Flow_ApplyRewards` 함수를 구현합니다. 이 함수는 현재 Flow 스택을 기반으로 `DT_FlowRules` 데이터 테이블에서 적용할 보상을 조회하고, 기존의 Flow 보너스를 모두 제거한 뒤 새로운 보너스 스탯 Modifier를 캐릭터에게 적용합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `Flow_ApplyRewards` 함수에 정확히 반영되어야 한다.
- [ ] 함수는 `Source.Flow` 태그를 가진 모든 기존 `FSTAT_Modifier`를 성공적으로 제거해야 한다.
- [ ] 현재 `CurrentFlowStack`에 해당하는 보상 목록을 `DT_FlowRules`에서 정확히 조회해야 한다.
- [ ] 조회된 모든 보상을 새로운 `FSTAT_Modifier`로 만들어 캐릭터에 적용해야 한다.
- [ ] 모든 Modifier 적용이 끝난 후, 영향을 받은 스탯들이 재계산되고 관련 `STAT_OnStatChanged_E` 이벤트가 전파되어야 한다.

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
    1.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Flow_ApplyRewards_GameThread)`를 선언한다.
    2.  **[기존 보상 제거]** `Source.Flow` 태그를 가진 모든 `FSTAT_Modifier`를 캐릭터의 Modifier 목록에서 제거한다.
    3.  **[조건 확인]** `if (CurrentFlowStack <= 0)` 이면, 제거 작업으로 충분하므로 함수를 종료한다.
    4.  **[적용할 보상 조회]**
        a. `DT_FlowRules` 데이터 테이블에서 현재 Flow 규칙(Row)을 조회한다. 실패 시 `ensure` 후 종료한다.
        b. `G01`의 `DT_FlowBuffRewards` 테이블 구조를 참조하여, `CurrentFlowStack`에 해당하는 가장 높은 레벨의 보상(`TArray<FFLOW_RewardSpec>`)을 찾는다.
    5.  **[새 보상 적용]** 조회된 `Rewards` 배열을 순회한다.
        a. 각 `RewardSpec`에 대해 새로운 `FSTAT_Modifier` 인스턴스를 생성한다.
        b. `Modifier.TargetStatTag = RewardSpec.AffectedStatTag;`
        c. `Modifier.Operation = RewardSpec.Operation;`
        d. `Modifier.Value = RewardSpec.Value;`
        e. `Modifier.SourceTag = TAG_Source_Flow;` // 식별을 위한 고유 태그
        f. 생성된 Modifier를 캐릭터의 Modifier 목록에 추가한다.
    6.  **[일괄 재계산 및 이벤트 전파]** 모든 새 Modifier가 추가된 후, 영향을 받은 모든 스탯에 대해 재계산을 수행하고, 변경된 각 스탯에 대해 `STAT_OnStatChanged_E` 이벤트를 전파하는 통합 함수(예: `RecalculateAffectedStatsAndBroadcast()`)를 호출한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`Flow_ApplyRewards`** 함수의 `// TODO` 부분을 완성하십시오.

---
### **6. 예외 처리 프로토콜 (Exception Handling Protocol)**
*   본 Task는 **`B-Type`** 프로토콜을 따릅니다.
*   Task 수행 중 문제 발생 시, 즉시 실행을 중단하고 `2.2. 예외 처리 프로토콜`의 **상태 전이 규칙**에 따라 상태를 변경하고 보고해야 합니다.

### **7. 참조 소스 (Reference from Source)**
*   **핵심 청사진:** `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`
*   **요구사항 컨텍스트:** `Mp00_ProjectAAA 핵심 전투 재미 요소 기획서.md`, `G01_ProjectAAA 게임_어빌리티_캐릭터 상태 시스템 개발 지침서.md`

### **8. 소스 요구사항 매핑 (Source Requirement Mapping)**
*   `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`:
    ```yaml
    - step: Flow_ApplyRewards
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "For each reward in DT_FlowRules.Rewards, apply as FSTAT_Modifier scaled by current Stack"
      outputs: [ "STAT_OnStatChanged_E" ]
    ```
*   `Mp00...기획서.md`: "보상: `Flow.Stack` 스택 수에 비례하여 다음과 같은 보상이 `STAT` 시스템을 통해 실시간으로 적용된다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-05.04"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Flow_ApplyRewards` 구현"
---