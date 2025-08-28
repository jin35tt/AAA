### **Task ID: TASK-01.02**
### **Task: `MatchAttackVsDefense` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `MatchAttackVsDefense` 비동기 함수를 구현합니다. 이 함수는 공격자의 `AttackTags`와 방어자가 무시하는 모든 태그의 집합인 `IgnoreAttackTags`를 입력받아, 두 태그 컨테이너 간에 일치하는 태그가 하나라도 있는지 백그라운드 스레드에서 검사하고 그 결과를 (`bool` 형태의 `TFuture`) 반환합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `MatchAttackVsDefense` 함수에 정확히 반영되어야 한다.
- [ ] 함수는 `AsyncTask`로 실행되어 게임 스레드를 차단하지 않아야 한다.
- [ ] `AttackTags`와 `IgnoreAttackTags` 사이에 하나 이상의 공통 태그가 존재하면 `true`를, 그렇지 않으면 `false`를 `TFuture`를 통해 반환해야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`, `Z1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 함수는 순수 데이터 연산으로 UObject에 접근하지 않음)
    - [ ] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEAK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (해당 없음)
    - [ ] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (해당 없음)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (본 Task는 단기 실행이므로 해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[함수 시그니처]** `TFuture<bool> MatchAttackVsDefense(const FGameplayTagContainer& AttackTags, const FGameplayTagContainer& IgnoreAttackTags)`로 함수를 정의한다.
    2.  **[비동기 실행]** `UE::Tasks::Launch(UE_SOURCE_LOCATION, ...)`를 사용하여 새 비동기 Task를 실행한다. 람다(lambda) 내에서 다음을 수행한다:
        a. **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(MatchAttackVsDefense_AsyncTask)`를 선언한다.
        b. **[핵심 로직]** `bool bIsMatchFound = IgnoreAttackTags.HasAny(AttackTags);`를 실행하여 두 태그 컨테이너 간에 겹치는 태그가 있는지 확인한다.
        c. **[결과 반환]** `bIsMatchFound` 값을 반환한다.
    3.  **[Future 반환]** `UE::Tasks::Launch`가 반환한 `TFuture<bool>` 객체를 즉시 호출자에게 반환한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`MatchAttackVsDefense`** 함수의 `// TODO` 부분을 완성하십시오.

---
### **6. 예외 처리 프로토콜 (Exception Handling Protocol)**
*   본 Task는 **`B-Type`** 프로토콜을 따릅니다.
*   Task 수행 중 문제 발생 시, 즉시 실행을 중단하고 `2.2. 예외 처리 프로토tocol`의 **상태 전이 규칙**에 따라 상태를 변경하고 보고해야 합니다.

### **7. 참조 소스 (Reference from Source)**
*   **핵심 청사진:** `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`
*   **요구사항 컨텍스트:** `Mp01_ProjectAAA 캐릭터 상태 시스템 _01메인 기획서.md`

### **8. 소스 요구사항 매핑 (Source Requirement Mapping)**
*   `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`:
    ```yaml
    - step: MatchAttackVsDefense
      where: "AsyncTask"
      authority: "ServerOnly"
      effects:
        - "Compute (AttackTags - Defense.IgnoreAttackTags)"
        - "Set bDefenseIgnored if any AttackTag is in ignore list"
    ```
*   `Mp01...기획서.md`: "이 방어 효과의 `IgnoreAttackTags` 목록에, 현재 들어온 공격의 `AttackTags` 중 하나라도 포함되는가?"

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-01.02"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`MatchAttackVsDefense` 구현"
---