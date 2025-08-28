### **Task ID: TASK-07.01**
### **Task: `Fusion_NotifyEvent` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `I_STAT_Fusion_If::NotifyFusionEvent` 인터페이스 함수를 구현합니다. 이 함수는 외부 시스템(예: SKIL)으로부터 융합 관련 이벤트(`EventTag`, `Source` 액터)를 수신하고, `DT_SkillFusionRules`와 대조하여 융합 스택을 누적시키는 비동기 프로세스를 시작합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `NotifyFusionEvent` 함수에 정확히 반영되어야 한다.
- [ ] 함수는 `EventTag`와 `DT_SkillFusionRules`의 `RequiredTags`를 비동기적으로 비교하여 일치하는 규칙을 찾아야 한다.
- [ ] 일치하는 규칙이 있을 경우, `StackGainPerEvent`만큼 해당 대상(액터 또는 위치)의 융합 스택을 증가시켜야 한다.
- [ ] 스택 업데이트가 완료된 후, 게임 스레드에서 `Fusion_CheckAmp` 함수를 호출하여 증폭 상태를 검사해야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`, `Z1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 설계는 비동기 Task에 순수 데이터만 전달하여 준수)
    - [x] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEAK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (결과를 적용하는 최종 콜백에서 필수 적용)
    - [ ] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (해당 없음)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (본 Task는 단기 실행이므로 해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[인터페이스 구현부]** `void NotifyFusionEvent(FGameplayTag EventTag, AActor* Source)`:
        a. **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Fusion_NotifyEvent_GameThread)`를 선언한다.
        b. **[데이터 준비]**
            i.  `DT_SkillFusionRules`의 모든 Row를 순수 데이터 구조체 배열(`TArray<FFusionRuleData> RulesData`)로 복사한다.
            ii. `TWeakObjectPtr<AActor> TargetActor = Source;` // 또는 Source의 위치 정보
        c. **[비동기 실행]** `UE::Tasks::Launch`를 사용하여 비동기 Task를 실행한다. 이 Task는 계산 결과를 담을 구조체 `FFusionUpdateResult`를 `TFuture`로 반환한다.
            i.  **[AsyncTask 람다]** `(TArray<FFusionRuleData> RulesData, FGameplayTag EventTag)`를 캡처한다.
                1. `TRACE_CPUPROFILER_EVENT_SCOPE(Fusion_NotifyEvent_AsyncTask)`
                2. `float TotalStackGain = 0.0f;`
                3. `RulesData`를 순회하며 `Rule.RequiredTags` 쿼리가 `EventTag`와 일치하는지 검사한다.
                4. 일치하는 모든 `Rule`의 `StackGainPerEvent` 값을 `TotalStackGain`에 누적한다.
                5. `return FFusionUpdateResult{ TotalStackGain };`
        d. **[결과 적용 콜백]** `.Then()`을 사용하여 게임 스레드에서 실행될 콜백을 예약한다.
            i.  **[.Then() 람다]** `(TFuture<FFusionUpdateResult> Result)`를 인자로 받는다. `SAFE_WEAK_LAMBDA`로 `this`와 `TargetActor`를 캡처한다.
                1. `if (!WeakThis.IsValid() || !WeakTarget.IsValid()) return;`
                2. `float StackGain = Result.Get().StackGain;`
                3. `int32 NewStack = UpdateFusionStackForTarget(WeakTarget.Get(), StackGain);` // 내부 맵 업데이트
                4. `WeakThis->Fusion_CheckAmp(WeakTarget.Get(), NewStack);`

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  `I_STAT_Fusion_If` 인터페이스를 상속받고, 위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`NotifyFusionEvent`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: Fusion_NotifyEvent
      where: "AsyncTask"
      authority: "ServerOnly"
      effects:
        - "Evaluate DT_SkillFusionRules.RequiredTags"
        - "Fusion.Stack += StackGainPerEvent; clamp to MaxStacks"
    ```
*   `Mp00...기획서.md`: "에너지 누적: ...플레이어의 공격이 ... 반복적으로 적중할 때마다, 해당 위치/대상에 `Fusion.Charge.Stack` 태그 카운트가 누적된다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-07.01"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Fusion_NotifyEvent` 구현"
---