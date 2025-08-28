### **Task ID: TASK-01.01**
### **Task: `CollectDefenseRules` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `CollectDefenseRules` 함수를 구현합니다. 이 함수의 책임은 현재 캐릭터에게 적용되고 있는 모든 방어 효과(버프, 실드, 장비 패시브 등)로부터 무시할 공격 태그 목록(`IgnoreAttackTags`)을 수집하여 단일 `FGameplayTagContainer`로 집계하는 것입니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `CollectDefenseRules` 함수에 정확히 반영되어야 한다.
- [ ] 함수는 캐릭터에 부착된 모든 방어 효과 제공자(예: 특정 인터페이스를 구현하는 컴포넌트)를 성공적으로 찾아내야 한다.
- [ ] 찾아낸 모든 효과로부터 `IgnoreAttackTags`를 누락 없이 수집하여 하나의 컨테이너로 합쳐 반환해야 한다.

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
    1.  **[시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(USTATAbilityComponent::CollectDefenseRules)`를 선언하여 프로파일링을 시작한다.
    2.  **[초기화]** 비어있는 `FGameplayTagContainer CombinedIgnoreTags`를 생성한다.
    3.  **[소유자 확인]** `GetOwner()`를 통해 이 컴포넌트의 소유자(Owner Actor)를 가져온다. 소유자가 `IsValid()` 하지 않으면, 빈 `CombinedIgnoreTags`를 즉시 반환하고 종료한다.
    4.  **[방어 효과 수집]** 소유자 액터의 모든 컴포넌트를 대상으로 `IStatDefenseProvider_If` 인터페이스를 구현하는 컴포넌트 목록을 가져온다. (`GetComponentsByInterface`)
    5.  **[태그 집계]** 수집된 각 방어 효과 컴포넌트를 순회(loop)한다.
        a. 해당 컴포넌트의 `GetIgnoreAttackTags()` 인터페이스 함수를 호출하여 `FGameplayTagContainer`를 얻는다.
        b. 얻어온 태그 컨테이너를 `CombinedIgnoreTags.AppendTags()`를 사용하여 기존 컨테이너에 추가한다.
    6.  **[반환]** 모든 태그가 집계된 `CombinedIgnoreTags`를 반환한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`CollectDefenseRules`** 함수의 `// TODO` 부분을 완성하십시오. (`IStatDefenseProvider_If` 와 같은 필요한 인터페이스는 `.h` 파일에 선언해야 할 수 있습니다.)

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
    - step: CollectDefenseRules
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Gather Defense.IgnoreAttackTags from active buffs/shields"
    ```
*   `Mp01...기획서.md`: "방어(Defense)의 역할: 자신이 무력화시키거나 효과를 적용하지 않을 '공격 태그' 목록을 가진다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-01.01"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`CollectDefenseRules` 구현"
---