### **Task ID: TASK-02.01**
### **Task: `ValidateUpgradable` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `ValidateUpgradable` 함수를 구현합니다. 이 함수는 업그레이드하려는 스탯의 `FGameplayTag`를 입력받아, `DT_StatDefinition` 데이터 테이블을 참조하여 해당 스탯이 업그레이드 가능한지, 그리고 플레이어가 필요한 자원을 충분히 보유하고 있는지 검증합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `ValidateUpgradable` 함수에 정확히 반영되어야 한다.
- [ ] 함수는 `DT_StatDefinition` 데이터 테이블에서 `CanBeUpgraded` 플래그를 정확히 확인해야 한다.
- [ ] 함수는 `RequiredPointCurve`를 참조하여 다음 레벨업에 필요한 자원량을 계산하고, 플레이어의 현재 자원과 비교해야 한다.
- [ ] 모든 검증을 통과하면 `true`를, 하나라도 실패하면 `false`를 반환해야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 함수는 `GameThread`에서 Data Table을 조회하므로 준수)
    - [ ] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEAK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (해당 없음)
    - [ ] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (해당 없음)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(ValidateUpgradable_GameThread)`를 선언한다.
    2.  **[데이터 테이블 조회]** `DT_StatDefinition` 데이터 테이블에서 `StatTagToUpgrade`를 키로 사용하여 해당 스탯의 정의 데이터(Row)를 찾는다.
        *   **실패 처리:** 만약 Row를 찾지 못하면, `ensureMsgf`로 에러를 기록하고 `false`를 반환한다.
    3.  **[업그레이드 가능 여부 확인]** 조회한 Row에서 `CanBeUpgraded` 필드가 `false`이면, `false`를 반환한다.
    4.  **[필요 자원 계산]**
        a. 현재 캐릭터의 해당 스탯에 대한 업그레이드 레벨(예: `CurrentUpgradeLevel`)을 내부 데이터에서 가져온다.
        b. 조회한 Row의 `RequiredPointCurve` 에셋을 사용하여 `CurrentUpgradeLevel + 1`에 필요한 자원량(`RequiredPoints`)을 계산한다.
    5.  **[자원 보유량 확인]**
        a. 캐릭터가 현재 보유한 업그레이드 자원량(`CurrentPoints`)을 가져온다. (별도의 재화 시스템 또는 내부 스탯 참조)
        b. `if (CurrentPoints < RequiredPoints)` 이면, `false`를 반환한다.
    6.  **[검증 성공]** 모든 검사를 통과했으면, `true`를 반환한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`ValidateUpgradable`** 함수의 `// TODO` 부분을 완성하십시오.

---
### **6. 예외 처리 프로토콜 (Exception Handling Protocol)**
*   본 Task는 **`B-Type`** 프로토콜을 따릅니다.
*   Task 수행 중 문제 발생 시, 즉시 실행을 중단하고 `2.2. 예외 처리 프로토콜`의 **상태 전이 규칙**에 따라 상태를 변경하고 보고해야 합니다.

### **7. 참조 소스 (Reference from Source)**
*   **핵심 청사진:** `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`
*   **요구사항 컨텍스트:** `Mp01_ProjectAAA 캐릭터 상태 시스템 _01메인 기획서.md`

### **8. 소스 요구사항 매핑 (Source Requirement Mapping)**
*   `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`:
    ```yaml
    - step: ValidateUpgradable
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Check CanBeUpgraded == true"
        - "Check resource via RequiredPointCurve"
    ```
*   `Mp01...기획서.md`: "해당 스탯이 `DT_StatDefinition`에서 업그레이드 가능한지 (`CanBeUpgraded == true`). 플레이어가 요구되는 자원(`RequiredPointCurve` 참조)을 충분히 가지고 있는지"

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-02.01"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`ValidateUpgradable` 구현"
---