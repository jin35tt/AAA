### **Task ID: TASK-07.02**
### **Task: `Fusion_CheckAmp` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `Fusion_CheckAmp` 함수를 구현합니다. 이 함수는 특정 대상의 현재 융합 스택을 `DT_SkillFusionRules`에 정의된 증폭 임계값(`AmpThresholds`)과 비교하여 새로운 증폭 레벨을 결정합니다. 만약 증폭 레벨이 이전과 달라졌을 경우에만, 대상의 증폭 태그를 갱신하고 `STAT_OnFusionAmpChanged_E` 이벤트를 전파합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `Fusion_CheckAmp` 함수에 정확히 반영되어야 한다.
- [ ] 현재 융합 스택에 해당하는 가장 높은 증폭 레벨을 정확히 계산해야 한다.
- [ ] 계산된 신규 증폭 레벨이 기존 레벨과 다를 경우에만 태그 변경 및 이벤트 전파 로직이 실행되어야 한다.
- [ ] 상태 변경 시, 기존 증폭 태그는 제거되고 신규 증폭 태그가 정확히 적용되어야 한다.
- [ ] 상태 변경 시, `STAT_OnFusionAmpChanged_E` 이벤트가 정확한 페이로드(현재 스택, 신규 증폭 레벨, 신규 태그)와 함께 전파되어야 한다.

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
    - [ ] **취소/타임アウト:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[함수 시그니처]** `void Fusion_CheckAmp(AActor* TargetActor, int32 CurrentStack)`
    2.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Fusion_CheckAmp_GameThread)`를 선언한다.
    3.  **[상태 조회]** `TargetActor`를 키로 사용하여 내부 융합 상태 맵(`FusionStatesMap`)에서 기존 상태(`FSTAT_FusionState OldState`)를 가져온다.
    4.  **[규칙 조회]** 관련된 `DT_SkillFusionRules` Row를 찾는다. 실패 시 `ensure` 후 종료한다.
    5.  **[신규 증폭 레벨 계산]**
        a. `int32 NewAmpLevel = 0;`
        b. `Rule.AmpThresholds` 배열을 (내림차순으로) 순회하며, `if (CurrentStack >= Threshold)`를 만족하는 가장 높은 레벨을 `NewAmpLevel`로 설정한다.
    6.  **[상태 변경 감지]** `if (NewAmpLevel != OldState.AmpLevel)`:
        a. **[기존 태그 제거]** `if (OldState.ActiveAmpTag.IsValid())` 이면, `TargetActor`에서 `OldState.ActiveAmpTag`를 제거한다.
        b. **[신규 태그 적용]**
            i.   `FGameplayTag NewAmpTag;`
            ii.  `if (NewAmpLevel > 0)` 이면, `Rule.AmpTags` 배열에서 `NewAmpLevel - 1` 인덱스에 해당하는 태그를 `NewAmpTag`로 설정한다.
            iii. `if (NewAmpTag.IsValid())` 이면, `TargetActor`에 `NewAmpTag`를 추가한다.
        c. **[내부 상태 갱신]** `FusionStatesMap`의 `TargetActor`에 해당하는 상태를 `CurrentStack`, `NewAmpLevel`, `NewAmpTag`로 업데이트한다.
        d. **[이벤트 전파]** 갱신된 `FSTAT_FusionState`를 페이로드로 사용하여 `STAT_OnFusionAmpChanged_E.Broadcast()`를 호출한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`Fusion_CheckAmp`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: Fusion_CheckAmp
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "If thresholds crossed -> set AmpLevel and ActiveAmpTag"
      outputs: [ "STAT_OnFusionAmpChanged_E" ]
    ```
*   `Mp00...기획서.md`: "증폭 상태 전환: `Fusion.Charge.Stack`이 데이터 테이블에 정의된 최대치에 도달하면... `Fusion.State.Amplified` 라는 새로운 상태 태그가 부여된다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-07.02"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Fusion_CheckAmp` 구현"
---