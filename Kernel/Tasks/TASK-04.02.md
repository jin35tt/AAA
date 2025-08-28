### **Task ID: TASK-04.02**
### **Task: `CheckSignatureReady` (SigCharge_CheckReady) 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `CheckSignatureReady` 함수를 구현합니다. 이 함수는 시그니처 차지 스택이 변경된 직후 호출되며, 차지 스택이 최대치에 도달하여 '준비 완료' 상태로 전환되었는지, 또는 최대치 미만으로 떨어져 '준비 해제' 상태로 전환되었는지를 검사하고, 상태 전환이 발생했을 때만 관련 태그를 부여/제거하고 이벤트를 전파합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `CheckSignatureReady` 함수에 정확히 반영되어야 한다.
- [ ] 차지 스택이 최대치에 도달하는 '순간'에 `Ability.CanUnleash.SignatureMove` 태그가 캐릭터에 정확히 1회 부여되어야 한다.
- [ ] 차지 스택이 최대치 미만으로 떨어지는 '순간'에 `Ability.CanUnleash.SignatureMove` 태그가 캐릭터에서 정확히 1회 제거되어야 한다.
- [ ] 위 상태 전환이 발생했을 때만 `STAT_OnSignatureChargeReady_E` 이벤트가 `bIsReady` 값을 담아 전파되어야 한다.

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
    1.  **[함수 시그니처]** `void CheckSignatureReady(float PreviousCharge)`로 함수를 정의한다. `AddSignatureCharge` 함수가 호출한 직후 이 함수를 호출하며, 변경 전 값을 인자로 넘겨준다.
    2.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(CheckSignatureReady_GameThread)`를 선언한다.
    3.  **[상태 정의]**
        a. `bool bWasReady = (PreviousCharge >= MaxSignatureCharge);`
        b. `bool bIsNowReady = (SignatureChargeStack >= MaxSignatureCharge);`
    4.  **[상태 전환 감지]**
        a. `if (!bWasReady && bIsNowReady)` // 준비 안됨 -> 준비 완료 전환
            i.   캐릭터의 태그 컴포넌트에 `Ability.CanUnleash.SignatureMove` 태그를 추가한다.
            ii.  `FSTAT_ReadyPayload Payload; Payload.bIsReady = true;`
            iii. `STAT_OnSignatureChargeReady_E.Broadcast(Payload);`
        b. `else if (bWasReady && !bIsNowReady)` // 준비 완료 -> 준비 안됨 전환
            i.   캐릭터의 태그 컴포넌트에서 `Ability.CanUnleash.SignatureMove` 태그를 제거한다.
            ii.  `FSTAT_ReadyPayload Payload; Payload.bIsReady = false;`
            iii. `STAT_OnSignatureChargeReady_E.Broadcast(Payload);`

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`CheckSignatureReady`** 함수의 `// TODO` 부분을 완성하십시오. 이 함수는 `AddSignatureCharge` 함수의 로직 마지막 부분에서 호출되어야 합니다.

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
    - step: CheckSignatureReady
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "If SignatureChargeStack >= MaxSignatureCharge -> set Ability.CanUnleash.SignatureMove"
      outputs:
        - "STAT_OnSignatureChargeReady_E"
    ```
*   `Mp00...기획서.md`: "해방 조건: '시그니처 차지'가 최대치에 도달하면, 캐릭터에게 `Ability.CanUnleash.SignatureMove` 태그가 부여된다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-04.02"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`CheckSignatureReady` 구현"
---