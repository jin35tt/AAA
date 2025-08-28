### **Task ID: TASK-03.02**
### **Task: `CalculateSynergyScore` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `CalculateSynergyScore` 비동기 함수를 구현합니다. 이 함수는 캐릭터의 모든 활성 소스 태그(`SourceTags`)와 관련 데이터 테이블(`DT_SynergyGroupRules`, `DT_SynergyLevelThresholds`)의 내용을 입력받아, '동적 세트 시너지' 점수를 계산하고, 최종적으로 부여할 시너지 레벨 태그(`Synergy.Level.*`)를 결정하여 (`FGameplayTagContainer` 형태의 `TFuture`) 반환합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `CalculateSynergyScore` 함수에 정확히 반영되어야 한다.
- [ ] 함수는 `AsyncTask`로 실행되어 게임 스레드를 차단하지 않아야 한다.
- [ ] `DT_SynergyGroupRules`의 가중치, `DiminishingReturnCurve`를 포함한 모든 규칙을 적용하여 시너지 점수를 정확히 계산해야 한다.
- [ ] 계산된 점수를 `DT_SynergyLevelThresholds`와 비교하여 가장 높은 달성 가능한 시너지 레벨 태그를 `TFuture`를 통해 반환해야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`, `Z1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 함수는 UObject 대신 데이터 복사본을 사용하므로 준수)
    - [ ] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEAK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (해당 없음)
    - [ ] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (해당 없음)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (본 Task는 단기 실행이므로 해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[함수 시그니처]** (스레드 안전성을 위해) `TFuture<FGameplayTagContainer> CalculateSynergyScore(TArray<FGameplayTag> SourceTags, TArray<FSynergyGroupRule> Rules, TArray<FSynergyLevelThreshold> Thresholds)`로 함수를 정의한다.
    2.  **[비동기 실행]** `UE::Tasks::Launch(UE_SOURCE_LOCATION, ...)`를 사용하여 새 비동기 Task를 실행한다. 람다 내에서 다음을 수행한다:
        a. **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(CalculateSynergyScore_AsyncTask)`를 선언한다.
        b. **[초기화]** `float TotalSynergyScore = 0.0f;`
        c. **[그룹별 점수 계산]** `Rules` 배열의 각 `Rule`을 순회한다:
            i.   `SourceTags`에서 `Rule.GroupQuery`와 일치하는 태그들의 개수(`MatchCount`)를 센다.
            ii.  `ensure(Rule.DiminishingReturnCurve != nullptr)`로 커브 유효성을 검사한다.
            iii. `float Contribution = Rule.DiminishingReturnCurve->GetFloatValue(MatchCount);`
            iv. `float GroupScore = Contribution * Rule.Weight;`
            v.  `TotalSynergyScore += GroupScore;`
        d. **[레벨 태그 결정]** `FGameplayTagContainer ResultingLevelTags;`를 생성한다.
            i.   (가정: `Thresholds` 배열은 `MinScore` 기준으로 내림차순 정렬되어 있음)
            ii.  `Thresholds` 배열을 순회하며 `if (TotalSynergyScore >= Threshold.MinScore)` 조건을 만족하는 첫 번째 `Threshold`를 찾는다.
            iii. 찾았다면, `ResultingLevelTags.AddTag(Threshold.LevelTag);`를 실행하고 루프를 중단한다.
        e. **[결과 반환]** `ResultingLevelTags`를 반환한다.
    3.  **[Future 반환]** `UE::Tasks::Launch`가 반환한 `TFuture<FGameplayTagContainer>` 객체를 즉시 호출자에게 반환한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`CalculateSynergyScore`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: CalculateSynergyScore
      where: "AsyncTask"
      authority: "ServerOnly"
      effects:
        - "For each rule in DT_SynergyGroupRules, compute contribution with DiminishingReturnCurve"
        - "Aggregate and map to thresholds in DT_SynergyLevelThresholds"
    ```
*   `Mp00...기획서.md`: "규칙 기반 평가: `DT_SynergyGroupRules` 데이터 테이블에 정의된 규칙에 따라 수집된 태그들을 평가하여 '시너지 점수'를 계산한다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-03.02"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`CalculateSynergyScore` 구현"
---