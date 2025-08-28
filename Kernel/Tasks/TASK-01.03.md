### **Task ID: TASK-01.03**
### **Task: `ComputeFinalDamage` 구현**
### **유형: B. 가변 기능 구현**

---
### **1. 목표 (Objective)**
*   `USTATAbilityComponent` 내에 `ComputeFinalDamage` 비동기 함수를 구현합니다. 이 함수는 기본 피해량(Base Damage), 공격 태그(`AttackTags`), 그리고 방어 캐릭터의 모든 관련 스탯 맵을 입력받아, 저항(Resistances) 및 기타 방어 스탯을 적용한 최종 피해량을 백그라운드 스레드에서 계산하고 그 결과를 (`float` 형태의 `TFuture`) 반환합니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `4. 논리적 설계`에 명시된 모든 요구사항이 `USTATAbilityComponent.cpp` 내의 `ComputeFinalDamage` 함수에 정확히 반영되어야 한다.
- [ ] 함수는 `AsyncTask`로 실행되어 게임 스레드를 차단하지 않아야 한다.
- [ ] `AttackTags`에 포함된 피해 유형 태그(예: `Damage.Type.Fire`)를 분석하여, 캐릭터 스탯 맵에서 해당하는 저항 값(예: `Stat.Resistance.Fire`)을 찾아 피해량 감소 공식을 정확히 적용해야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`, `Z1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   **Z1 아키텍처 준수 체크리스트:**
    - [x] **UObject 접근:** 모든 UObject 멤버 접근은 `GameThread` 또는 `GameThread_TimeSliced` 컨텍스트에서만 수행되는가? (본 함수는 UObject 대신 스탯 데이터 복사본을 사용하므로 준수)
    - [ ] **비동기 콜백:** 모든 비동기 람다(lambda) 콜백에 `SAFE_WEAK_LAMBDA`와 같은 약한 참조 캡처가 사용되었는가? (해당 없음)
    - [ ] **시간 분할:** `GameThread_TimeSliced` 작업의 단위 실행 시간은 1ms 예산을 초과하지 않는가? (해당 없음)
    - [ ] **취소/타임아웃:** 장기 실행 `AsyncTask`는 `FCancellationToken` 전파 및 타임아웃 처리를 지원하는가? (본 Task는 단기 실행이므로 해당 없음)
    - [x] **프로파일링:** 모든 주요 Task 범위에 `TRACE_CPUPROFILER_EVENT_SCOPE`가 포함되었는가? (필수 적용)
*   **의사코드:**
    1.  **[함수 시그니처]** `TFuture<float> ComputeFinalDamage(float BaseDamage, const FGameplayTagContainer& AttackTags, const TMap<FGameplayTag, float>& DefenderStats)`로 함수를 정의한다.
    2.  **[비동기 실행]** `UE::Tasks::Launch(UE_SOURCE_LOCATION, ...)`를 사용하여 새 비동기 Task를 실행한다. 람다 내에서 다음을 수행한다:
        a. **[프로파일링 시작]** `TRACE_CPUPROFILER_EVENT_SCOPE(ComputeFinalDamage_AsyncTask)`를 선언한다.
        b. **[초기화]** `float FinalDamage = BaseDamage;`를 선언한다.
        c. **[피해 유형 식별]** `AttackTags`를 순회하여 `Damage.Type.*` 패턴과 일치하는 태그를 찾는다. (예: `Damage.Type.Fire`)
        d. **[저항 태그 변환]** 식별된 피해 유형 태그를 해당하는 저항 스탯 태그로 변환한다. (예: `Damage.Type.Fire` -> `Stat.Resistance.Fire`)
        e. **[저항 값 조회]** `DefenderStats` 맵에서 변환된 저항 스탯 태그를 키로 사용하여 저항 값을 조회한다. 만약 키가 존재하지 않으면 저항 값은 0.0f이다.
        f. **[피해량 계산]** 조회된 저항 값을 사용하여 최종 피해량을 계산한다. (예: `FinalDamage *= (1.0f - ResistanceValue);`)
        g. **[결과 반환]** 계산된 `FinalDamage` 값을 반환한다.
    3.  **[Future 반환]** `UE::Tasks::Launch`가 반환한 `TFuture<float>` 객체를 즉시 호출자에게 반환한다.

---
### **5. 작업 지시 (Instructions)**
1.  `TASK-00`에 의해 생성된 **`/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp`** 파일을 여십시오.
2.  위 **`4. 논리적 설계`**에 명시된 의사코드를 C++로 정확하게 구현하여, **`ComputeFinalDamage`** 함수의 `// TODO` 부분을 완성하십시오.

---
### **6. 예외 처리 프로토콜 (Exception Handling Protocol)**
*   본 Task는 **`B-Type`** 프로토콜을 따릅니다.
*   Task 수행 중 문제 발생 시, 즉시 실행을 중단하고 `2.2. 예외 처리 프로토콜`의 **상태 전이 규칙**에 따라 상태를 변경하고 보고해야 합니다.

### **7. 참조 소스 (Reference from Source)**
*   **핵심 청사진:** `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`
*   **요구사항 컨텍스트:** `Mp01_ProjectAAA 캐릭터 상태 시스템 _01메인 기획서.md`

### **8. 소스 요구사항 매핑 (Source Requirement Mapping)**
*   `M_ProjectAAA 캐릭터 상태 시스템_md_c.md`:
    ```yaml
    - step: ComputeFinalDamage
      where: "AsyncTask"
      authority: "ServerOnly"
      effects:
        - "Apply resistances via tags (Stat.Resistance.*)"
        - "Return FinalDamage"
    ```
*   `Mp01...기획서.md`: "STAT 시스템은 `Stat.Defense.Armor.Physical` 태그와 연계하여 피해량을 계산하도록 합니다."

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-01.03"
  type: code_implementation
  path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  owner: "ai-agent-developer"
  description: "`ComputeFinalDamage` 구현"
---