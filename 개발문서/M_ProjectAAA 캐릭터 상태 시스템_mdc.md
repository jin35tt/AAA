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


# ===================================================================
# 10. Mp00 통합 추가 — Core Combat Fun Pillars (Flow · Synergy · Climax)
# ===================================================================
mp00Integration:
  sourceDoc: "Mp00_ProjectAAA 핵심 전투 재미 요소 기획서"
  version: "2025-08-27"
  compliance: { z0: true, z1: true, z2: true }
  pillars: ["Flow", "Synergy", "Climax", "Counterplay", "Overload", "Fusion"]

  # ---------------------------------------------------------------
  # 10.1 데이터 스키마(신규)
  # ---------------------------------------------------------------
  dataSchemas:
    - id: "struct.FFLOW_RewardSpec"
      kind: "struct"
      fields:
        - { name: "AffectedStatTag", type: "FGameplayTag", desc: "보상 적용 대상 스탯" }
        - { name: "Operation",       type: "EModifierOp",  desc: "Add / Multiply / Set" }
        - { name: "Value",           type: "float",        desc: "보상 수치" }
        - { name: "bTimeBound",      type: "bool",         desc: "시간 제한 보상 여부" }
        - { name: "Duration",        type: "float",        desc: "지속 시간(초), bTimeBound=true 일 때" }
        - { name: "SourceTag",       type: "FGameplayTag", desc: "보상 출처" }
    - id: "datatable.DT_FlowRules"
      kind: "dataTable"
      key:  "FlowID:FName"
      columns:
        - { name: "MaxStack",          type: "int32" }
        - { name: "OnHitGain",         type: "float" }
        - { name: "OnDamageTakenLoss", type: "int32" }
        - { name: "DecayPerSecond",    type: "float" }
        - { name: "TimeoutSeconds",    type: "float" }
        - { name: "Rewards",           type: "TArray<FFLOW_RewardSpec>" }
    - id: "datatable.DT_SignatureChargeConditions"
      kind: "dataTable"
      key:  "CauseTag:FGameplayTag"
      columns:
        - { name: "Amount",                     type: "float" }
        - { name: "TargetFilterTagQuery",       type: "FGameplayTagQuery" }
        - { name: "MultiplierByTargetTagQuery", type: "CurveFloat" }
    - id: "struct.FSTAT_OverloadParameters"
      kind: "struct"
      fields:
        - { name: "Duration",        type: "float" }
        - { name: "Costs",           type: "TArray<FSTAT_Modifier>", desc: "자원 소모/패널티" }
        - { name: "Buffs",           type: "TArray<FSTAT_Modifier>", desc: "과부하 중 보너스" }
        - { name: "Penalties",       type: "TArray<FSTAT_Modifier>", desc: "과부하 중 디버프" }
        - { name: "BurnoutDebuffTag",type: "FGameplayTag", desc: "종료 후 번아웃 태그" }
        - { name: "BurnoutDuration", type: "float" }
        - { name: "SourceTag",       type: "FGameplayTag" }
    - id: "struct.FSTAT_OverloadPayload"
      kind: "struct"
      fields:
        - { name: "bIsOverloading",  type: "bool" }
        - { name: "TimeRemaining",   type: "float" }
    - id: "datatable.DT_SkillFusionRules"
      kind: "dataTable"
      key:  "FusionID:FName"
      columns:
        - { name: "RequiredTags",    type: "FGameplayTagQuery" }
        - { name: "MaxStacks",       type: "int32" }
        - { name: "StackGainPerEvent", type: "float" }
        - { name: "AmpThresholds",   type: "TArray<float>" }
        - { name: "AmpTags",         type: "TArray<FGameplayTag>" }
    - id: "struct.FSTAT_FusionState"
      kind: "struct"
      fields:
        - { name: "CurrentStacks", type: "int32" }
        - { name: "AmpLevel",      type: "int32" }
        - { name: "ActiveAmpTag",  type: "FGameplayTag" }
    - id: "datatable.DT_CounterplayWindows"
      kind: "dataTable"
      key:  "WindowID:FName"
      columns:
        - { name: "TargetQuery",     type: "FGameplayTagQuery" }
        - { name: "TriggerTag",      type: "FGameplayTag" }
        - { name: "Duration",        type: "float" }
        - { name: "SuccessRewards",  type: "TArray<FSTAT_Modifier>" }
        - { name: "FailurePenalties",type: "TArray<FSTAT_Modifier>" }
    - id: "datatable.DT_SynergyLevelBonuses"
      kind: "dataTable"
      key:  "LevelTag:FGameplayTag"
      columns:
        - { name: "RewardModifiers", type: "TArray<FSTAT_Modifier>" }

  # ---------------------------------------------------------------
  # 10.2 인터페이스(신규/확장)
  # ---------------------------------------------------------------
  interfaces:
    inputs:
      - kind: "UInterface"
        name: "I_STAT_Flow_If::AddFlowStack"
        params: ["Amount:float=1.0", "SourceTag:FGameplayTag", "Instigator:AActor*" ]
        notes: "적중/행동 성공 등으로 모멘텀 스택 증가"
      - kind: "UInterface"
        name: "I_STAT_Flow_If::ResetFlow"
        params: ["ReasonTag:FGameplayTag"]
        notes: "피격/타임아웃 등으로 모멘텀 초기화"
      - kind: "UInterface"
        name: "I_STAT_Overloadable_If::RequestOverload"
        params: ["Params:const FSTAT_OverloadParameters&"]
        notes: "과부하 발동 요청(비용/버프/패널티/번아웃 포함)"
      - kind: "UInterface"
        name: "I_STAT_Fusion_If::NotifyFusionEvent"
        params: ["EventTag:FGameplayTag", "Source:AActor*"]
        notes: "융합 스택/증폭 트리거 이벤트 알림"
    outputs:
      - kind: "EventDispatcher"
        name: "STAT_OnOverloadStateChanged_E"
        payload: { struct: "FSTAT_OverloadPayload", fields: ["bIsOverloading","TimeRemaining"] }
      - kind: "EventDispatcher"
        name: "STAT_OnFusionAmpChanged_E"
        payload: { struct: "FSTAT_FusionState", fields: ["CurrentStacks","AmpLevel","ActiveAmpTag"] }
      - kind: "EventDispatcher"
        name: "STAT_OnCounterplayWindow_E"
        payload: { struct: "FSTAT_CounterplayWindowPayload", fields: ["WindowID","State","TargetActor","TimeRemaining"] }

  # ---------------------------------------------------------------
  # 10.3 파이프라인(신규 단계)
  # ---------------------------------------------------------------
  pipeline:
    # Flow (모멘텀)
    - step: Flow_OnHitGain
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Stack += DT_FlowRules.OnHitGain"
        - "Clamp to MaxStack"
      outputs: [ "STAT_OnFlowStackChanged_E" ]
    - step: Flow_OnDamageTakenReset
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Stack -= DT_FlowRules.OnDamageTakenLoss or reset to 0"
      outputs: [ "STAT_OnFlowStackChanged_E" ]
    - step: Flow_DecayTick
      where: "GameThread_TimeSliced"
      authority: "ServerOnly"
      effects:
        - "Every second: Stack -= DecayPerSecond; if TimeoutSeconds elapsed → Reset"
        - "Schedule Flow_ApplyRewards"
    - step: Flow_ApplyRewards
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "For each reward in DT_FlowRules.Rewards, apply as FSTAT_Modifier scaled by current Stack"
      outputs: [ "STAT_OnStatChanged_E" ]

    # Signature Charge (클라이맥스 준비)
    - step: SigCharge_Accumulate
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Lookup DT_SignatureChargeConditions by CauseTag"
        - "Signature.Charge.Stack += Amount*(MultiplierByTargetTagQuery)"
      outputs: [ "STAT_OnSignatureChargeChanged_E" ]
    - step: SigCharge_CheckReady
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "If Stack >= MaxSignatureCharge → grant Ability.CanUnleash.SignatureMove"
      outputs: [ "STAT_OnSignatureChargeReady_E" ]

    # Overload (과부하)
    - step: Overload_Start
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Apply Costs and Buffs"
        - "Set State.Overload.Active"
      outputs: [ "STAT_OnOverloadStateChanged_E" ]
    - step: Overload_Tick
      where: "GameThread_TimeSliced"
      authority: "ServerOnly"
      effects:
        - "Decrement TimeRemaining; maintain Buffs/Penalties"
      outputs: [ "STAT_OnOverloadStateChanged_E" ]
    - step: Overload_End
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Clear Overload buffs; apply BurnoutDebuffTag for BurnoutDuration"
        - "Unset State.Overload.Active"

    # Fusion (스킬 융합/증폭)
    - step: Fusion_NotifyEvent
      where: "AsyncTask"
      authority: "ServerOnly"
      effects:
        - "Evaluate DT_SkillFusionRules.RequiredTags"
        - "Fusion.Stack += StackGainPerEvent; clamp to MaxStacks"
    - step: Fusion_CheckAmp
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "If thresholds crossed → set AmpLevel and ActiveAmpTag"
      outputs: [ "STAT_OnFusionAmpChanged_E" ]

    # Counterplay (카운터 창)
    - step: Counterplay_OpenWindow
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "Spawn window by DT_CounterplayWindows(TriggerTag, TargetQuery)"
      outputs: [ "STAT_OnCounterplayWindow_E" ]
    - step: Counterplay_Resolve
      where: "GameThread"
      authority: "ServerOnly"
      effects:
        - "On success: apply SuccessRewards; on fail: apply FailurePenalties"
      outputs: [ "STAT_OnCounterplayWindow_E", "STAT.Broadcast.Changed" ]

  # ---------------------------------------------------------------
  # 10.4 네트워킹(요약)
  # ---------------------------------------------------------------
  networking:
    prediction: { clientPrediction: false }
    authorityRules:
      - { step: "Flow_*",       authority: "ServerOnly" }
      - { step: "SigCharge_*",  authority: "ServerOnly" }
      - { step: "Overload_*",   authority: "ServerOnly" }
      - { step: "Fusion_*",     authority: "ServerOnly" }
      - { step: "Counterplay_*",authority: "ServerOnly" }

  # ---------------------------------------------------------------
  # 10.5 테스트(신규)
  # ---------------------------------------------------------------
  tests:
    - id: "MP00-T-001"
      name: "Flow: Gain/Decay/Reset"
      given:  { StartStack: 0, OnHitGain: 1, DecayPerSecond: 0.5, TimeoutSeconds: 3 }
      expect: { StackTimeline: [1, 1.5, 1.0, 0.5, 0] }
    - id: "MP00-T-002"
      name: "Signature: Ready Gate"
      given:  { CauseTag: "Hit.Critical", Amount: 2, MaxSignatureCharge: 6 }
      expect: { Event: "STAT_OnSignatureChargeReady_E" }
    - id: "MP00-T-003"
      name: "Overload: Burnout Applied"
      given:  { Duration: 5, BurnoutDebuffTag: "State.Overload.Burnout", BurnoutDuration: 10 }
      expect: { TagsAfterEnd: ["State.Overload.Burnout"] }
    - id: "MP00-T-004"
      name: "Fusion: Threshold Amp"
      given:  { Thresholds: [2,4], Events: 5 }
      expect: { AmpLevel: 2 }
    - id: "MP00-T-005"
      name: "Counterplay: Success Reward"
      given:  { WindowID: "Boss_Parry", Trigger: "Boss.Attack.Heavy", Duration: 1.0 }
      expect: { RewardApplied: true }

  # ---------------------------------------------------------------
  # 10.6 태그/용어(추가)
  # ---------------------------------------------------------------
  conventions:
    gameplayTags:
      add:
        - "Flow.Stack"
        - "Flow.Reward.AttackSpeed"
        - "Ability.CanUnleash.SignatureMove"
        - "State.Overload.Active"
        - "State.Overload.Burnout"
        - "Fusion.Amp.1"
        - "Fusion.Amp.2"
        - "Fusion.Amp.3"
        - "Counterplay.Window.Open"
        - "Counterplay.Success"
        - "Counterplay.Fail"
        - "Source.Relic"

  # ---------------------------------------------------------------
  # 10.7 가드레일(추가)
  # ---------------------------------------------------------------
  guardrails:
    - "Flow_DecayTick, Overload_Tick 등 모든 주기 작업은 Time-Sliced(≤1ms)로 유지"
    - "모든 보상/패널티는 FSTAT_Modifier로 표준화해 적용 및 롤백이 가능해야 함"
    - "C++ 심볼 직접참조 금지, 모든 연동은 UInterface/EventDispatcher/MessageBus로 한정"

```

