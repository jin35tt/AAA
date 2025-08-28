# 🎯 Unreal Engine 5.6 어빌리티 최상위 아키텍처: Master v1.0

## 1) 핵심 원칙
- **완전 격리:** 각 시스템은 Game Feature Plugin으로 구현, C++ 직접 참조 금지.
- **블루프린트 중심 조립:** 모든 상호작용은 BP 레이어(Interaction Hub)에서 이벤트/인터페이스 라우팅.
- **데이터 중심 + 검증:** Primary DataAsset, GameplayTag 검증, DataValidation+CI.

---

## 2) 상위 구성도 (MVVM + Dispatcher + Registry)
```

\[STAT Plugin]                         \[ITEM Plugin]                          \[SKIL Plugin]
USTATAbilityComponent                 USITEM\_ItemComponent                   USKIL\_SkillComponent
├─ STAT\_On\* Dispatchers  \~>           ├─ ITEM\_On\* Dispatchers  \~>            ├─ SKIL\_On\* Dispatchers  \~>
├─ DA\_STAT\_AttributeSet              ├─ USITEM\_ItemRegistry (Primary)       ├─ USKIL\_SkillRegistry (Primary)
└─ ViewModel: StatusVM               └─ ViewModel: InventoryVM              └─ ViewModel: SkillBarVM

```
                             ┌─────────────────────────────────────────┐
                             │    BP_InteractionHub (최상위 조립)      │
                             │  - 모든 Dispatcher 바인딩/해제           │
                             │  - ViewModel ↔ 위젯 연결                 │
                             │  - 인터페이스 라우팅(I_STAT_* 등)        │
                             └─────────────────────────────────────────┘
```

```

---

## 3) 모듈 간 “계약” (Interfaces & Events)

### 인터페이스 표준
- `ISTAT_Damageable_If`
- `ISTAT_Resource_If`
- `ISTAT_Buffable_If`

### 이벤트 페이로드 표준
- **STAT**:  
  `FStatChangedPayload { Prev, Cur, Delta, Max, Instigator, CauseTag(FName) }`
- **ITEM**:  
  `FItemUsedPayload { ItemID, CountDelta, Instigator, CauseTag }`
- **SKIL**:  
  `FSkillUsedPayload { SkillID, Cooldown, Cost, Instigator, CauseTag }`

---

## 4) CauseTag 규칙
- STAT은 `FName CauseTag` 유지
- Hub에서 `FGameplayTag → FName` 변환 처리

---

## 5) 디자이너 워크플로우
- **DataAsset 단위 작업**: `DA_Item_*`, `DA_Skill_*`는 개별 에셋로 충돌 최소화
- **ViewModel 노출 필드**: HP%, 쿨다운, 스택 등 위젯에 직접 바인딩 가능한 값 제공
- **테스트 맵 & 자동화**: 각 플러그인 `*_Test.umap` + FunctionalTest 필수

---

## 6) 폴더/빌드 규칙
```

Plugins/
STAT/ (Runtime, Editor)
ITEM/
SKIL/
Shared/ (Optional)

```
- `.uplugin` 버전/세맨틱 버전
- Precompile/IWYU
- 런타임 온·오프 시나리오 포함

---

# 🛠 템플릿 PR (디자이너 × AI 협업 에디션)

## 1) PR 제목
```

\[MASTER] STAT/ITEM/SKIL UI-MVVM & InteractionHub 통합 v1.0

````

## 2) 변경 요약
- Dispatcher→Payload→ViewModel→Widget 파이프라인 확립
- Interaction Hub에서 모든 연결 집중 관리
- DataAsset/Tag/Validation/CI 규칙 공용화

---

## 3) 포함 항목
- `BP_InteractionHub` (샘플 바인딩 상태)
- `WBP_StatusPanel`, `WBP_Inventory`, `WBP_SkillBar`
- 샘플 DataAsset: `DA_Item_Potion_HP`, `DA_Skill_Fireball`
- 각 플러그인 테스트 맵: `STAT_Test.umap`, `ITEM_Test.umap`, `SKIL_Test.umap`

---

## 4) 인터페이스 & 이벤트 시그니처
```cpp
// STAT
USTRUCT(BlueprintType)
struct FStatChangedPayload {
    float PreviousValue;
    float CurrentValue;
    float Delta;
    float MaxValue;
    TWeakObjectPtr<AActor> Instigator;
    FName CauseTag;
};

// ITEM
USTRUCT(BlueprintType)
struct FItemUsedPayload {
    FName ItemID;
    int32 CountDelta;
    TWeakObjectPtr<AActor> Instigator;
    FName CauseTag;
};

// SKIL
USTRUCT(BlueprintType)
struct FSkillUsedPayload {
    FName SkillID;
    float Cooldown;
    float ManaCost;
    TWeakObjectPtr<AActor> Instigator;
    FName CauseTag;
};
````

---

## 5) Interaction Hub 규칙

* 모든 바인딩/라우팅은 Hub 단일 BP 담당
* CauseTag 어댑터: `FGameplayTag` → `FName` 변환

---

## 6) CI/검증

* **Pre-Commit**: IWYU, 순환참조, 네이밍, Tag 오타 검사
* **CI Build**: `-RunDataValidation`, 인터페이스 시그니처 체크
* **FunctionalTest**: 각 `*_Test.umap` 필수 통과



---

## 7) 핵심 전투 재미 요소 교차 검증 표
*   **내용:**
    > 이 표는 `DOC-GAMEPLAY-FUNFACTORS`에 정의된 핵심 전투 재미 요소들이 각 시스템 지침서 (`G01`, `G02`, `G03`)에서 어떻게 구체적으로 기술되고, 어떤 자동화 테스트를 통해 검증되는지 명확하게 연결하여 보여줍니다. 이는 `G00_1 (7. 수용 기준)`, `G00_1 (8. 체크리스트)`, `Z2 (4. 보안/검증 파이프라인)` 원칙을 준수합니다.

    | 전투 재미 요소 (Fun Factors) | 근거 문서 (섹션)                    | 주관 시스템 (G0x 문서) | 구현 지점 (클래스/함수/데이터 테이블 예시)              | 자동화 테스트 (맵 & FunctionalTest 예시)   |
    | :-------------------------- | :---------------------------------- | :------------------- | :------------------------------------------ | :----------------------------------------- |
    | Flow (모멘텀) 버프            | `DOC-GAMEPLAY-FUNFACTORS (3.1)`     | `G01`, `G03`         | `ISTAT_FlowSupport_If::AddFlowStack()`, `DT_FlowBuffRewards`, `SKIL_OnSkillHitEnemy_E` | `STAT_Test.umap` (`FT_FlowBuffStacking`), `SKIL_Test.umap` (`FT_SkillHitFlowTrigger`) |
    | 상호작용 태그 미니 시너지     | `DOC-GAMEPLAY-FUNFACTORS (3.2)`     | `G01`, `G03`         | `FGameplayTagContainer AttackTags`, `FGameplayTagContainer IgnoreAttackTags`, `DT_SkillFusionRules` | `STAT_Test.umap` (`FT_TagInteractionMatch`), `SKIL_Test.umap` (`FT_FusionShatterLogic`) |
    | 시그니처 무브                | `DOC-GAMEPLAY-FUNFACTORS (3.3)`     | `G01`, `G03`         | `I_STAT_SignatureSupport_If::AddSignatureCharge()`, `DT_SignatureChargeConditions`, `ActivateSignatureMove()` | `STAT_Test.umap` (`FT_SignatureChargeUnlock`), `SKIL_Test.umap` (`FT_SignatureMoveExecution`) |
    | 동적 세트 시너지             | `DOC-GAMEPLAY-FUNFACTORS (3.4)`     | `G01`                | `USTATAbilityComponent::CalculateSynergyScore()`, `DT_SynergyGroupRules` | `STAT_Test.umap` (`FT_DynamicSynergyCalculation`) |
    | 전략적 카운터플레이          | `DOC-GAMEPLAY-FUNFACTORS (3.5)`     | `G01`, `G03`, `G02`  | `ISTAT_Damageable_If::ApplyDamage()`, `USKIL_SkillComponent::ResetCooldown()`, `UITEM_ItemComponent::DropSpecialRewardItem()` | `STAT_Test.umap` (`FT_BossCounterSuccess`) |
    | 과부하 (Overload) 시스템     | `DOC-GAMEPLAY-FUNFACTORS (3.6)`     | `G03`, `G01`         | `FSKIL_OverloadParameters`, `I_STAT_Overloadable_If::RequestOverload()` | `SKIL_Test.umap` (`FT_OverloadActivation`), `STAT_Test.umap` (`FT_OverloadEffectApplication`) |
    | 유물 (Relic) & 저주 (Curse)  | `DOC-GAMEPLAY-FUNFACTORS (3.7)`     | `G02`, `G01`         | `DA_ITEM_Base::bIsRelic`, `FSTAT_Modifier CurseModifier`, `URelicEffectLogic` | `ITEM_Test.umap` (`FT_RelicCurseApplication`), `ITEM_Test.umap` (`FT_RelicPassiveActivation`) |
    | 스킬 융합 (Fusion) 시스템    | `DOC-GAMEPLAY-FUNFACTORS (3.8)`     | `G03`, `G01`         | `DT_SkillFusionRules`, `USKIL_SkillComponent::TryFusion()`, `Fusion Volume Manager` | `SKIL_Test.umap` (`FT_SkillFusionNormal`), `SKIL_Test.umap` (`FT_FusionVolumeManagement`) |
    | 융합 증폭 시스템             | `DOC-GAMEPLAY-FUNFACTORS (3.8.9)`   | `G03`, `G01`         | `STAT_OnFusionChargeChanged_E`, `Fusion.State.Amplified` | `SKIL_Test.umap` (`FT_FusionAmplified`) |
    | UI 스탯 미리보기             | `ProjectAAA 캐릭터 상태 시스템 _01메인 기획서 (7.4)` | `G01`, `G02`         | `I_STAT_Comparable_If::GetStatDeltaComparedTo()`, `UITEM_InventoryViewModel::PreviewItemStats()` | `STAT_Test.umap` (`FT_StatPreviewCalculation`), `ITEM_Test.umap` (`FT_ItemStatComparisonUI`) |
    | 지능형 스탯 표시 시스템 v2.0 | `ProjectAAA 캐릭터 상태 시스템 _01메인 기획서 (10)` | `G01`                | `FSTAT_Modifier::SourceTag`, `USTAT_StatusViewModel::FilterStatsBySource()` | `STAT_Test.umap` (`FT_FilteredStatDisplay`) |

---

## 8) 수용 기준

* 플레이 중 상태창/인벤토리/스킬바가 일관성 있게 즉시 갱신
* 플러그인 온/오프 시 의존성 누락 없이 안정 동작
* 새 DataAsset 추가 시 충돌 없이 즉시 반영 가능
---

## 9) 체크리스트

* [ ] Dispatcher 페이로드 구조체 3종 도입 및 바인딩
* [ ] Hub 인터페이스 라우팅 연결
* [ ] CauseTag 어댑터 적용
* [ ] 각 플러그인 테스트 맵에 FunctionalTest 배치
* [ ] CI에 Validation/Tag/IWYU 검사 통합

---

