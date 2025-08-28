### **`G02_ProjectAAA 게임_어빌리티_아이템 시스템 개발 지침서.md` (v2.1 - Master)**

# **Unreal Engine 5.6 어빌리티_아이템 시스템 개발 지침서 (v2.1)**

> **문서 상태:** **최종 승인 (Final & Complete Master)**
> **버전:** **v2.1**
> **최종 수정일:** 2025-08-11
>
> **변경 이력:**
> - **v2.1:** **최종 마스터 버전.** 상위 지침서(A0, A1, G00, Z1, Z2) 및 기획서(아이템 GDD, 전투재미)의 모든 요구사항을 **완전 반영**.
>   - 네이밍, 예외처리, 비동기, 메시징 규칙 및 구체적인 피드백(VFX/SFX) 예시 등 모든 누락 항목 보강.
> - v2.0: 에고 아이템, 유물, 태그 쿼리 등 핵심 기능의 기술 구현 명세 추가.
>
> **문서의 목적:**
> 본 문서는 ProjectAAA의 아이템(ITEM) 시스템 개발을 위한 **최상위 기술 규범이자 최종 설계도**입니다. AI 수석 아키텍트는 이 지침서에 명시된 모든 규칙을 **변경 없이 그대로 준수하여** 시스템을 구현해야 합니다.

---

## **1. 네이밍 및 코드 스타일 (Z2 준수)**

| 구분 | 규칙 | 예시 |
| :--- | :--- | :--- |
| **C++ Class/Struct/Enum**| `U`+`ITEM`+`PascalCase` | `USITEM_ItemComponent`, `FITEM_InstanceData` |
| **Interface** | `I`+`ITEM`+`_Descriptor_If` | `IITEM_InventoryOwner_If` |
| **Data Asset** | `DA_ITEM_`+`PascalCase` | `DA_ITEM_EgoSword`, `DA_ITEM_Potion_HP` |
| **Event Dispatcher** | `ITEM_`+`OnEventName_E` | `ITEM_OnInventoryChanged_E` |
| **Gameplay Tag** | `Item.`+`Category.Sub...` | `Item.Weapon.Sword`, `Item.Consumable.Potion` |
| **Blueprint 변수/함수** | `PascalCase` (동사로 시작) | `GetItemData`, `bIsEquipped` |

---

## **2. 모듈 간 통신 표준**

### **2.1 인터페이스 및 이벤트 중계 (G00 준수)**
*   `STAT` 시스템과의 상호작용은 반드시 `ISTAT_*_If` 인터페이스를 통해서만 이루어집니다.
*   `ITEM` 시스템 내부 이벤트는 `BP_InteractionHub`를 통해 중계하는 것을 원칙으로 합니다.

### **2.2 범용 메시징 (Z2 준수)**
*   **원칙:** `ITEM` 시스템과 직접적인 관련이 없는 범용 게임 월드 이벤트(예: '보스 처치', '던전 클리어')에 반응해야 할 경우, `EMessageBus`의 공통 채널을 구독하여 처리합니다. 이는 `ITEM` 시스템이 다른 시스템의 존재를 알 필요 없이 독립적으로 동작하게 합니다.
*   **예시:** '보스 처치' 메시지를 수신하면, '에고 아이템'이 추가 경험치를 얻는 로직을 구현할 수 있습니다.

### **2.3 CauseTag 변환 (G00 준수)**
*   `ITEM` 시스템이 `STAT` 시스템에 스탯 변경을 요청할 때 사용하는 `CauseTag`는 `FGameplayTag`를 사용합니다. 이 태그가 `STAT` 내부에서 `FName`으로 변환될 필요가 있다면, 그 책임은 `BP_InteractionHub`에 있습니다.

---

## **3. 핵심 아키텍처 및 데이터 구조**

### **3.1 아이템 데이터 에셋 (`DA_ITEM_Base`)**
*   **`DA_ITEM_Base` 최종 구조 (유물/저주 필드 상세화):**
    ```cpp
    UCLASS(Abstract, BlueprintType)
    class DA_ITEM_Base : public UPrimaryDataAsset
    {
        // ... (v2.0과 동일한 기본/에고 속성)

        // 유물 속성
        UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Relic", meta=(EditCondition="bIsRelic"))
        bool bIsRelic = false;

        UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Relic", meta=(EditCondition="bIsRelic", Tooltip="유물의 핵심 패시브 로직입니다. BP로 기능을 자유롭게 확장할 수 있습니다."))
        TSubclassOf<URelicEffectLogic> RelicEffectLogic;

        UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Relic", meta=(EditCondition="bIsRelic", Tooltip="이 유물에 저주(페널티)가 있는지 여부를 결정합니다."))
        bool bHasCurse = false;

        UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Relic", meta=(EditCondition="bHasCurse", Tooltip="저주가 적용될 스탯 태그와 상세 내용입니다."))
        FSTAT_Modifier CurseModifier; // 저주의 모든 정보(대상, 값, 연산, 소스 태그)를 담습니다.
    };
    ```

---

## **4. 성능 및 안정성**

### **4.1 비동기 처리 (Z1 준수)**
*   **규칙:** 대규모 인벤토리 정렬, 다수 아이템 일괄 처리 등 2ms를 초과할 가능성이 있는 모든 작업은 `FTimeSlicedTask`를 통해 분산 처리해야 합니다.
*   **우선순위/Timeout:** Task 등록 시 반드시 `ETaskPriority`를 명시하고, `Timeout`과 `OnTimeout` 콜백을 설정하여 무한 루프를 방지하고 실패 상황을 로깅해야 합니다.

### **4.2 예외 처리 및 안정성 (A0 준수)**
*   `check(StaticData != nullptr)`: 아이템 인스턴스가 유효한 `DA_ITEM_Base`를 갖는 것은 필수이므로 `check`를 사용합니다.
*   `ensure(PlayerController != nullptr)`: UI를 여는 등, 실패해도 크래시로 이어지지 않는 로직에는 `ensure`를 사용하여 개발 중 문제를 조기에 발견합니다.
*   모든 포인터 사용 전 `IsValid()` 체크를 생활화합니다.

### **4.3 블루프린트 Tooltip 의무화 (A1 준수)**
블루프린트에 노출되는 모든 UPROPERTY, UFUNCTION에는 `Tooltip` 메타 태그를 사용하여 **반드시 한글로 된 설명**을 추가해야 합니다.
```cpp
UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category="Base", meta=(Tooltip="아이템을 식별하는 고유 ID입니다. 중복될 수 없습니다."))
FName ItemID;```

---

## **5. 핵심 재미 요소 기술 구현 및 피드백 명세**

### **5.1 에고 아이템 성장 및 진화**
*   **피드백 강화:**
    *   **레벨업:** 아이템 레벨업 시, 채팅창에 "[{아이템 이름}]이 {레벨} 레벨이 되었습니다!" 메시지를 출력하고, 아이콘에서 빛나는 파티클(VFX)이 잠시 발생합니다.
    *   **진화:** 진화 성공 시, 화면 중앙에 아이템의 새로운 아이콘과 등급이 표시되는 전용 연출(UI 애니메이션, VFX, SFX)을 재생하여 플레이어의 성취감을 극대화합니다.

### **5.2 유물(Relic) & 저주(Curse) 시스템**
*   **UI 피드백 상세화:**
    *   캐릭터 정보창 UI는 '활성화된 유물 효과'와 '적용 중인 저주' 섹션을 명확히 분리하여 표시해야 합니다.
    *   `'활성화된 유물 효과'`: `RelicEffectLogic`의 설명 텍스트를 표시.
    *   `'적용 중인 저주'`: `CurseModifier`의 `TargetStatTag`와 `Value`를 기반으로 "최대 체력 -15%" 와 같이 명확한 페널티 내용을 표시.

### **5.3 전략적 카운터플레이 지원**
*   **역할:** `ITEM` 시스템은 `전략적 카운터플레이`에 필요한 **'수단'**을 제공합니다.
*   **구현:** 특정 아이템(장비, 소모품)은 장착/사용 시 플레이어의 다음 공격에 `Emit.Condition.Wet`과 같은 특수 태그를 부여하는 `UItemUseLogic`을 가질 수 있습니다.
*   **보상 로직:** 카운터 성공 시의 보상(자원 회복, 쿨다운 초기화, 아이템 드랍)은 `STAT`, `SKIL`, `ITEM` 시스템이 각자 담당하며, 이 모든 흐름은 `BP_InteractionHub`가 조율합니다. 예를 들어, `BP_InteractionHub`는 카운터 성공 이벤트를 받아 `ITEM` 시스템의 `DropSpecialRewardItem()` 함수를 호출할 수 있습니다.

### **5.4 과부하(Overload) 시스템 지원**
*   **역할:** `ITEM` 시스템은 `과부하` 상태를 직접 관리하지 않지만, 이를 강화하거나 약화시키는 옵션을 제공하여 빌드의 깊이를 더합니다.
*   **구현:** 아이템의 `EquipStatModifiers`에 `Item.Modifier.Overload.DurationBonus`, `Item.Modifier.Overload.CostReduction`과 같은 태그를 가진 옵션을 부여할 수 있습니다. `SKIL` 시스템은 스킬 제작/사용 시 이 태그들을 확인하여 최종 `FOverloadParameters`를 계산합니다.

---

## **6. 개발 및 테스트 체크리스트 (v2.1)**

*   [ ] **(완료)** 모든 식별자에 **네이밍 컨벤션** 완전 적용.
*   [ ] **(완료)** 상위 지침서의 **예외 처리, 비동기, 메시징, Tooltip** 규칙 모두 반영.
*   [ ] **(완료)** 에고 아이템 진화 시 **VFX/SFX 피드백** 연동 로직 구현.
*   [ ] **(완료)** 유물 장착 시 **UI에 효과/저주 분리 표시**를 위한 데이터 제공 로직 구현.
*   [ ] **(완료)** **전략적 카운터플레이**를 지원하는 아이템 효과 및 보상 드랍 연동 로직 구현.
*   [ ] **(완료)** **과부하 시스템**을 지원하는 아이템 옵션 태그 및 데이터 제공 로직 구현.
*   [ ] **(완료)** `ITEM_Test.umap` 테스트 맵에 상기 모든 기능에 대한 `FunctionalTest` 자동화 테스트 배치 및 통과 확인.