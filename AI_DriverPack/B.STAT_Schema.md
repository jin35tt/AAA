### **B. 가변 데이터 스키마: 캐릭터 상태(STAT) 시스템**

> **[AI 행동 지침]** 아래 **첫 번째 `yaml` 코드 블록**은 `캐릭터 상태(STAT) 시스템`의 상태를 정의하는 유일한 데이터 소스이다. 이 스키마를 파싱하여, `A.STAT_Contract.md`의 시그니처와 `C.STAT_Process_and_Gates.md`의 테스트 요구사항에 맞는 코드와 에셋을 생성하라.

#### **1. 스키마 정책 및 규칙 (Schema Policy & Rules)**
*   **권한의 범위 (Scope of Authority)**:
    *   본 스키마(B)는 `캐릭터 상태(STAT) 시스템`의 **내용(수치, 규칙, 목록)**에 한해 다른 모든 문서(기획서, 위키 등)보다 **최상위 권한**을 갖는다.
    *   단, 스키마의 형식이나 구조는 `A.STAT_Contract.md`에 정의된 규칙을 준수해야 한다.
*   **데이터 우선 원칙 (Data-First Principle)**:
    *   새로운 규칙이나 수치를 시스템에 반영할 때는 다른 문서나 코드가 아닌, **본 스키마에 가장 먼저 기록**하는 것을 원칙으로 한다.
*   **자동화된 검증 (Automated Validation)**:
    *   모든 `stat_tag`는 `Stat.*` 접두어 규칙을 반드시 따라야 하며, 이를 위반하는 데이터는 CI 과정에서 자동으로 거부된다.
    *   `stat_definitions`의 각 항목은 필수 키(`stat_tag`, `display_name`, `can_be_upgraded`)를 반드시 포함해야 하며, 누락 시 CI에서 실패 처리된다.
*   **생애주기 및 변경 정책 (Lifecycle & Change Policy)**:
    *   스키마 내용이 변경될 경우, 반드시 `schema_version` 메타데이터를 시맨틱 버전(SemVer) 규칙에 따라 갱신해야 한다.

#### **2. 데이터 스키마 정의 (Data Schema Definition)**
```yaml
schema_version: "1.0.0"

# ==========================================================
# 1. 스탯 정의 (EPIC-STAT-01)
# ==========================================================
# 게임에 존재하는 모든 스탯의 기본 정보와 업그레이드 가능 여부를 정의합니다.
stat_definitions:
  - stat_tag: "Stat.Core.HP.Max"
    display_name: "최대 체력"
    can_be_upgraded: true
    growth_value_curve: "Curve_Stat_HP_Growth"
    required_point_curve: "Curve_Stat_Default_Cost"
  - stat_tag: "Stat.Core.AttackPower"
    display_name: "공격력"
    can_be_upgraded: true
    growth_value_curve: "Curve_Stat_AP_Growth"
    required_point_curve: "Curve_Stat_Default_Cost"
  - stat_tag: "Stat.Resistance.Fire"
    display_name: "화염 저항력"
    can_be_upgraded: false
  # ... (기타 모든 스탯 정의)

# ==========================================================
# 2. 동적 세트 시너지 규칙 (EPIC-STAT-02)
# ==========================================================
# 태그 조합에 따른 시너지 점수 계산 규칙을 정의합니다.
synergy_group_rules:
  - rule_id: "PoisonMastery"
    group_query:
      match_any: ["Source.Skill.Poison", "Source.Item.Poison"]
    weight: 1.0
    diminishing_return_curve: "Curve_DR_Default"
  - rule_id: "BackstabSynergy"
    group_query:
      match_any: ["Exploit.Backstab.Window"]
    weight: 1.2
    diminishing_return_curve: "Curve_DR_Default"
  # ... (기타 모든 시너지 규칙)

# 시너지 점수에 따른 최종 레벨 및 보너스 정의
synergy_level_thresholds:
  - level: 1
    min_score: 2.0
    bonus_tag: "Synergy.Level.1"
  - level: 2
    min_score: 4.5
    bonus_tag: "Synergy.Level.2"
  # ... (기타 레벨 정의)

# ==========================================================
# 3. 핵심 데이터 구조 정의
# ==========================================================
# A.STAT_Contract.md에 명시된 핵심 구조체의 상세 필드입니다.
core_structs:
  FSTAT_Modifier:
    fields:
      - name: "TargetStatTag"
        type: "FGameplayTag"
        description: "변경할 스탯 (예: Stat.Core.AttackPower)"
      - name: "Operation"
        type: "EModifierOp"
        description: "연산 방식 (Add, Multiply 등)"
      - name: "Value"
        type: "float"
        description: "값"
      - name: "SourceTag"
        type: "FGameplayTag"
        description: "출처 태그 (예: Source.Item.Weapon, Source.Relic)"
      - name: "SourceObject"
        type: "TWeakObjectPtr<UObject>"
        description: "이 Modifier를 제공한 실제 오브젝트 (DA, Actor 등)"

  FSTAT_ChangedPayload:
    fields:
      - name: "StatTag"
        type: "FGameplayTag"
        description: "변경된 스탯 태그"
      - name: "PreviousValue"
        type: "float"
        description: "이전 값"
      - name: "CurrentValue"
        type: "float"
        description: "현재 값"
      - name: "Delta"
        type: "float"
        description: "변화량"
      - name: "MaxValue"
        type: "float"
        description: "최대 값"
      - name: "Instigator"
        type: "TWeakObjectPtr<AActor>"
        description: "원인 제공 액터"
      - name: "SourceTag"
        type: "FGameplayTag"
        description: "변경을 유발한 소스 태그"
      - name: "SourceObject"
        type: "TWeakObjectPtr<UObject>"
        description: "원인 오브젝트"
```