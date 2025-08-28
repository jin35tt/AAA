### **Task ID: TASK-02.02**
### **Task: `ConsumeResource` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `ConsumeResource` 함수를 구현합니다. `ValidateUpgradable` 검증이 통과된 후 호출되는 이 함수는, 스탯 업그레이드에 필요한 자원(예: 'Upgrade Points')을 플레이어의 재화/자원 풀에서 실제로 차감하는 역할을 수행합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `ConsumeResource` 함수에 정확히 반영되어야 한다.
- [ ] 함수는 업그레이드에 필요한 자원량을 정확히 재계산해야 한다.
- [ ] 외부 재화 시스템(또는 내부 스탯)과 연동하여 계산된 양만큼의 자원을 성공적으로 차감해야 한다.
- [ ] 자원 차감 성공 시 `true`를, 어떠한 이유로든 실패 시 `false`를 반환해야 한다.

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
    1.  **[함수 시그니처]** `bool ConsumeResource(const FGameplayTag& StatTagToUpgrade)`로 함수를 정의한다.
    2.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(ConsumeResource_GameThread)`를 선언한다.
    3.  **[필요 자원 재계산]** (안정성을 위해) `ValidateUpgradable`에서 수행했던 것과 동일한 로직으로 `DT_StatDefinition`과 `RequiredPointCurve`를 참조하여 필요한 자원량(`RequiredPoints`)을 다시 계산한다.
    4.  **[자원 차감 시도]**
        a. 플레이어의 재화를 관리하는 시스템(예: `UPlayerCurrencyComponent`)의 참조를 얻는다.
        b. 해당 시스템의 `SpendUpgradePoints(RequiredPoints)`와 같은 함수를 호출한다. 이 함수는 차감 성공 시 `true`를 반환한다고 가정한다.
    5.  **[결과 반환]** `SpendUpgradePoints` 함수의 반환 값을 그대로 이 함수의 최종 반환 값으로 사용한다. (`return bWasSuccessful;`)

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`ConsumeResource`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: ConsumeResource
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Spend UpgradePoints"
    ```
*   `Mp01...기획서.md`: "검증 통과 시, 자원을 소모하고..."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-02.02"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`ConsumeResource` 구현"
---