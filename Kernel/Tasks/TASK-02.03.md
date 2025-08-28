### **Task ID: TASK-02.03**
### **Task: `ApplyUpgradeModifier` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `ApplyUpgradeModifier` 함수를 구현합니다. 이 함수는 자원 소모가 성공적으로 끝난 후, `DT_StatDefinition`의 `GrowthValueCurve`를 참조하여 스탯 증가량을 계산하고, 이 값을 영구적인 `FSTAT_Modifier`로 만들어 캐릭터의 스탯에 적용합니다. 마지막으로, `STAT_OnStatChanged_E` 이벤트를 전파하여 상태 변화를 알립니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `ApplyUpgradeModifier` 함수에 정확히 반영되어야 한다.
- [ ] `GrowthValueCurve`를 사용하여 다음 레벨의 스탯 증가량이 정확하게 계산되어야 한다.
- [ ] `Source.Upgrade` 태그를 가진 영구적인 `FSTAT_Modifier`가 생성되고 캐릭터에 적용되어야 한다.
- [ ] 스탯 적용 후 `STAT_OnStatChanged_E` 이벤트가 정확한 `FSTAT_ChangedPayload` 데이터와 함께 전파되어야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 함수는 `GameThread`에서 스탯을 수정하므로 준수)
    - [ ] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEAK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (해당 없음)
    - [ ] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (해당 없음)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(ApplyUpgradeModifier_GameThread)`를 선언한다.
    2.  **[데이터 조회]** `DT_StatDefinition`에서 `StatTagToUpgrade`에 해당하는 Row를 찾는다. 실패 시 `ensure` 후 종료한다.
    3.  **[현재 상태 저장]**
        a. 현재 스탯의 최종 값(`PreviousValue`)을 조회하여 저장한다.
        b. 현재 스탯의 업그레이드 레벨(`CurrentUpgradeLevel`)을 조회한다.
    4.  **[증가량 계산]** `GrowthValueCurve`를 사용하여 `CurrentUpgradeLevel + 1`에 해당하는 스탯 증가량(`BonusValue`)을 계산한다.
    5.  **[Modifier 생성]** 새로운 `FSTAT_Modifier` 인스턴스를 생성한다.
        *   `Modifier.TargetStatTag = StatTagToUpgrade;`
        *   `Modifier.Operation = EModifierOp::Add;` // 기본 값에 더하는 방식
        *   `Modifier.Value = BonusValue;`
        *   `Modifier.SourceTag = TAG_Source_Upgrade;` // 영구적임을 식별하는 태그
        *   `Modifier.SourceObject = GetOwner();`
    6.  **[Modifier 적용]** 생성된 Modifier를 캐릭터의 영구 Modifier 목록에 추가한다.
    7.  **[스탯 재계산]** Modifier 적용 후, 해당 스탯의 최종 값을 다시 계산(`RecalculateStat`)한다.
    8.  **[업그레이드 레벨 갱신]** 해당 스탯의 업그레이드 레벨을 1 증가시킨다. (`CurrentUpgradeLevel++`)
    9.  **[이벤트 전파]** `TASK-01.04`와 동일한 방식으로, 변경된 스탯 정보를 담은 `FSTAT_ChangedPayload`를 생성하여 `STAT_OnStatChanged_E` 이벤트를 브로드캐스트한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`ApplyUpgradeModifier`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: ApplyUpgradeModifier
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Add persistent FSTAT_Modifier using GrowthValueCurve"
      outputs:
        - "STAT_OnStatChanged_E(StatTagToUpgrade)"
    ```
*   `Mp01...기획서.md`: "`GrowthValueCurve`를 참조하여 해당 스탯의 기본 값(Base Value)에 영구적인 Modifier를 추가합니다. `STAT_OnStatChanged_E` 이벤트를 전파하여 모든 관련 시스템(UI 등)이 갱신되도록 합니다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-02.03"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`ApplyUpgradeModifier` 구현"
---