### **Task ID: TASK-06.03**
### **Task: `Overload_End` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `Overload_End` 함수를 구현합니다. 이 함수는 과부하 지속 시간 타이머가 만료되었을 때 호출되며, 과부하 상태를 종료하고 모든 관련 버프/페널티를 제거한 뒤, `FOverloadParameters`에 명시된 '번아웃' 디버프를 지정된 시간 동안 적용합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `Overload_End` 함수에 정확히 반영되어야 한다.
- [ ] `bIsOverloading` 상태 플래그가 `false`로 설정되어야 한다.
- [ ] `Source.Overload` 태그를 가진 모든 Modifier와 `State.Overload.Active` 태그가 캐릭터에서 제거되어야 한다.
- [ ] 모든 과부하 관련 타이머(`OverloadEndTimerHandle`, `OverloadTickTimerHandle`)가 중지되어야 한다.
- [ ] `BurnoutDebuffTag`와 `BurnoutDuration`에 따른 번아웃 디버프 Modifier가 캐릭터에 적용되어야 한다.
- [ ] `STAT_OnOverloadStateChanged_E` 이벤트가 `bIsOverloading = false` 상태로 전파되어야 한다.

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
    1.  **[함수 시그니처]** `void Overload_End(FGameplayTag BurnoutDebuffTag, float BurnoutDuration)`로 정의한다 (`Overload_Start`에서 타이머 설정 시 파라미터를 넘겨받음).
    2.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(Overload_End_GameThread)`를 선언한다.
    3.  **[가드 로직]** `if (!bIsOverloading)` 이면, 즉시 함수를 종료한다.
    4.  **[상태 종료]** `bIsOverloading = false;`
    5.  **[타이머 정리]**
        a. `GetWorld()->GetTimerManager().ClearTimer(OverloadEndTimerHandle);`
        b. `GetWorld()->GetTimerManager().ClearTimer(OverloadTickTimerHandle);`
    6.  **[기존 효과 제거]**
        a. `Source.Overload` 태그를 가진 모든 `FSTAT_Modifier`를 제거한다.
        b. 캐릭터의 태그 컴포넌트에서 `State.Overload.Active` 태그를 제거한다.
    7.  **[번아웃 적용]**
        a. `if (BurnoutDebuffTag.IsValid() && BurnoutDuration > 0)` 조건 확인.
        b. `BurnoutDebuffTag`를 기반으로 데이터 테이블 등에서 디버프 내용을 조회하여, `Source.Overload.Burnout` 태그를 가진 임시 `FSTAT_Modifier`를 생성하고 적용한다. 이 Modifier는 `BurnoutDuration` 이후 자동으로 소멸되어야 한다.
    8.  **[이벤트 전파]**
        a. `FSTAT_OverloadPayload Payload; Payload.bIsOverloading = false; Payload.TimeRemaining = 0;`
        b. `STAT_OnOverloadStateChanged_E.Broadcast(Payload);`
    9.  **[스탯 최종 갱신]** 영향을 받은 모든 스탯(Overload 효과 제거 + 번아웃 효과 적용)을 재계산하고 관련 `STAT_OnStatChanged_E` 이벤트를 전파한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`Overload_End`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: Overload_End
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Clear Overload buffs; apply BurnoutDebuffTag for BurnoutDuration"
        - "Unset State.Overload.Active"
    ```
*   `Mp00...기획서.md`: "지정된 `Duration`이 지나면 모든 효과를 제거하고, `BurnoutDebuffProfile`에 명시된 번아웃 디버프를 적용한다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-06.03"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`Overload_End` 구현"
---