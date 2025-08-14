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

## 7) 수용 기준

* 플레이 중 상태창/인벤토리/스킬바가 일관성 있게 즉시 갱신
* 플러그인 온/오프 시 의존성 누락 없이 안정 동작
* 새 DataAsset 추가 시 충돌 없이 즉시 반영 가능

---

## 8) 체크리스트

* [ ] Dispatcher 페이로드 구조체 3종 도입 및 바인딩
* [ ] Hub 인터페이스 라우팅 연결
* [ ] CauseTag 어댑터 적용
* [ ] 각 플러그인 테스트 맵에 FunctionalTest 배치
* [ ] CI에 Validation/Tag/IWYU 검사 통합

```

---

