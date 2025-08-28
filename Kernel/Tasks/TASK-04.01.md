### **Task ID: TASK-04.01**
### **Task: `AddSignatureCharge` (SigCharge_Accumulate) 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `AddSignatureCharge` 함수를 구현합니다. 이 함수는 특정 행동(`CauseTag`)이 발생했을 때 `DT_SignatureChargeConditions` 데이터 테이블을 참조하여 획득할 시그니처 차지 양을 계산하고, 이를 캐릭터의 `SignatureChargeStack`에 누적시킨 후, `STAT_OnSignatureChargeChanged_E` 이벤트를 전파합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `AddSignatureCharge` 함수에 정확히 반영되어야 한다.
- [ ] `DT_SignatureChargeConditions` 데이터 테이블을 `CauseTag`로 조회하여 기본 차지 획득량을 성공적으로 가져와야 한다.
- [ ] `TargetActor`의 태그를 `MultiplierByTargetTagQuery` 조건과 비교하여 보너스 배율을 정확히 적용해야 한다.
- [ ] 최종 계산된 차지 양이 캐릭터의 `SignatureChargeStack`에 누적되고, 최대치를 초과하지 않도록 Clamp 되어야 한다.
- [ ] 차지 스택 변경 후 `STAT_OnSignatureChargeChanged_E` 이벤트가 전파되어야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 함수는 `GameThread`에서 실행되므로 준수)
    - [ ] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (해당 없음)
    - [ ] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (해당 없음)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[함수 시그니처]** `void AddSignatureCharge(AActor* TargetActor, FGameplayTag CauseTag, float Amount)` 인터페이스 구현부를 작성한다.
    2.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(AddSignatureCharge_GameThread)`를 선언한다.
    3.  **[데이터 조회]** `DT_SignatureChargeConditions` 데이터 테이블에서 `CauseTag`를 키로 사용하여 해당 Row를 찾는다.
        *   **실패 처리:** Row를 찾지 못하면 함수를 조용히 종료한다.
    4.  **[기본값 설정]** `float ChargeToAdd = Row.Amount;`
    5.  **[배율 적용]**
        a. `if (TargetActor && Row.MultiplierByTargetTagQueryCurve)` 조건 확인.
        b. `TargetActor`의 태그 컴포넌트에서 태그 목록을 가져온다.
        c. `if (Row.TargetFilterTagQuery.Matches(TargetActorTags))` 조건 확인.
            i.   `float Multiplier = Row.MultiplierByTargetTagQueryCurve->GetFloatValue(1.0f);` // 가정: 커브는 고정된 값으로 평가
            ii.  `ChargeToAdd *= Multiplier;`
    6.  **[스택 누적]**
        a. `float PreviousCharge = SignatureChargeStack;`
        b. `SignatureChargeStack = FMath::Clamp(PreviousCharge + ChargeToAdd, 0.0f, MaxSignatureCharge);`
    7.  **[페이로드 생성 및 전파]**
        a. `FSTAT_SignaturePayload Payload;`를 생성한다.
        b. `Payload.CurrentCharge = SignatureChargeStack;`, `Payload.MaxCharge = MaxSignatureCharge;` 등을 채운다.
        c. `STAT_OnSignatureChargeChanged_E.Broadcast(Payload);`를 호출한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  `I_STAT_SignatureSupport_If` 인터페이스를 상속받고, 위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`AddSignatureCharge`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: SigCharge_Accumulate
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Lookup DT_SignatureChargeConditions by CauseTag"
        - "Signature.Charge.Stack += Amount*(MultiplierByTargetTagQuery)"
      outputs: [ "STAT_OnSignatureChargeChanged_E" ]
    ```
*   `Mp00...기획서.md`: "자원 축적: 전투 중 특정 조건(데이터 테이블로 정의)을 충족할 때마다 '시그니처 차지'...자원이 축적된다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-04.01"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`AddSignatureCharge` 구현"
---