### **`DOC-STAT-GDD-MAIN` (v1.6 - Final-Master)**

# **ProjectAAA: 캐릭터 상태 시스템 메인 기획서 (v1.6 - 최종 마스터)**

## **1. 문서 개요**

| 항목 | 내용 |
| :--- | :--- |
| **문서 ID** | DOC-STAT-GDD-MAIN |
| **문서명** | 캐릭터 상태 시스템 메인 기획서 (Character Status System Game Design Document) |
| **버전** | **1.6 (Final & Approved)** |
| **담당자** | PM/AI |
| **목적** | ProjectAAA의 어빌리티 시스템 전체의 기반이 되는 캐릭터 상태 및 상호작용 규칙을 최종 정의한다. 이 문서는 MGI의 모든 아키텍처 원칙을 준수한다. |
| **변경 이력**| **v1.6:** MGI 아키텍처 원칙(플러그인화, 이벤트 중계, 비동기 의무) 명시. 최종 마스터 버전. |
| | v1.5: '9. 핵심 상호작용 모델: 공격/방어 태그 매칭 시스템' 추가. |
| | **v1.4:** '8. 연동 시스템 심화 요구사항' 섹션 추가. |
| | **v1.3:** '스탯 업그레이드' 아키텍처 및 'UI 스탯 미리보기' 기능 명세 추가. |
| | **v1.2:** '7. UI 연동 및 표시 명세' 섹션 추가. |
| | **v1.1:** '태그 기반 동적 스탯' 및 '태그 위임 모델' 아키텍처 도입. |

## **2. 시스템 개요 및 비전**

### **2.1 시스템 정의**
'캐릭터 상태 시스템(이하 STAT 시스템)'은 게임 내 모든 액터의 핵심 능력치(Attribute)와 그 변화를 총괄 관리하는 **독립된 기능 모듈**입니다. 핵앤슬래시 슈팅 RPG 장르의 특성상 발생하는 수많은 피해 계산, 상태 이상 적용, 스탯 변동을 안정적이고 효율적으로 처리하는 **어빌리티 시스템의 중앙 데이터 허브** 역할을 수행합니다.

### **2.2 설계 비전**
> **"모든 것은 태그다. 태그로 정의하고, 태그로 소통하며, 태그로 반응한다."**

STAT 시스템은 `G01` 지침서의 기술적 원칙을 기획적으로 구체화합니다. 디자이너는 코드를 직접 수정하지 않고 **게임플레이 태그(Gameplay Tag)와 데이터 테이블 수정만으로 새로운 능력치를 창조하고, 캐릭터의 특성을 만들며, 밸런싱**할 수 있어야 합니다. 아이템, 스킬 등 다른 시스템은 STAT 시스템의 내부를 알 필요 없이, 약속된 **태그를 통해 'Stat.Damage.Fire' 능력치를 변경해달라 요청**하고, STAT 시스템은 그 결과를 **구조화된 이벤트(Payload)로 외부에 전파**하여 UI, 이펙트, 사운드가 반응하게 만듭니다.

### **2.3 상위 아키텍처 원칙 (Overarching Architectural Principles)**
본 시스템은 ProjectAAA의 최상위 개발 지침(MGI)을 절대적으로 준수하며, 다음 원칙에 따라 설계 및 구현되어야 한다.

*   **아키텍처 원칙 (A1/Z2 준수):** STAT 시스템은 독립적인 **Game Feature Plugin**으로 제공하며, 엔진 코어 및 다른 플러그인과의 C++ 레벨 강결합을 금지한다.
*   **이벤트 중계 (G00_1/G00_2 준수):** `USTATAbilityComponent`에서 발생하는 모든 `STAT_*` 이벤트 디스패처는 반드시 중앙 허브인 **`BP_InteractionHub`**를 통해 중계되어, 다른 시스템이 안전하게 구독할 수 있도록 해야 한다.
*   **성능 규칙 (Z1 준수):** 다수의 액터를 대상으로 하는 대량 스탯 재계산, 광역 도트(DoT) 피해 처리 등 단일 프레임에 심각한 부하를 유발할 수 있는 모든 로직은 **`Z1: 비동기+프레임 분산` 아키텍처를 의무적으로 적용**하여 처리해야 한다.


## **3. 핵심 아키텍처: 태그 기반 동적 스탯 시스템**

### **3.1 스탯의 정의: Gameplay Tag**
게임에 존재하는 모든 스탯(기본 능력치, 저항, 특수 능력치 등)은 **하드코딩된 변수가 아닌, 게임플레이 태그(Gameplay Tag)로 정의되고 관리됩니다.**

*   **스탯 생성 워크플로우:**
    1.  **태그 추가:** 디자이너가 언리얼 에디터의 GameplayTag 에디터에서 `Stat.Core.HP.Max` 또는 `Stat.Resistance.Fire`와 같은 새 태그를 추가합니다.
    2.  **규칙 정의:** `DT_StatDefinition` 데이터 테이블에 해당 태그를 키(Key)로 하여, 스탯의 기본 정보(한글명, 설명, UI 그룹)와 **성장 규칙**을 정의합니다.
    3.  **시스템 인지:** 이 작업만으로 STAT 시스템은 새로운 능력치의 존재를 인지하고, 모든 캐릭터는 해당 능력치를 가질 수 있는 기반을 갖추게 됩니다.

*   **성장 규칙 데이터 구조 (in `DT_StatDefinition`):**

| 컬럼명 | 데이터 타입 | 설명 |
| :--- | :--- | :--- |
| `StatTag` | `GameplayTag` | 이 행이 정의하는 스탯의 태그 (예: `Stat.Core.HP.Max`) |
| `DisplayName` | `Text` | UI에 표시될 이름 (예: "최대 체력") |
| **`CanBeUpgraded`** | **`bool`** | **이 스탯이 자원을 소모하여 업그레이드 가능한지 여부. 디자이너가 스탯별로 직접 제어.** |
| `GrowthValueCurve` | `CurveFloat` | 투자 포인트 대비 스탯의 실제 증가 값을 정의하는 커브 |
| `RequiredPointCurve`| `CurveFloat` | 스탯 레벨업에 필요한 포인트의 양을 정의하는 커브 |

### **3.2 능력치 적용 및 변경**
캐릭터가 가지는 모든 스탯 값은 내부적으로 `TMap<FGameplayTag, FStatData>` 형태로 관리됩니다. 아이템, 스킬, 버프 등 모든 외부 시스템은 **오직 태그를 이용해** 특정 스탯에 접근하고 값을 변경합니다.

- **예시:** 화염 저항력을 50만큼 증가시키는 버프는 `Stat.Resistance.Fire` 태그를 대상으로 `+50`의 Modifier를 적용하라고 시스템에 요청합니다. 이는 특정 클래스나 변수를 몰라도 되게 만들어 시스템 간의 결합도를 원천적으로 차단합니다.

### **3.3 스탯 업그레이드 기능**
플레이어는 특정 자원(예: '성장 포인트')을 소모하여 `DT_StatDefinition`에 `CanBeUpgraded`가 `true`로 설정된 스탯의 레벨을 올릴 수 있습니다.

*   **업그레이드 프로세스:**
    1.  **UI 요청:** 플레이어가 UI에서 '공격력' 스탯의 업그레이드 버튼을 누릅니다.
    2.  **인터페이스 호출:** UI는 `I_STAT_Upgradable_If::RequestUpgradeStat(StatTagToUpgrade)` 인터페이스 함수를 호출합니다.
    3.  **내부 검증:** `USTATAbilityComponent`는 다음을 검증합니다.
        *   해당 스탯이 `DT_StatDefinition`에서 업그레이드 가능한지 (`CanBeUpgraded == true`).
        *   플레이어가 요구되는 자원(`RequiredPointCurve` 참조)을 충분히 가지고 있는지 (별도의 재화 시스템과 연동).
    4.  **자원 소모 및 스탯 적용:** 검증 통과 시, 자원을 소모하고 `GrowthValueCurve`를 참조하여 해당 스탯의 기본 값(Base Value)에 영구적인 Modifier를 추가합니다.
    5.  **이벤트 전파:** `STAT_OnStatChanged_E` 이벤트를 전파하여 모든 관련 시스템(UI 등)이 갱신되도록 합니다.

## **4. 주요 기능 명세**

### **4.1 상태 변화 (Status Changes)**
피해, 회복, 버프, 디버프의 기본 메커니즘은 모두 Gameplay Tag를 기반으로 이루어집니다.

*   **피해 적용 예시:** `I_STAT_Damageable_If::ApplyDamage(Amount, Instigator, DamageTypeTag, CauseTag)`
    *   `DamageTypeTag` (예: `Damage.Type.Physical`)를 추가하여, STAT 시스템이 내부적으로 `Stat.Defense.Armor.Physical` 태그와 연계하여 피해량을 계산하도록 합니다.

### **4.2 특수 능력 부여: 태그 기반 위임 모델**
**STAT 시스템은 '조건 충족' 여부만 판단하고, 실제 행동은 다른 시스템에 위임합니다.** 이는 각 시스템의 단일 책임 원칙(SRP)을 완벽하게 지키는 핵심 규칙입니다.

*   **원칙:**
    1.  **조건 감지:** STAT 시스템은 캐릭터의 상태, 장착 아이템 수/종류(ITEM 시스템으로부터 정보를 받음) 등 복합적인 조건을 지속적으로 감지합니다.
    2.  **상태 태그 부여:** '방어구 6개 + 반지 3개' 같은 특정 조건이 만족되면, 이 시스템은 캐릭터에게 `Ability.CanActivate.Shield`와 같은 **상태 태그를 부여하고 자신의 임무를 마칩니다.**
    3.  **행동 위임:** 실제 '보호막' 스킬의 로직을 가진 **SKIL 시스템**이 해당 태그(`Ability.CanActivate.Shield`)의 존재를 감지하고, 관련 스킬을 활성화/비활성화합니다.

## **5. 외부 시스템 연동 방안**

| 연동 대상 시스템 | 상호작용 시나리오 | 통신 방식 (태그 중심) |
| :--- | :--- | :--- |
| **아이템(ITEM)** | '화염 저항의 반지' 장착 | ITEM 시스템이 `I_STAT_Buffable_If` 호출 → `Stat.Resistance.Fire` 태그에 `+30` Modifier 적용 요청. |
| | **(복합 조건)** '수호자 세트' 6 부위를 모두 장착. | ITEM 시스템이 장착 상태 변경을 알림 → STAT 시스템이 조건 충족을 확인 → 캐릭터에 `Ability.CanActivate.GuardianSetBonus` 태그 부여. |
| **스킬(SKIL)** | '수호자 세트 보너스' 스킬(예: 1분마다 보호막)이 활성화됨. | SKIL 시스템이 플레이어의 `Ability.CanActivate.GuardianSetBonus` 태그를 감지하고, 해당 스킬 로직을 활성화. |
| | 파이어볼이 몬스터에게 명중. | SKIL 시스템이 `I_STAT_Damageable_If` 호출 → `Damage.Type.Magic` 태그와 함께 피해량 전달 → STAT 시스템은 `Stat.Defense.Armor.Magic` 태그를 참조하여 최종 피해 계산. |
| **UI** | 플레이어의 화염 저항력이 변동됨. | `STAT_OnStatChanged_E` 이벤트에 `Stat.Resistance.Fire` 태그와 변경 값이 담긴 Payload 전달 → UI는 이를 수신하여 캐릭터 정보창 업데이트. |

## **6. 디자이너 워크플로우**

1.  **신규 스탯 '냉기 저항' 추가:**
    -   GameplayTag 에디터에서 `Stat.Resistance.Cold` 태그 생성.
    -   `DT_StatDefinition` 데이터 테이블에 새 행을 추가하고, 태그, 이름, 설명, 성장 규칙 등을 정의.
    -   **이것으로 시스템 개발은 완료됩니다.**
2.  **'냉기 저항' 아이템 제작:**
    -   ITEM 시스템 기획에 따라, 아이템 데이터 에셋에 `Stat.Resistance.Cold` 태그에 `+20`의 Modifier를 부여하는 옵션을 추가.
3.  **테스트:**
    -   플레이어가 해당 아이템을 장착/해제할 때, 디버그 창에서 `Stat.Resistance.Cold` 값이 정상적으로 변동되는지 확인.

## **7. UI 연동 및 표시 명세**

### **7.1 UI 연동 아키텍처: MVVM (Model-View-ViewModel)**
모든 UI 업데이트는 `G00_1`에서 정의한 MVVM 패턴을 절대적으로 따릅니다. 이는 UI(View)와 게임 로직(Model) 간의 의존성을 완전히 제거하여, 각각 독립적으로 개발하고 수정할 수 있게 하기 위함입니다.

*   **Model (모델):** `USTATAbilityComponent` 자체. 모든 실제 데이터와 상태 변화 로직을 가집니다.
*   **ViewModel (뷰모델):** `USTAT_StatusViewModel`. `USTATAbilityComponent`로부터 필요한 데이터를 받아, UI가 표시하기 좋은 형태로 가공하여 **자신이 직접 가지고 있는** UPROPERTY 변수들(예: `HealthPercentage`, `CurrentAttackPower`)을 갱신합니다. **뷰모델은 뷰(UI)를 전혀 알지 못합니다.**
*   **View (뷰):** `WBP_CharacterStatus`, `WBP_HUD`. 뷰모델의 UPROPERTY 변수들을 직접 바인딩(결합)하여 화면에 표시합니다. **뷰는 뷰모델의 데이터가 변경되면 자동으로 업데이트될 뿐, 직접 데이터를 요청하거나 계산하지 않습니다.**

### **7.2 핵심 UI 요소 및 ViewModel 속성**

| UI 요소 | 표시 내용 | ViewModel 속성 (예시) | 데이터 타입 | 업데이트 트리거 |
| :--- | :--- | :--- | :--- | :--- |
| **HUD 체력/마나 바** | 현재 체력/마나 비율 | `HealthPercentage` / `ManaPercentage` | `float` (0.0~1.0) | `STAT_OnStatChanged_E` (`Stat.Core.HP.Current` 태그) |
| **캐릭터 정보창** | 모든 스탯 목록과 현재 값. **스탯 업그레이드 버튼.** | `TArray<FUI_StatDisplayInfo>` | `struct` | 캐릭터 정보창 열릴 때 & `STAT_OnStatChanged_E` (모든 스탯 태그) |
| **장비/스킬 툴팁** | **장착/선택 시 변동될 스탯 미리보기** | `TArray<FUI_StatChangePreview>` | `struct` | 플레이어가 인벤토리나 스킬 아이콘에 마우스 호버 시 |
| **데미지 플로터** | 피해량/회복량 숫자 표시 | (UI 시스템이 직접 처리) | (N/A) | `STAT_OnHealthChanged_E` 이벤트의 **Payload**를 직접 수신 |
| **버프/디버프 아이콘**| 현재 적용된 효과 아이콘과 남은 시간| `TArray<FUI_BuffDisplayInfo>` | `struct` | `STAT_OnBuffApplied_E`, `STAT_OnBuffRemoved_E` |

*   **`FUI_StatDisplayInfo` 구조체 예시:**
    ```cpp
    USTRUCT(BlueprintType)
    struct FUI_StatDisplayInfo
    {
        GENERATED_BODY()

        UPROPERTY(BlueprintReadOnly) FText StatName;       // "공격력"
        UPROPERTY(BlueprintReadOnly) FText BaseValue;      // "100"
        UPROPERTY(BlueprintReadOnly) FText ModifierValue;  // "+25"
        UPROPERTY(BlueprintReadOnly) FSlateColor ModifierColor;  // 녹색 또는 적색
        UPROPERTY(BlueprintReadOnly) bool bCanBeUpgraded;    // 업그레이드 버튼 활성화 여부
        UPROPERTY(BlueprintReadOnly) FText UpgradeCost;      // 다음 레벨업 필요 자원 ("50 GP")
        UPROPERTY(BlueprintReadOnly) FText NextLevelValue;   // 업그레이드 시 변하게 될 값 ("100 -> 105")
    };
    ```

### **7.3 UI 업데이트 파이프라인**
1.  **상태 변경 발생:** `USTATAbilityComponent`에서 캐릭터의 체력이 변경됩니다.
2.  **이벤트 브로드캐스트:** 컴포넌트는 `STAT_OnStatChanged_E` 이벤트를 `FStatChangedPayload`와 함께 전파합니다. 이 페이로드에는 변경된 스탯 태그(`Stat.Core.HP.Current`), 이전 값, 현재 값 등의 상세 정보가 담겨 있습니다.
3.  **ViewModel 데이터 가공:** `USTAT_StatusViewModel`은 이 이벤트를 수신합니다. 그리고 페이로드 정보를 바탕으로 자신의 `HealthPercentage` 속성 값을 `(현재 체력 / 최대 체력)`으로 재계산하여 업데이트합니다.
4.  **UI 자동 갱신:** `WBP_HUD`의 체력 바(Progress Bar)는 `HealthPercentage` 속성에 바인딩되어 있으므로, 언리얼 UMG의 바인딩 시스템에 의해 **자동으로** 표시가 갱신됩니다.

### **7.4 스탯 변화 미리보기 (Preview) 기능**
플레이어의 선택을 돕기 위해, UI는 아이템 장착, 스킬 선택, 버프 적용 시 발생할 스탯 변화를 미리 계산하여 보여줄 수 있어야 합니다.

*   **미리보기 프로세스:**
    1.  **UI 이벤트 발생:** 플레이어가 인벤토리의 '화염 저항의 반지' 아이콘에 마우스를 올립니다.
    2.  **ViewModel에 미리보기 요청:** UI는 `ITEM_InventoryViewModel`에 `PreviewItemStats(ItemID)` 함수를 호출합니다.
    3.  **가상 계산 요청:** `ITEM_InventoryViewModel`은 `DA_Item_...` 데이터 에셋을 참조하여, 이 아이템이 `Stat.Resistance.Fire`에 `+30`의 Modifier를 준다는 것을 알아냅니다. 그리고 `USTAT_StatusViewModel`에 **가상(Virtual) 계산**을 요청합니다. `USTAT_StatusViewModel::CalculatePreview(ModifierList)`
    4.  **ViewModel에 미리보기 데이터 저장:** `USTAT_StatusViewModel`은 현재 스탯 값에 가상의 Modifier를 적용한 결과를 계산하여, `PreviewStatChanges`라는 별도의 UPROPERTY 변수에 저장합니다. 이 계산은 실제 캐릭터의 스탯에는 영향을 주지 않습니다.
    5.  **툴팁 UI 자동 갱신:** 아이템 툴팁 위젯(`WBP_ItemTooltip`)은 이 `PreviewStatChanges` 변수에 바인딩되어 있으며, "화염 저항: 50 -> 80" 과 같이 변경 전/후 값을 표시합니다.
    6.  **초기화:** 마우스가 아이콘에서 벗어나면, `PreviewStatChanges` 변수는 비워져 툴팁이 사라집니다.

## **8. 연동 시스템 심화 요구사항 (Advanced Interaction Requirements)**

이 섹션은 STAT 시스템이 직접 구현하지는 않지만, 다른 시스템(ITEM, SKIL, UI)이 특정 고급 기능을 구현할 수 있도록 **반드시 제공해야 하는 기반 기능과 데이터**를 정의합니다.

### **8.1 아이템 시스템(ITEM)을 위한 지원 기능**

#### **8.1.1 기능 목표: 드래그 앤 드롭 아이템 비교 및 교체 UX**
-   **사용자 시나리오:**
    1.  플레이어가 인벤토리(가방)에서 '화염의 장검'을 클릭(또는 드래그 시작)한다.
    2.  이때, 현재 '무기' 슬롯에 장착된 '얼음의 단검'의 툴팁과, 비어있는 다른 장비 슬롯(반지, 갑옷 등)이 모두 화면에 나타난다.
    3.  '화염의 장검' 툴팁에는 현재 스탯이 표시되고, '얼음의 단검' 툴팁에는 **'화염의 장검'으로 교체 시 변하게 될 스탯의 차이(+/-)**가 명확하게 표시된다. (예: 공격력 +20, 냉기 저항 -15)
    4.  플레이어가 '화염의 장검'을 '얼음의 단검' 툴팁 위로 드래그하여 드롭하면, 두 아이템이 자동으로 교체된다.

#### **8.1.2 STAT 시스템의 역할: 가상 스탯 세트 비교 (Virtual Stat-Set Comparison)**
위 기능을 구현하기 위해 `STAT` 시스템은 **실제 캐릭터의 스탯을 변경하지 않고** 두 개의 가상 아이템 세트를 비교하여 그 차이점을 계산해주는 기능을 인터페이스로 제공해야 합니다.

*   **필요 인터페이스 함수 (in `I_STAT_Comparable_If`):**
    ```cpp
    /**
     * 현재 장착된 아이템 세트와 비교 대상 아이템 세트 간의 스탯 변화를 계산하여 반환합니다.
     * @param ModifiersToCompare - 비교할 아이템(들)이 제공하는 스탯 Modifier 목록
     * @return 각 스탯 태그별 변화량(Delta)을 담은 TMap
     */
    UFUNCTION(BlueprintCallable)
    TMap<FGameplayTag, float> GetStatDeltaComparedTo(const TArray<FItemStatModifier>& ModifiersToCompare);
    ```

*   **동작 파이프라인:**
    1.  **UI 이벤트 발생:** 플레이어가 인벤토리의 아이템 A를 클릭.
    2.  **ITEM 시스템 정보 조회:** UI는 ITEM 시스템에 "아이템 A가 장착될 슬롯에 현재 장착된 아이템 B는 무엇인가?" 와 "아이템 A와 B의 스탯 Modifier 목록은 무엇인가?"를 질의.
    3.  **STAT 시스템에 비교 요청:** UI(또는 ViewModel)는 `GetStatDeltaComparedTo()` 함수를 호출. 이때 `ModifiersToCompare` 인자에는 **(아이템 A의 Modifier) - (아이템 B의 Modifier)**를 계산한 순수 변화량 목록을 전달.
    4.  **STAT 시스템 계산:** `USTATAbilityComponent`는 현재 캐릭터의 전체 스탯에 `ModifiersToCompare`를 가상으로 적용하여 최종 스탯 변화량을 계산하고, 그 결과를 `TMap`으로 반환. (예: `[Stat.Attack.Power, +20.0]`, `[Stat.Resistance.Cold, -15.0]`)
    5.  **UI에 표시:** UI는 이 `TMap` 데이터를 받아, 비교 대상(아이템 B) 툴팁에 "공격력 +20", "냉기 저항 -15" 와 같이 보기 쉽게 표시합니다.

## **9. 핵심 상호작용 모델: 공격/방어 태그 매칭 시스템**

모든 피해 및 상태 변화의 상호작용은 복잡한 우선순위 숫자나 시간 순서가 아닌, **논리적이고 직관적인 '태그 매칭' 시스템**을 통해 결정된다. 이는 디자이너가 의도한 상성 관계를 명확하게 구현하고, 향후 확장을 용이하게 하기 위함이다.

### **9.1 시스템 원칙**

> **"방어는 자신이 무시할 공격을 명시하고, 공격은 자신의 속성을 명시한다."**

*   **공격(Attack)의 역할:** 자신을 설명하는 태그들을 보유한다. (예: `Damage.Type.Fire`, `Damage.Attribute.Critical`, `Damage.Attribute.IgnoreArmor`)
*   **방어(Defense)의 역할:** 자신이 무력화시키거나 효과를 적용하지 않을 **'공격 태그'** 목록을 가진다. 이 목록에 없는 모든 공격은 정상적으로 처리된다.
*   **판정 주체:** `STAT` 시스템은 이 모든 판정을 담당하는 최종 심판 역할을 한다.

### **9.2 데이터 구조 정의**

#### **9.2.1 공격 효과 (in `DA_Skill` or `DA_Item`)**
모든 공격/디버프 효과는 자신을 설명하는 태그 컨테이너를 가집니다.

```cpp
// 스킬/아이템 데이터 에셋 내의 공격 효과 정의 예시
UPROPERTY(EditDefaultsOnly, Category="Attack Effect")
FGameplayTagContainer AttackTags;
```
*   **예시:** '방어력 무시 독 피해' 아이템의 `AttackTags` = { `Damage.Type.Poison`, `Damage.Attribute.IgnoreArmor` }

#### **9.2.2 방어 효과 (in `DA_Skill` or `DA_Item`)**
모든 방어/버프 효과는 자신이 무시할 **'공격 태그'** 목록을 명시하는 태그 컨테이너를 가집니다.

```cpp
// 스킬/아이템 데이터 에셋 내의 방어 효과 정의 예시
UPROPERTY(EditDefaultsOnly, Category="Defense Effect")
FGameplayTagContainer IgnoreAttackTags; // 이 목록에 있는 태그를 가진 공격은 이 방어 효과의 영향을 받지 않음
```
*   **예시 1:** '절대 보호막' 스킬의 `IgnoreAttackTags` = { `Damage.Attribute.IgnoreShield` }
    *   *해석: 이 보호막은 `Damage.Attribute.IgnoreShield` 태그를 가진 공격만 무시하고, 그 외 모든 공격에는 50% 피해 감소 효과를 적용한다.*
*   **예시 2:** '화염 면역' 버프의 `IgnoreAttackTags` = { } (비어 있음)
    *   *해석: 이 버프는 어떤 공격도 무시하지 않는다. 대신, 자신의 로직(화염 피해 100% 감소)은 `Damage.Type.Fire` 태그를 가진 공격에만 적용된다.*

### **9.3 상호작용 판정 파이프라인**

1.  **공격 발생:** 외부 시스템(SKIL, ITEM)이 `I_STAT_Damageable_If::ApplyDamage`를 호출. 이때 **피해량**과 함께 공격의 속성을 담은 **`AttackTags`** 를 전달.
2.  **적용 대상 확인:** `STAT` 시스템은 피해를 입을 캐릭터에게 현재 적용 중인 모든 방어/버프 효과 목록을 가져온다.
3.  **효과 필터링 (핵심 로직):** `STAT` 시스템은 각 방어 효과를 순회하며 다음을 검사한다.
    *   "이 방어 효과의 `IgnoreAttackTags` 목록에, 현재 들어온 공격의 `AttackTags` 중 하나라도 포함되는가?"
    *   **포함된다면 (Match!)**: 해당 방어 효과는 이번 계산에서 **무시된다.**
    *   **포함되지 않는다면**: 해당 방어 효과는 이번 계산에 **포함된다.**
4.  **최종 계산:** 필터링을 통과한 유효한 방어 효과들(예: 피해량 50% 감소 보호막, 방어력 스탯 등)을 모두 적용하여 최종 피해량을 계산한다.
5.  **결과 적용:** 계산된 최종 피해량을 캐릭터의 체력에 적용하고, 관련 이벤트를 전파한다.

### **9.4 시나리오 예시**

*   **상황:** 플레이어는 '일반 보호막'(50% 피해 감소, `IgnoreAttackTags`={`Damage.Attribute.IgnoreShield`})을 사용 중이다.
*   **시나리오 A: '화염창' 공격 (`AttackTags`={`Damage.Type.Fire`})**
    1.  '화염창'의 태그는 보호막의 `IgnoreAttackTags` 목록에 없다.
    2.  보호막 효과가 정상 적용된다.
    3.  **결과: 화염창 피해가 50% 감소하여 들어온다.**
*   **시나리오 B: '방패 파괴자' 공격 (`AttackTags`={`Damage.Type.Physical`, `Damage.Attribute.IgnoreShield`})**
    1.  '방패 파괴자'의 태그 중 `Damage.Attribute.IgnoreShield`가 보호막의 `IgnoreAttackTags` 목록과 일치한다.
    2.  보호막 효과가 이번 계산에서 **무시된다.**
    3.  **결과: 방패 파괴자 피해가 100% 그대로 들어온다.**

---

### **10. 핵심 UI 연동 확장: 지능형 스탯 표시 시스템 v2.0**

#### **10.1 기획 의도 (Why)**
(기존과 동일) "모든 숫자의 출처를 명확히 밝혀", 플레이어에게 최고의 가독성과 이해도를 제공하고, 디자이너에게는 밸런싱의 편의성을 제공하는 것을 목표로 한다.

#### **10.2 시스템 원칙 (What is it?)**
> **"정보를 분류하고, 필터링하여 원하는 것만 보여준다."**

캐릭터 정보창은 플레이어의 선택에 따라 두 가지 핵심 보기 모드를 제공한다.
1.  **상세 보기:** 특정 스탯을 클릭하면 해당 스탯의 모든 구성 요소를 계층적으로 보여준다.
2.  **필터 보기:** 상단의 필터 버튼(전체, 장비, 유물, 버프 등)을 통해, 전체 스탯 목록을 특정 출처가 제공하는 값만 표시하도록 필터링한다.

#### **10.3 구현 및 역할 분담 (How does it work?)**
1.  **[STAT 시스템] 출처 태그 부여:** (기존과 동일) 모든 스탯 모디파이어는 `Source.Item`, `Source.Relic` 등의 출처 태그를 가진다.

2.  **[ViewModel] 데이터 재가공 및 분류:** (기존과 동일) `USTAT_StatusViewModel`은 출처 태그를 기준으로 모든 스탯 데이터를 분류하여 `TMap<EStatSource, TArray<FUI_StatDisplayInfo>>` 와 같은 형태로 가공한다.

3.  **[UI View] 듀얼 모드 표시:**
    *   `WBP_CharacterStatus` 위젯은 상단에 **'전체', '장비', '유물', '스킬/버프'** 와 같은 필터 버튼 그룹을 가진다.
    *   **'전체' 필터가 활성화된 상태**에서는 각 스탯 항목을 클릭하여 상세 구성 내역을 펼쳐보는 '계층적 보기'가 가능하다.
    *   **'유물' 필터 버튼을 클릭**하면, 스탯 목록 전체가 오직 유물로부터 영향을 받는 스탯과 그 값만을 표시하도록 리프레시된다.

#### **10.4 플레이어 경험 예시 (Player's Experience)**

*   **초기 상태 (전체 필터):**
    *   공격력: 150
    *   방어력: 200
    *   체력: 1000

*   **'유물' 필터 버튼 클릭:**
    *   공격력: +15
    *   최대 체력: -200
    *   (유물이 다른 스탯에 영향을 주지 않았다면 해당 스탯은 목록에서 사라짐)

*   **'장비' 필터 버튼 클릭:**
    *   공격력: +35
    *   방어력: +200
    *   (장비가 체력에 영향을 주지 않았다면 해당 스탯은 목록에서 사라짐)

---

