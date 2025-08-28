### **Task ID: TASK-08.02**
### **Task: `Counterplay_Resolve` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `Counterplay_Resolve` 로직을 구현합니다. 이 로직은 `ApplyDamage` 함수 내에서 플레이어의 공격이 유효한 카운터인지 판별하고, 그 결과(또는 시간 초과로 인한 실패)에 따라 `DT_CounterplayWindows`에 명시된 보상 또는 페널티를 적용합니다. 모든 과정이 끝나면 카운터 상태를 초기화하고 관련 이벤트를 전파합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ 'ApplyDamage' 함수는 'Vulnerable' 상태의 캐릭터에게 들어온 공격의 'AttackTags'와 저장된 'TriggerTag'를 비교하여 성공/실패를 정확히 판별해야 한다.
- [ ] 판정 즉시, 'CounterWindowTimerHandle' 타이머가 중지되어야 한다.
- [ ] 성공 시 'SuccessRewards'가, 실패 시 'FailurePenalties'가 'FSTAT_Modifier'로 캐릭터에 정확히 적용되어야 한다.
- [ ] 카운터 상태가 해결된 후(Success/Fail), 모든 관련 내부 변수(CurrentCounterState 등)가 초기 상태(None)로 리셋되어야 한다.
- [ ] 'STAT_OnCounterplayWindow_E' 이벤트가 해결된 상태(Success/Fail)를 담아 전파되어야 한다.

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
    *   **참고:** 이 로직은 주로 `ApplyDamage` 함수 내에서 시작되고, `ResolveCounterplay`라는 중앙 헬퍼 함수를 통해 처리된다.

    1.  **[진입점 수정]** `ApplyDamage(..., const FGameplayTagContainer& AttackTags, ...)` 함수 내부:
        a. `if (CurrentCounterState == ECounterState::Vulnerable)` 조건 확인.
            i.   `bool bSuccess = AttackTags.HasTag(CurrentCounterTriggerTag);`
            ii.  `ResolveCounterplay(bSuccess);`
            iii. `return;` // 카운터가 시도되었으므로, 일반 피해 계산 로직을 건너뛴다.

    2.  **[중앙 처리 함수]** `void ResolveCounterplay(bool bSuccess)`:
        a. **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Counterplay_Resolve_GameThread)`를 선언한다.
        b. **[가드 로직]** `if (CurrentCounterState != ECounterState::Vulnerable)` 이면 즉시 종료한다.
        c. **[타이머 정리]** `GetWorld()->GetTimerManager().ClearTimer(CounterWindowTimerHandle);`
        d. **[규칙 조회]** `DT_CounterplayWindows`에서 `CurrentCounterWindowID`에 해당하는 Row를 찾는다. 실패 시 `ensure` 후 상태만 리셋하고 종료한다.
        e. **[결과 적용]**
            i.   `if (bSuccess)`:
                1.  `CurrentCounterState = ECounterState::Success;`
                2.  `Row.SuccessRewards`를 `FSTAT_Modifier`로 변환하여 캐릭터에 적용한다.
            ii.  `else`:
                1.  `CurrentCounterState = ECounterState::Fail;`
                2.  `Row.FailurePenalties`를 `FSTAT_Modifier`로 변환하여 캐릭터에 적용한다.
        f. **[이벤트 전파]** `STAT_OnCounterplayWindow_E` 이벤트를 현재 `CurrentCounterState`와 함께 전파한다.
        g. **[상태 완전 초기화]**
            i.   `CurrentCounterState = ECounterState::None;`
            ii.  `CurrentCounterTriggerTag = FGameplayTag::EmptyTag;`
            iii. `CurrentCounterWindowID = NAME_None;`
        h. **[스탯 최종 갱신]** 영향을 받은 스탯들을 재계산하고 `STAT.Broadcast.Changed` 이벤트를 전파한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, `ApplyDamage` 함수를 수정하고 새로운 헬퍼 함수 `ResolveCounterplay`의 `// TODO` 부분을 완성하십시오.

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
    - step: Counterplay_Resolve
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "On success: apply SuccessRewards; on fail: apply FailurePenalties"
      outputs: [ "STAT_OnCounterplayWindow_E", "STAT.Broadcast.Changed" ]
    ```
*   `Mp00...기획서.md`: "카운터 성공 시... 최대 자원(마나)의 30%를 즉시 회복한다... 쿨타임을 즉시 초기화한다..."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-08.02"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Counterplay_Resolve` 구현"
---