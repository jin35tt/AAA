### **`G03_ProjectAAA 게임_어빌리티_스킬 시스템 개발 지침서.md` (v2.0 - Master)**

# **Unreal Engine 5.6 어빌리티_스킬 시스템 개발 지침서 (v2.0)**

> **문서 상태:** **최종 승인 (Final & Complete Master)**
> **버전:** **v2.0**
> **최종 수정일:** 2025-08-11
>
> **변경 이력:**
> - **v2.0:** **최종 마스터 버전.** 상위 지침서(A0, A1, G00, Z1, Z2) 및 기획서(스킬 GDD, 전투재미)의 모든 요구사항을 **완전 반영**.
>   - 조합형 스킬, 스킬 융합/증폭, 과부하, 카운터플레이 등 핵심 기능의 기술 구현 명세 추가.
>   - 네이밍, 예외처리, 비동기 규칙, 상세 피드백(VFX/SFX) 등 모든 누락 항목 보강.
> - v1.1: 데이터/로직 분리 구조 및 기본 아키텍처 정의.
>
> **문서의 목적:**
> 본 문서는 ProjectAAA의 스킬(SKIL) 시스템 개발을 위한 **최상위 기술 규범이자 최종 설계도**입니다. AI 수석 아키텍트는 이 지침서에 명시된 모든 규칙을 **변경 없이 그대로 준수하여** 시스템을 구현해야 합니다.

---

## **1. 네이밍 및 코드 스타일 (Z2 준수)**

| 구분 | 규칙 | 예시 |
| :--- | :--- | :--- |
| **C++ Class/Struct/Enum**| `U`+`SKIL`+`PascalCase` | `USKIL_SkillComponent`, `FSKIL_Context` |
| **Interface** | `I`+`SKIL`+`_Descriptor_If` | `ISKIL_Owner_If` |
| **Data Asset** | `DA_SKIL_`+`PascalCase` | `DA_SKIL_CorePack_Projectile`, `DA_SKIL_Rune_Fire` |
| **Event Dispatcher** | `SKIL_`+`OnEventName_E` | `SKIL_OnSkillUsed_E` |
| **Gameplay Tag** | `Skill.`+`Category.Sub...` | `Skill.Type.Projectile`, `Skill.Element.Fire` |
| **Blueprint 변수/함수** | `PascalCase` (동사로 시작) | `ExecuteSkill`, `bCanUseSkill` |

---

## **2. 모듈 간 통신 표준**

### **2.1 인터페이스 및 이벤트 중계 (G00 준수)**
*   `STAT` 시스템과의 상호작용은 반드시 `ISTAT_*_If` 인터페이스를 통해서만 이루어집니다.
*   `SKIL` 시스템 내부 이벤트는 `BP_InteractionHub`를 통해 중계하는 것을 원칙으로 합니다.

### **2.2 범용 메시징 (Z2 준수)**
*   **원칙:** `SKIL` 시스템과 직접 관련 없는 범용 게임 이벤트에 반응해야 할 경우, `EMessageBus`의 공통 채널을 구독하여 처리합니다.
*   **예시:** '특정 보스 2페이즈 시작' 메시지를 수신하면, '과부하' 스킬의 쿨다운을 초기화하는 로직을 구현할 수 있습니다.

---

## **3. 핵심 아키텍처 및 데이터 구조**

### **3.1 데이터 중심 설계: 조합형 커스텀 스킬**
스킬은 `코어 팩(설계도)`, `메인 룬(핵심 재료)`, `옵션 파츠(부가 재료)` 3가지 데이터 에셋의 조합으로 생성됩니다.

*   **`DA_SKIL_CorePack`:** 스킬의 기본 형태와 파츠 슬롯 수를 정의. (예: `DA_SKIL_CorePack_Projectile`, `DA_SKIL_CorePack_Aura`)
*   **`DA_SKIL_MainRune`:** 스킬의 핵심 효과와 성장 데이터를 정의. (예: `DA_SKIL_Rune_FireDamage`)
*   **`DA_SKIL_OptionPart`:** 부가 효과를 정의. (예: `DA_SKIL_Part_MultiShot`)

### **3.2 스킬 로직 추상화 (`USKIL_ExecutionLogic`)**
스킬의 실제 행동은 `USKIL_ExecutionLogic`을 상속받는 블루프린트 클래스에서 구현됩니다.

```cpp
// C++ Base
UCLASS(Abstract, Blueprintable)
class USKIL_ExecutionLogic : public UObject
{
    GENERATED_BODY();
public:
    // 스킬의 모든 정보(조합된 파츠, 시전자, 대상 등)를 담는 컨텍스트 구조체
    UPROPERTY(BlueprintReadOnly, Category="Skill", meta=(ExposeOnSpawn=true))
    FSKIL_Context SkillContext;

    UFUNCTION(BlueprintNativeEvent, meta=(Tooltip="이 스킬의 실제 행동 로직을 여기에 구현합니다."))
    void Execute();
};
```

---

## **4. 성능 및 안정성**

### **4.1 비동기 처리 (Z1 준수)**
*   **규칙:** 다수의 투사체를 생성하거나, 수십 명에게 영향을 미치는 광역 DoT 로직 등은 `FTimeSlicedTask`를 통해 분산 처리해야 합니다.
*   **우선순위/Timeout:** Task 등록 시 반드시 `ETaskPriority`를 명시하고, `Timeout`과 `OnTimeout` 콜백을 설정하여 실패 상황을 로깅해야 합니다.

### **4.2 예외 처리 및 안정성 (A0 준수)**
*   `check(SkillContext.Instigator != nullptr)`: 시전자가 없는 스킬은 논리적 오류이므로 `check`를 사용합니다.
*   `ensure(TargetActor->Implements<ISTAT_Damageable_If>())`: 피해 적용 전, 대상이 피해를 받을 수 있는 인터페이스를 가졌는지 `ensure`로 확인하여 개발 중 문제를 조기에 발견합니다.

### **4.3 블루프린트 Tooltip 의무화 (A1 준수)**
블루프린트에 노출되는 모든 UPROPERTY, UFUNCTION에는 `Tooltip` 메타 태그를 사용하여 **반드시 한글로 된 설명**을 추가해야 합니다.

---

## **5. 핵심 재미 요소 기술 구현 및 피드백 명세**

### **5.1 스킬 융합 및 증폭 (Skill Fusion & Amplification)**
*   **역할:** `SKIL` 시스템은 **'융합'의 최종 실행자**입니다. `STAT` 시스템이 관리하는 '융합 증폭' 상태를 확인하여 일반/증폭 융합을 결정합니다.
*   **데이터 구조 (`DA_SKIL_Base` 확장):**
    ```cpp
    // 모든 스킬 파츠 데이터 에셋에 아래 속성 추가
    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fusion")
    FGameplayTag FusionBaseTag; // 이 스킬이 '베이스'를 생성할 경우 (예: Fusion.Base.Fire)

    UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Fusion")
    FGameplayTag FusionTriggerTag; // 이 스킬이 '트리거'일 경우 (예: Skill.Type.Tornado)
    ```
*   **융합 판정 알고리즘 (`USKIL_SkillComponent::TryFusion()`):**
    1.  `트리거` 스킬 사용 시, 대상 지역/액터가 `Fusion Volume Manager`에 등록된 `베이스`와 겹치는지 확인합니다.
    2.  겹칠 경우, `DT_SkillFusionRules` 데이터 테이블에서 `베이스 태그` + `트리거 태그` 조합에 맞는 `Result_ExecutionLogic`을 조회합니다.
    3.  **증폭 확인:** `ISTAT_FusionSupport_If::IsAmplified()` 인터페이스를 호출하여 대상이 '증폭 상태'인지 확인합니다.
    4.  증폭 상태라면 `Result_ExecutionLogic_Amplified`를, 아니라면 일반 `Result_ExecutionLogic`을 실행합니다.
*   **피드백 상세:**
    *   **융합 힌트 (UI):** 스킬 슬롯 UI에서, `DT_SkillFusionRules`를 실시간으로 조회하여 융합 가능한 스킬 쌍에 동일한 색상의 테두리나 연결선(VFX)을 표시합니다.
    *   **융합 성공 (VFX/SFX):** 일반 스킬과 확연히 다른, 두 원소(예: 불, 바람)가 섞이는 듯한 시각 효과와 웅장한 효과음을 재생하여 성공을 명확히 인지시킵니다.

### **5.2 과부하 (Overload) 시스템**
*   **역할:** `SKIL` 시스템은 플레이어가 조합한 파츠들을 기반으로 **`FOverloadParameters`를 최종 완성하여 `STAT` 시스템에 '요청'**하는 주체입니다.
*   **`FOverloadParameters` 최종 구조체 정의 (SKIL -> STAT 전달용):**
    ```cpp
    USTRUCT(BlueprintType)
    struct FSKIL_OverloadParameters
    {
        GENERATED_BODY()

        // 비용 관련
        UPROPERTY(BlueprintReadOnly) EOverloadCostType CostType;
        UPROPERTY(BlueprintReadOnly) float CostMinPercent;
        UPROPERTY(BlueprintReadOnly) float CostMaxPercent;

        // 효과 관련
        UPROPERTY(BlueprintReadOnly) float Duration;
        UPROPERTY(BlueprintReadOnly) TMap<FGameplayTag, float> StatModifiers;

        // 페널티 관련
        UPROPERTY(BlueprintReadOnly) float DamageTakenMultiplier;
        UPROPERTY(BlueprintReadOnly) TSoftObjectPtr<DA_STAT_DebuffProfile> BurnoutDebuffProfile; // TObjectPtr -> TSoftObjectPtr로 변경하여 강결합 방지
    };
    ```

### **5.3 전략적 카운터플레이 지원**
*   **역할:** `SKIL` 시스템은 카운터 성공 시의 **'보상'을 실행**하는 역할을 담당합니다.
*   **보상 로직:**
    1.  `BP_InteractionHub`로부터 '카운터 성공' 이벤트를 수신합니다.
    2.  `USKIL_SkillComponent`는 현재 쿨다운 중인 스킬 목록 중 하나를 무작위로 선택하여 `ResetCooldown()`을 호출합니다.
    3.  **피드백 (UI):** 쿨다운이 초기화된 스킬 아이콘을 화면 중앙에 잠시 팝업으로 띄우고, "쿨다운 초기화!" 텍스트를 함께 표시합니다.

### **5.4 스킬 시전 피드백**
*   **마나 부족/쿨다운/시전불가:** 기획서에 명시된 대로 아이콘의 색상 변화(붉은색 점멸, 회색 비활성화) 및 숫자 오버레이로 명확히 피드백합니다. **별도의 실패 사운드는 재생하지 않습니다.**

---

## **6. 개발 및 테스트 체크리스트 (v2.0)**

*   [ ] **(완료)** 모든 식별자에 **네이밍 컨벤션** 완전 적용.
*   [ ] **(완료)** 상위 지침서의 **예외 처리, 비동기, 메시징, Tooltip** 규칙 모두 반영.
*   [ ] **(완료)** **조합형 스킬** 제작(`코어 팩`+`메인 룬`+`옵션 파츠`) 로직 및 UI 연동 기능 구현.
*   [ ] **(완료)** **스킬 융합 및 증폭**을 위한 `Fusion Tag` 조회, `ExecutionLogic` 실행, 증폭 상태 확인 로직 구현 완료.
*   [ ] **(완료)** **과부하 스킬**의 `FOverloadParameters` 생성 및 `STAT` 시스템 요청 로직 구현.
*   [ ] **(완료)** **전략적 카운터플레이** 성공 시 쿨다운 초기화 및 UI 피드백 로직 구현.
*   [ ] **(완료)** **시전 실패 피드백** (색상 점멸, 비활성화) UI 로직 구현.
*   [ ] **(완
완료)** `SKIL_Test.umap` 테스트 맵에 상기 모든 기능에 대한 `FunctionalTest` 자동화 테스트 배치 및 통과 확인.