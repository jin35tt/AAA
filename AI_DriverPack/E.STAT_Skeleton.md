### **E. 프로젝트 골격 계약: 캐릭터 상태(STAT) 시스템**

#### **I. 프로젝트 골격 생성 4단계 절차 (The 4-Step Derivation Process)**
본 프로젝트 골격은 `A.STAT_Contract.md`, `B.STAT_Schema.md`, `C.STAT_Process_and_Gates.md` 문서로부터 아래 4단계의 논리적 추론 과정을 통해 체계적으로 생성됩니다.

1.  **1단계 (`A` → Core Structure):** `A` 문서의 `3. 핵심 시그니처`에서 `ISTAT_Damageable_If` 등 인터페이스, `USTATAbilityComponent` 등 핵심 클래스, `FSTAT_Modifier` 등 데이터 구조로부터 `/Source/STAT/Public` 및 `/Private` 폴더의 기본 구조를 도출합니다.
2.  **2단계 (`B` → Config & Tools):** `B` 문서의 `stat_definitions`와 `synergy_group_rules`로부터 `UDataAsset` 및 `UDataTable`을 관리할 `/Content/Data/STAT` 폴더 구조와, 스키마 유효성을 검사할 에디터 모듈 `/Source/STATEditor`의 필요성을 도출합니다.
3.  **3단계 (`C` → Tests):** `C` 문서의 `3. 품질 게이트 명세`에 정의된 `T_Test_STAT_*` 테스트 케이스 목록으로부터 `/Source/STATTests/Tests` 폴더 내의 각 테스트 파일(.cpp) 구조를 1:1로 도출합니다.
4.  **4단계 (종합 및 최종 트리):** 1, 2, 3단계의 결과를 종합하여 Unreal Engine Game Feature Plugin 표준에 맞춰 최종 골격을 완성합니다.

#### **II. 최종 프로젝트 골격 (Final Project Skeleton)**
```
Plugins/
└── STAT/
    ├── STAT.uplugin
    ├── Resources/
    │   └── Icon128.png
    ├── Content/
    │   └── Data/
    │       ├── DA_STAT_DefaultAttributeSet.uasset
    │       └── DT_STAT_SynergyRules.uasset
    └── Source/
        ├── STAT/
        │   ├── Private/
        │   │   └── USTATAbilityComponent.cpp
        │   ├── Public/
        │   │   ├── STAT.h
        │   │   ├── Components/
        │   │   │   └── USTATAbilityComponent.h
        │   │   ├── Data/
        │   │   │   └── FSTAT_Structures.h  // FSTAT_Modifier, FSTAT_ChangedPayload 등
        │   │   └── Interfaces/
        │   │       ├── ISTAT_Damageable_If.h
        │   │       ├── ISTAT_Buffable_If.h
        │   │       └── ISTAT_Comparable_If.h
        │   └── STAT.Build.cs
        ├── STATEditor/
        │   ├── Private/
        │   │   └── Validation/
        │   │       └── STATSchemaValidator.cpp
        │   ├── Public/
        │   │   └── STATEditor.h
        │   └── STATEditor.Build.cs
        └── STATTests/
            ├── Private/
            └── Tests/
                ├── T_Test_STAT_01_TagBasedStat.cpp
                ├── T_Test_STAT_01_StatUpgrade.cpp
                ├── T_Test_STAT_02_DynamicSynergy.cpp
                └── T_Test_STAT_03_MVVMBinding.cpp
```