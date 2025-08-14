### **`G01_ProjectAAA 게임_어빌리티_캐릭터 상태 시스템 개발 지침서.md` (v4.1 - Master)**

# **Unreal Engine 5.6 어빌리티_캐릭터 상태 시스템 개발 지침서 (v4.1)**

> **문서 상태:** **최종 승인 (Final & Complete Master)**
> **버전:** **v4.1**
> **최종 수정일:** 2025-08-11
>
> **변경 이력:**
> - **v4.1:** 최종 마스터 버전. 전문 형식으로 재구성하고 모든 코드 예시 및 알고리즘 명세 최종 확정.
> - **v4.0:** 핵심 재미 요소(스킬 융합, 동적 세트, 카운터플레이)의 세부 구현 알고리즘 및 데이터 구조 명세화. 이벤트 버스 원칙 추가. 상위 문서의 모든 요구사항을 완전 반영.
> - v3.0: 상위 지침서(A0, A1, G00, Z1, Z2) 및 기획서 요구사항을 반영하여 대대적 개정.
>
> **문서의 목적:**
> 본 문서는 ProjectAAA의 캐릭터 상태(STAT) 시스템 개발을 위한 **최상위 기술 규범이자 최종 설계도**입니다. AI 수석 아키텍트는 이 지침서에 명시된 모든 규칙, 인터페이스, 데이터 구조, 알고리즘을 **변경 없이 그대로 준수하여** 시스템을 구현해야 합니다.

---

## **1. 네이밍 및 코드 스타일 (Z2 준수)**

모듈의 독립성과 가독성을 위해 모든 식별자는 아래 규칙을 엄격히 따릅니다.

| 구분 | 규칙 | 예시 |
| :--- | :--- | :--- |
| **C++ Class/Struct/Enum**| `U`+`STAT`+`PascalCase` | `USTATAbilityComponent`, `FSTAT_Modifier` |
| **Interface** | `I`+`STAT`+`_Descriptor_If` | `ISTAT_Damageable_If`, `ISTAT_Comparable_If` |
| **Data Asset** | `DA_STAT_`+`PascalCase` | `DA_STAT_AttributeSet`, `DA_STAT_DebuffProfile` |
| **Event Dispatcher** | `STAT_`+`OnEventName_E` | `STAT_OnHealthChanged_E` |
| **Gameplay Tag** | `Stat.`+`Category.Sub...` | `Stat.Core.HP.Max`, `Stat.Resistance.Fire` |

---

## **2. 모듈 간 통신 표준**

### **2.1 인터페이스 기반 통신**
`STAT` 시스템의 모든 기능은 외부에서 직접 호출할 수 없으며, 반드시 아래에 정의된 C++ 인터페이스를 통해서만 접근해야 합니다. **다른 플러그인의 헤더 파일을 `#include`하는 행위는 절대 금지됩니다.**

### **2.2 이벤트 버스 원칙 (Z2 준수)**
*   **원칙:** 모듈 간의 결합도를 최소화하기 위해, 중앙 집중식 허브(`BP_InteractionHub`) 외에도 범용적인 메시지 전달을 위해 **`EMessageBus`** 또는 **모듈러 기능(Modular Features)** 사용을 고려합니다.
*   **적용 시나리오:** 특정 게임 월드 이벤트(예: '밤이 되었습니다')에 따라 여러 시스템(AI, 스폰, 스탯)이 동시에 반응해야 할 때, 각 시스템이 서로를 알 필요 없이 공통 메시지 채널을 구독하게 하여 결합도를 원천적으로 차단합니다. `STAT` 시스템은 이러한 범용 이벤트에 반응하여 특정 버프/디버프를 적용할 수 있어야 합니다.

---

## **3. 핵심 아키텍처 및 구현 파이프라인**

### **3.1 Modifier와 출처 추적 (Source Tracking)**
모든 스탯 변경의 근원을 추적하여 '동적 세트 시너지', '지능형 UI' 등 핵심 기능을 지원합니다.

*   **표준 Modifier 구조체 (최종):**
    ```cpp
    USTRUCT(BlueprintType)
    struct FSTAT_Modifier
    {
        GENERATED_BODY()

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat Modifier")
        FGameplayTag TargetStatTag; // 변경할 스탯 (예: Stat.Core.AttackPower)

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat Modifier")
        EModifierOp Operation = EModifierOp::Add; // 연산 방식 (Add, Multiply 등)

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat Modifier")
        float Value = 0.f; // 값

        UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Stat Modifier")
        FGameplayTag SourceTag; // 출처 태그 (예: Source.Item.Weapon, Source.Relic)

        UPROPERTY(BlueprintReadOnly, Category="Stat Modifier")
        TWeakObjectPtr<UObject> SourceObject; // 이 Modifier를 제공한 실제 오브젝트 (DA, Actor 등)
    };
    ```

### **3.2 공격/방어 태그 매칭 시스템**
모든 피해 판정은 이 파이프라인을 따라 처리되어야 합니다.

*   **인터페이스 최종 시그니처 (in `ISTAT_Damageable_If`):**
    ```cpp
    UFUNCTION(BlueprintNativeEvent, Category = "STAT|Damageable")
    void ApplyDamage(float BaseDamage, AActor* Instigator, const FGameplayTagContainer& AttackTags, FName CauseTag);
    ```
*   **구현 의무:** 이 함수는 `6.3 전략적 카운터플레이` 기획을 지원해야 합니다. 보스가 가진 `State.Vulnerable.To.Counter.*` 태그와 `AttackTags`가 매칭될 경우, 피해 적용 로직을 중단하고 관련 시스템에 카운터 성공 이벤트를 전파해야 합니다.

### **3.3 상태 변경 이벤트 페이로드 (Payload)**
외부 시스템이 풍부한 정보에 기반하여 반응하도록, 모든 이벤트는 아래 구조체를 사용합니다.

*   **표준 페이로드 구조체 (최종):**
    ```cpp
    USTRUCT(BlueprintType)
    struct FSTAT_ChangedPayload
    {
        GENERATED_BODY()

        UPROPERTY(BlueprintReadOnly) FGameplayTag StatTag;         // 변경된 스탯 태그
        UPROPERTY(BlueprintReadOnly) float PreviousValue;      // 이전 값
        UPROPERTY(BlueprintReadOnly) float CurrentValue;       // 현재 값
        UPROPERTY(BlueprintReadOnly) float Delta;              // 변화량
        UPROPERTY(BlueprintReadOnly) float MaxValue;           // 최대 값
        UPROPERTY(BlueprintReadOnly) TWeakObjectPtr<AActor> Instigator; // 원인 제공 액터
        UPROPERTY(BlueprintReadOnly) FGameplayTag SourceTag;   // 변경을 유발한 소스 태그
        UPROPERTY(BlueprintReadOnly) TWeakObjectPtr<UObject> SourceObject; // 원인 오브젝트
    };
    ```

---

## **4. 성능 및 비동기 처리 규칙 (Z1 준수)**

### **4.1 `FTimeSlicedTask` 사용 규칙**
대규모 상태 업데이트(예: 광역 DoT, 동적 세트 시너지 재계산)는 반드시 `FTimeSlicedTask`를 통해 프레임 분산 처리해야 합니다.

*   **우선순위/Timeout:** 모든 Task는 `FPriorityTaskQueue`에 등록 시 `ETaskPriority` (High, Normal, Low)를 명시해야 하며, `Timeout`과 `OnTimeout` 콜백을 설정하여 무한 루프 또는 처리 실패 상황을 로깅해야 합니다.
    ```cpp
    FTimeSlicedTask SynergyCalcTask;
    SynergyCalcTask.TaskName = TEXT("CalcDynamicSetSynergy");
    SynergyCalcTask.Timeout = FTimespan::FromSeconds(5);
    SynergyCalcTask.OnTimeout = [this]() { UE_LOG(LogStatSystem, Error, TEXT("Synergy Calculation TIMEOUT!")); };
    // ... StepFunction 정의 후 Queue에 Enqueue
    ```

### **4.2 UObject 람다 캡처 (`SAFE_WEAK_LAMBDA`)**
비동기 작업의 콜백에서 UObject에 접근 시, 댕글링 포인터로 인한 크래시를 방지하기 위해 `SAFE_WEAK_LAMBDA` 매크로 사용을 **의무화**합니다.

```cpp
// SAFE_WEAK_LAMBDA(this) 매크로가 프로젝트 공용 헤더에 정의되어 있다고 가정
void USTATAbilityComponent::RequestRecalculateSynergy()
{
    SAFE_WEAK_LAMBDA(this); // this에 대한 Weak Ptr 생성
    UE::Tasks::Launch(UE_SOURCE_LOCATION, [WeakPtr_this]
    {
        // ... (무거운 계산)
        FCalculatedSynergyData Result;

        AsyncTask(ENamedThreads::GameThread, [WeakPtr_this, Result]()
        {
            if (!WeakPtr_this.IsValid()) return; // 객체가 소멸되었으면 즉시 중단
            WeakPtr_this->ApplySynergyResult(Result);
        });
    });
}
```

---

## **5. 안정성 및 UI 연동 규칙 (A0, A1, G00 준수)**

### **5.1 예외 처리 및 안정성 (A0 준수)**
*   **Assertion:** 개발 중 논리 오류를 조기에 발견하기 위해 `check(Condition)`과 `ensure(Condition)`를 적극 사용합니다. `check`는 실패 시 항상 크래시, `ensure`는 실패 시 에디터에서만 경고를 발생시킵니다.
*   **런타임 안정성:** 모든 포인터 변수는 사용 전 `IsValid()` 체크를 습관화하여 Shipping 빌드에서의 크래시를 원천 차단해야 합니다.

### **5.2 블루프린트 Tooltip 의무화 (A1 준수)**
블루프린트에 노출되는 모든 `UPROPERTY`, `UFUNCTION`에는 `Tooltip` 메타 태그를 사용하여 **반드시 한글로 된 설명**을 추가해야 합니다.
```cpp
/** 캐릭터의 현재 체력입니다. 이 값은 직접 수정할 수 없습니다. */
UPROPERTY(BlueprintReadOnly, Category = "STAT|Core", meta = (Tooltip = "캐릭터의 현재 체력입니다. 이 값은 직접 수정할 수 없습니다."))
float CurrentHP;
```

### **5.3 MVVM 동기화 파이프라인 (G00 준수)**
UI가 모델(`USTATAbilityComponent`)을 직접 참조하는 것을 막고, ViewModel을 통해 안전하게 데이터를 동기화합니다.
1.  **Model 변경:** `USTATAbilityComponent`에서 스탯 변경 발생.
2.  **이벤트 전파:** `STAT_OnStatChanged_E` 이벤트가 `FSTAT_ChangedPayload`와 함께 `BP_InteractionHub`를 통해 전파됨.
3.  **ViewModel 수신/가공:** `USTAT_StatusViewModel`이 이벤트를 수신하여, 자신의 UPROPERTY(예: `HealthPercentage`)를 갱신.
4.  **View 자동 갱신:** `WBP_CharacterStatus` 위젯의 UI 요소들은 ViewModel의 UPROPERTY에 바인딩되어 있으므로, UMG 바인딩 시스템에 의해 자동으로 표시가 갱신됨.

### **5.4 CauseTag 변환 어댑터 (G00 준수)**
다른 플러그인에서 `FGameplayTag`로 전달된 원인 태그를 `STAT` 내부에서 `FName`으로 사용해야 할 경우, `BP_InteractionHub`가 그 변환을 책임지는 **어댑터(Adapter)** 역할을 수행합니다.

---

## **6. 핵심 재미 요소 기술 구현 명세**

### **6.1 동적 세트 시너지 계산 알고리즘**
*   **역할:** `STAT` 시스템은 모든 스탯 Modifier의 `SourceTag`를 기반으로 '동적 세트 시너지' 점수를 계산하고, 결과 레벨에 맞는 보너스를 적용할 책임을 진다.
*   **데이터 테이블:** `DT_SynergyGroupRules`
    | RuleID (FName) | GroupQuery (FGameplayTagQuery) | Weight (float) | DiminishingReturnCurve (CurveFloat) |
    | :--- | :--- | :--- | :--- |
    | `PoisonMastery`| `{ "MatchAny": ["Source.Skill.Poison", "Source.Item.Poison"] }` | 1.0 | `Curve_DR_Default` |
*   **계산 알고리즘 (`CalculateSynergyScore()`):**
    1.  **트리거:** 캐릭터의 장비, 스킬, 버프 상태가 변경될 때마다 비동기 Task로 호출.
    2.  **Modifier 수집:** 현재 캐릭터에 적용된 모든 `FSTAT_Modifier` 목록을 가져온다.
    3.  **그룹별 점수 계산:** `DT_SynergyGroupRules`의 각 행(Rule)을 순회한다.
        a. 현재 Modifier 목록에서 `GroupQuery` 조건에 맞는 Modifier들을 필터링한다.
        b. 필터링된 Modifier의 개수를 `DiminishingReturnCurve`에 입력하여 최종 기여도(Contribution)를 얻는다. (예: 1개=1.0, 2개=1.6, 3개=1.9)
        c. `최종 기여도 * Weight` 로 해당 그룹의 점수를 계산한다.
    4.  **최종 점수 합산 및 레벨 변환:** 모든 그룹의 점수를 합산하여 `DT_SynergyLevelThresholds` 테이블과 비교, `Synergy.Level.1` 등의 태그를 캐릭터에게 부여/제거한다.

### **6.2 스킬 융합/증폭 지원 로직**
*   **역할:** `STAT` 시스템은 '융합 증폭'의 전제 조건인 **'융합 에너지 스택'**을 관리하고, `SKIL` 시스템이 이를 조회하여 증폭된 융합을 실행할 수 있도록 지원한다.
*   **데이터 구조 (in `USTATAbilityComponent`):**
    `TMap<TWeakObjectPtr<AActor>, int32> FusionChargeOnActor;`
    `TMap<FVector, int32> FusionChargeOnLocation;`
*   **융합 증폭 스택 처리 알고리즘:**
    1.  **입력:** `ApplyDamage` 인터페이스 호출 시, 피격 정보를 받는다.
    2.  **스택 누적:** 피격 대상이 액터일 경우 `FusionChargeOnActor` 맵에서, 장판기처럼 위치 기반일 경우 `FusionChargeOnLocation` 맵에서 스택을 1 증가시킨다.
    3.  **증폭 상태 전환:** 스택이 임계값(예: 10)에 도달하면, 해당 Actor/위치에 `Fusion.State.Amplified` 태그를 부여하고 스택을 0으로 초기화한다.
    4.  **정보 제공:** `SKIL` 시스템이 융합 판정 시 `ISTAT_FusionSupport_If::IsAmplified(TargetActor/Location)` 인터페이스를 통해 증폭 상태를 조회할 수 있도록 한다.

### **6.3 전략적 카운터플레이 지원 시나리오**
*   **역할:** `STAT` 시스템은 다단계 카운터 및 성공/실패 시나리오를 처리할 수 있는 상태 관리 기능을 제공한다.
*   **상태 정의:** `ECounterState` { `None`, `Vulnerable`, `Success`, `Fail` } 상태 Enum을 정의하여 캐릭터 상태를 관리한다.
*   **카운터 시나리오 처리 로직:**
    1.  **취약 상태 진입:** 보스가 특정 패턴 사용 시, 스스로 `ISTAT_Counterable_If::EnterVulnerableState(CounterTriggerTag)`를 호출한다. `CounterTriggerTag`는 이 패턴을 카운터할 수 있는 공격의 태그(예: `Counter.Type.Stun`)이다.
    2.  **카운터 시도:** 플레이어의 공격(`AttackTags`)이 `ApplyDamage`를 통해 전달된다.
    3.  **판정:**
        a. **(성공):** `AttackTags`가 `CounterTriggerTag`와 일치하면, 보스의 상태를 `ECounterState::Success`로 변경하고, '그로기' 버프를 적용하며, 보상 드랍 등 관련 이벤트를 전파한다.
        b. **(실패 - 다른 공격):** `AttackTags`가 일치하지 않으면, 상태를 `ECounterState::Fail`로 변경하고, 즉시 취약 상태를 해제하며 '카운터 실패' 로직(예: 즉시 광역 반격)을 트리거할 수 있다.
        c. **(실패 - 시간 초과):** 일정 시간 내에 카운터 시도가 없으면, 상태는 `ECounterState::None`으로 자동 복귀된다.

---

## **7. 개발 및 테스트 체크리스트 (v4.1)**

*   [ ] **(완료)** 모든 C++/BP 식별자에 **네이밍 컨벤션** 적용.
*   [ ] **(완료)** 동적 세트 시너지 계산 알고리즘(가중치, 점감 곡선) 구현 및 단위 테스트 완료.
*   [ ] **(완료)** 융합 증폭 스택 누적/초기화 및 증폭 상태 태그 부여/제거 로직 구현 및 단위 테스트 완료.
*   [ ] **(완료)** 전략적 카운터플레이를 위한 상태(Vulnerable, Success, Fail) 관리 및 시나리오 처리 로직 구현 및 단위 테스트 완료.
*   [ ] **(완료)** 범용 게임 이벤트에 반응하기 위한 `EMessageBus` 구독 및 처리 로직 예시 구현.
*   [ ] **(완료)** 모든 비동기 콜백에 `SAFE_WEAK_LAMBDA` 적용.
*   [ ] **(완료)** 모든 BP 노출 변수/함수에 한글 Tooltip 추가.
*   [ ] **(완료)** `ensure`, `check` 등 예외 처리 규칙 적용.
*   [ ] **(완료)** `STAT_Test.umap` 테스트 맵에 상기 모든 기능에 대한 `FunctionalTest` 자동화 테스트 배치 및 통과 확인.