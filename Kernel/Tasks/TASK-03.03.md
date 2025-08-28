### **Task ID: TASK-03.03**
### **Task: `ApplySynergyLevelTags` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `ApplySynergyLevelTags` 함수를 구현합니다. 이 함수는 `CalculateSynergyScore`로부터 계산된 새로운 시너지 레벨 태그를 입력받아, 캐릭터의 현재 시너지 태그와 비교하여 변경사항(추가/제거)을 식별하고, 이 변경 작업을 `FTimeSlicedTask`를 사용하여 여러 프레임에 걸쳐 안전하게 적용합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `ApplySynergyLevelTags` 함수에 정확히 반영되어야 한다.
- [ ] 함수는 캐릭터의 기존 시너지 태그와 신규 태그를 비교하여 추가할 태그와 제거할 태그를 정확히 식별해야 한다.
- [ ] 식별된 모든 태그 변경 작업이 `FTimeSlicedTask`를 통해 프레임 분산 방식으로 처리되어야 하며, 각 프레임의 작업량은 1ms 예산을 초과하지 않아야 한다.
- [ ] 모든 태그 적용이 완료된 후, 관련 시스템에 상태 변경을 알리는 이벤트가 전파되어야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`, `Z1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 함수는 `GameThread_TimeSliced` 컨텍스트에서 UObject(TagComponent)를 수정하므로 준수)
    - [ ] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEAK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (해당 없음)
    - [x] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (본 설계의 핵심)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(ApplySynergyLevelTags_TimeSliced)`를 선언한다.
    2.  **[기존 작업 취소]** 만약 현재 실행 중인 `ApplySynergyLevelTags` Task가 있다면, 즉시 취소하여 최신 데이터만 적용되도록 보장한다.
    3.  **[변경사항 계산]**
        a. 캐릭터의 태그 컴포넌트에서 현재 적용된 모든 `Synergy.Level.*` 태그(`CurrentSynergyTags`)를 가져온다.
        b. 입력받은 새로운 시너지 태그(`NewSynergyTags`)와 비교한다.
        c. `TagsToRemove = CurrentSynergyTags - NewSynergyTags`
        d. `TagsToAdd = NewSynergyTags - CurrentSynergyTags`
    4.  **[작업 목록 생성]** 추가/제거할 태그들을 하나의 "작업 목록"(`TagUpdateQueue`)으로 만든다. (예: `TQueue<TTuple<FGameplayTag, ETagUpdateOp>>`)
    5.  **[프레임 분산 Task 생성]** `FTimeSlicedTask`를 생성하고 게임 태스크 매니저에 등록한다.
        a. **`StepFunction` (매 프레임 실행될 람다):**
            i.   `if (TagUpdateQueue.IsEmpty()) return false;` // 작업 완료, Task 종료
            ii.  `TagUpdateQueue.Dequeue(CurrentOperation);` // 작업 하나 꺼내기
            iii. `if (CurrentOperation.Op == Add)` 캐릭터의 태그 컴포넌트에 `CurrentOperation.Tag`를 추가한다.
            iv. `else if (CurrentOperation.Op == Remove)` 캐릭터의 태그 컴포넌트에서 `CurrentOperation.Tag`를 제거한다.
            v.  `return true;` // 작업 남음, 다음 프레임에 계속
        b. **`OnDone` (Task 완료 시 실행될 람다):**
            i.   `STAT.Broadcast.Changed` 메시지 버스 이벤트를 전파하여 시너지 레벨 변경을 시스템 전체에 알린다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`ApplySynergyLevelTags`** 함수의 `// TODO` 부분을 완성하십시오.

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
    - step: ApplySynergyLevelTags
      where: "GameThread_TimeSliced"
      authority: "ServerOnly"
      effects:
        - "Grant/Remove Synergy.Level.* tags time-sliced"
      outputs:
        - "STAT.Broadcast.Changed(FSTAT_ChangedPayload)"
    ```
*   `Mp00...기획서.md`: "달성한 레벨에 맞는 `Set.Bonus.LvX` 태그를 캐릭터에게 부여한다. 이 모든 계산 과정은... 프레임 분산 처리되어 게임 성능에 영향을 주지 않는다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-03.03"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`ApplySynergyLevelTags` 구현"
---