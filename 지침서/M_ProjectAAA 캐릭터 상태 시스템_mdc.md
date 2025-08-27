# STAT: Character Status System — .MDC (Markdown Feature Contract)

```yaml
# ===================================================================
# 0. MGI Meta-Contract (Z0/B2 compliance)
# ===================================================================
mgiContract:
  policyVersion: "2025-08-25"
  comms:
    central_hub_required: true  # BP_InteractionHub를 통한 중계 요구
  review:
    owners: ["AI System Architect", "Gameplay Programmer", "Technical Designer"]
    approvalsRequired: 2

# ── Z0 Core Header ─────────────────────────────────────────────
schemaVersion: "mdc.schema.v11.0"
specVersion:   "11.0-living-codex"
createdAt:     "2025-08-27"
validationMode: "strict"

# ===================================================================
# 1. 공통 계약 (Z0 기준)
# ===================================================================
pluginContract:
  pluginName: "STAT"           # 3~4자 약어 규칙
  dependencies: []              # C++ 직접 참조 금지
feature:
  featureName: "CharacterStatus"
  description: "태그 기반 동적 스탯/상태 허브. 모든 능력치·버프/디버프·피해/회복·업그레이드를 통합 관리"
compatibility:
  version: "STAT.v1.0.0"       # API 호환성 버전
capabilities:
  usesNetworking: true

# ===================================================================
# 2. 아키텍처 (Z0/Z2 기준)
# ===================================================================
interfaces:
  inputs:
    - kind: "UInterface"
      name: "ISTAT_Damageable_If::ApplyDamage"
      params: ["BaseDamage:float", "Instigator:AActor*", "AttackTags:FGameplayTagContainer", "CauseTag:FName"]
      notes: "공격/방어 태그 매칭 파이프라인의 진입점"
    - kind: "UInterface"
      name: "I_STAT_Upgradable_If::RequestUpgradeStat"
      params: ["StatTagToUpgrade:FGameplayTag"]
      notes: "DT_StatDefinition의 CanBeUpgraded 규칙 기반 업그레이드 요청"
    - kind: "UInterface"
      name: "I_STAT_SignatureSupport_If::AddSignatureCharge"
      params: ["TargetActor:AActor*", "CauseTag:FGameplayTag", "Amount:float=1.0"]
  outputs:
    - kind: "EventDispatcher"
      name: "STAT_OnStatChanged_E"
      payload: { struct: "FSTAT_ChangedPayload", fields: ["StatTag","PreviousValue","CurrentValue","Delta","MaxValue","Instigator","SourceTag","SourceObject"] }
    - kind: "EventDispatcher"
      name: "STAT_OnSignatureChargeChanged_E"
      payload: { struct: "FSTAT_SignaturePayload", fields: ["CurrentCharge","MaxCharge","bIsReady"] }
    - kind: "EventDispatcher"
      name: "STAT_OnSignatureChargeReady_E"
      payload: { struct: "FSTAT_ReadyPayload", fields: ["bIsReady"] }
    - kind: "EventDispatcher"
      name: "STAT_OnFlowStackChanged_E"
      payload: { struct: "FSTAT_FlowPayload", fields: ["CurrentStack","MaxStack","bIsFlowActive"] }
    - kind: "MessageBus"
      name: "STAT.Broadcast.Changed"
      payload: { struct: "FSTAT_ChangedPayload" }
  dataAssets:
    - kind: "DataAsset"
      name: "DA_STAT_AttributeSet"
    - kind: "DataAsset"
      name: "DA_STAT_DebuffProfile"

# ===================================================================
# 3. 데이터 스키마 (코어 + 확장)
# ===================================================================
dataSchemas:
  - id: "struct.FSTAT_Modifier"
    kind: "struct"
    fields:
      - { name: "TargetStatTag", type: "FGameplayTag", desc: "변경 대상 스탯" }
      - { name: "Operation",     type: "EModifierOp",  desc: "Add/Multiply 등" }
      - { name: "Value",         type: "float",        desc: "변경 값" }
      - { name: "SourceTag",     type: "FGameplayTag", desc: "출처 태그 (Source.Item 등)" }
      - { name: "SourceObject",  type: "TWeakObjectPtr<UObject>", desc: "출처 객체" }
  - id: "struct.FSTAT_ChangedPayload"
    kind: "struct"
    fields:
      - { name: "StatTag",       type: "FGameplayTag" }
      - { name: "PreviousValue", type: "float" }
      - { name: "CurrentValue",  type: "float" }
      - { name: "Delta",         type: "float" }
      - { name: "MaxValue",      type: "float" }
      - { name: "Instigator",    type: "TWeakObjectPtr<AActor>" }
      - { name: "SourceTag",     type: "FGameplayTag" }
      - { name: "SourceObject",  type: "TWeakObjectPtr<UObject>" }
  - id: "datatable.DT_StatDefinition"
    kind: "dataTable"
    key:  "StatTag:FGameplayTag"
    columns:
      - { name: "DisplayName",        type: "FText" }
      - { name: "CanBeUpgraded",      type: "bool" }
      - { name: "GrowthValueCurve",   type: "CurveFloat" }
      - { name: "RequiredPointCurve", type: "CurveFloat" }
  - id: "datatable.DT_SynergyGroupRules"
    kind: "dataTable"
    columns:
      - { name: "RuleID",                 type: "FName" }
      - { name: "GroupQuery",             type: "FGameplayTagQuery" }
      - { name: "Weight",                 type: "float" }
      - { name: "DiminishingReturnCurve", type: "CurveFloat" }
  - id: "datatable.DT_SynergyLevelThresholds"
    kind: "dataTable"
    columns:
      - { name: "LevelTag",  type: "FGameplayTag" }
      - { name: "MinScore",  type: "float" }

# ===================================================================
# 4. 파이프라인 (Z1 철학을 Z0 구조로 표현)
# ===================================================================
# 4.1 피해 처리 (Attack/Defense Tag Matching)
pipeline:
  - step: CollectDefenseRules
    where: "GameThread"              # 데이터 수집/참조
    authority: "ServerOnly"
    effects:
      - "Gather Defense.IgnoreAttackTags from active buffs/shields"
  - step: MatchAttackVsDefense
    where: "AsyncTask"
    authority: "ServerOnly"
    effects:
      - "Compute (AttackTags - Defense.IgnoreAttackTags)"
      - "Set bDefenseIgnored if any AttackTag is in ignore list"
  - step: ComputeFinalDamage
    where: "AsyncTask"
    authority: "ServerOnly"
    effects:
      - "Apply resistances via tags (Stat.Resistance.*)"
      - "Return FinalDamage"
  - step: ApplyDamageToTarget
    where: "GameThread"
    authority: "ServerOnly"
    outputs:
      - "STAT_OnStatChanged_E(HP)"

# 4.2 스탯 업그레이드 (DT_StatDefinition 기반)
  - step: ValidateUpgradable
    where: "GameThread"
    authority: "ServerOnly"
    effects:
      - "Check CanBeUpgraded == true"
      - "Check resource via RequiredPointCurve"
  - step: ConsumeResource
    where: "GameThread"
    authority: "ServerOnly"
    effects:
      - "Spend UpgradePoints"
  - step: ApplyUpgradeModifier
    where: "GameThread"
    authority: "ServerOnly"
    effects:
      - "Add persistent FSTAT_Modifier using GrowthValueCurve"
    outputs:
      - "STAT_OnStatChanged_E(StatTagToUpgrade)"

# 4.3 동적 세트 시너지 재계산 (프레임 분산)
  - step: EnqueueSynergyRecalc
    where: "AsyncTask"
    authority: "ServerOnly"
    effects:
      - "Collect all active FSTAT_Modifier"
  - step: CalculateSynergyScore
    where: "AsyncTask"
    authority: "ServerOnly"
    effects:
      - "For each rule in DT_SynergyGroupRules, compute contribution with DiminishingReturnCurve"
      - "Aggregate and map to thresholds in DT_SynergyLevelThresholds"
  - step: ApplySynergyLevelTags
    where: "GameThread_TimeSliced"
    authority: "ServerOnly"
    effects:
      - "Grant/Remove Synergy.Level.* tags time-sliced"
    outputs:
      - "STAT.Broadcast.Changed(FSTAT_ChangedPayload)"

# 4.4 시그니처 차지 관리
  - step: AddSignatureCharge
    where: "GameThread"
    authority: "ServerOnly"
    effects:
      - "Lookup DT_SignatureChargeConditions by CauseTag and add to SignatureChargeStack"
    outputs:
      - "STAT_OnSignatureChargeChanged_E"
  - step: CheckSignatureReady
    where: "GameThread"
    authority: "ServerOnly"
    effects:
      - "If SignatureChargeStack >= MaxSignatureCharge -> set Ability.CanUnleash.SignatureMove"
    outputs:
      - "STAT_OnSignatureChargeReady_E"

# ===================================================================
# 5. 확장 (Z1/Z2 네임스페이스)
# ===================================================================
extensions:
  x-z1:
    pipeline:
      ApplySynergyLevelTags:
        authority: "ServerOnly"   # 미러(읽기 전용)
  x-z2:
    exposes:
      - type: UInterface
        name: ISTAT_StaminaOwner_If
    components:
      use: [FGameTaskManager, FPriorityTaskQueue, FTimeSlicedTask, FBatchProcessor]
    pipeline:
      ApplyDamageToTarget:
        builder: { name: "STAT.ApplyDamage" }
        time_slice_ms: 1
      ApplySynergyLevelTags:
        builder: { name: "STAT.ApplySynergyTags", priority: Normal }
        batch:   { key: "ApplyTags", flush: frame_end }
        time_slice_ms: 1

# ===================================================================
# 6. 네트워킹 정책 (요약)
# ===================================================================
networking:
  prediction: { clientPrediction: false }
  authorityRules:
    - { step: "ApplyDamageToTarget", authority: "ServerOnly" }
    - { step: "ValidateUpgradable",  authority: "ServerOnly" }

# ===================================================================
# 7. 테스트 (정합성·회귀)
# ===================================================================
tests:
  - id: "STAT-T-001"
    name: "DamageTagMatch: IgnoreShield"
    given:  { AttackTags: ["Damage.Type.Physical", "Damage.Attribute.IgnoreShield"], Defense.IgnoreAttackTags: ["Damage.Attribute.IgnoreShield"] }
    expect: { bDefenseIgnored: true }
  - id: "STAT-T-002"
    name: "UpgradeStat: CanBeUpgraded true"
    given:  { StatTag: "Stat.Core.HP.Max", CanBeUpgraded: true, Points: 10 }
    expect: { Event: "STAT_OnStatChanged_E" }
  - id: "STAT-T-003"
    name: "SynergyScore: DiminishingReturn"
    given:  { SourceTags: ["Source.Item.Poison","Source.Skill.Poison","Source.Item.Poison"], Rule: "PoisonMastery" }
    expect: { LevelTagGranted: "Synergy.Level.1+" }

# ===================================================================
# 8. 가드레일 (A0/A1 절대 원칙 요약)
# ===================================================================
guardrails:
  - "다른 플러그인의 C++ 심볼 직접 참조 금지. 통신은 UInterface/EventDispatcher/MessageBus만 허용"
  - "UObject 변이는 GameThread 또는 GameThread_TimeSliced 단계에서만 수행"
  - "모든 장기 작업은 AsyncTask + Time-Slicing 적용, 각 StepFunction ≤ 1ms"

# ===================================================================
# 9. 용어/태그 정책 (요약)
# ===================================================================
conventions:
  gameplayTags:
    prefix: "Stat."
    examples: ["Stat.Core.HP.Max", "Stat.Resistance.Fire", "Synergy.Level.1"]
  naming:
    classPrefix: "STAT"
    dataAssetPrefix: "DA_STAT_"

```

