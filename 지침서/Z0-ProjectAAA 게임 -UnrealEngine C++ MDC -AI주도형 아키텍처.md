---

### **[최종 파이널 마스터 버전] `Z0-ProjectAAA 게임 -UnrealEngine C++ MDC -AI주도형 아키텍처.md`**

> **버전:** 11.0 (Living Codex)
> **작성일:** 2025-08-25
> **대상:** **AI 시스템 아키텍트 (주요 사용자)**, 인간 개발자, 자동화 도구
>
> **목적:**
> 본 문서는 **프로젝트와 장르에 독립적인, 범용 `.MDC` 코어 스키마와 그 자체의 유효성을 검증하는 메타-규칙(Meta-Rules)을 정의**한다. 이 문서는 AI 주도 개발의 모든 기능 설계를 위한 **단일 출처 청사진(SSOT)**으로 기능한다.

---

### **1. 목표 및 구조 설명**

#### **1.1. 최종 목표 (Goal)**
이 문서의 최종 목표는, AI와 인간이 협업하여 **예측 가능하고, 안정적이며, 확장 가능한** 게임 기능을 개발하기 위한 **'시스템 청사진' 언어인 `.MDC`를 정의**하는 것이다. `.MDC`는 기능의 **기획(What), 구현(How), 품질(Guarantee)** 전반을 다루는, 기계가 읽고 검증할 수 있는 단일 진실 공급원이다.

#### **1.2. 아키텍처 철학 (Architectural Philosophy)**
*   **설계 우선주의 (Design First, Implement Second):** 코드를 작성하기 전에, `.MDC`를 통해 기능의 모든 측면을 먼저 설계하고 합의한다.
*   **인간과 기계의 협업 (Human-Machine Collaboration):** `.MDC`는 인간(기획자, 개발자)이 읽고 쓸 수 있으며, 동시에 기계(AI, 자동화 도구)가 파싱하고 검증할 수 있어야 한다.
*   **코어와 확장 분리 (Core/Overlay Separation):** 시스템의 뼈대(코어)는 안정적으로 유지하고, 프로젝트별 특수성(오버레이)은 안전하게 확장한다. 이를 통해 범용성과 안정성을 모두 확보한다.

#### **1.3. 아키텍처의 구성 요소 (Components)**
| 컴포넌트 | 역할 | 설명 |
| :--- | :--- | :--- |
| **메타-스키마 계약** | **헌법 (The Constitution)** | `.MDC` 파일 자체의 구조, 문법, 유효성 검증 규칙을 정의한다. |
| **코어 템플릿** | **뼈대 (The Skeleton)** | 모든 `.MDC`가 따라야 할, 100% 중립적인 최소 구조를 제공한다. |
| **오버레이** | **살과 피 (The Flesh & Blood)** | 코어 템플릿 위에 특정 도메인(예: 스태미나)의 구체적인 내용을 덧씌운다. |

---

### **2. 절대 준수 규칙 (Non-Negotiable Rules)**
`[AI 지침: 아래 규칙은 `.MDC`를 작성하고 사용하는 모든 주체(AI, 인간)가 반드시 따라야 하는 최상위 규범이다.]`

1.  **메타-스키마 준수:** 모든 `.MDC` 파일과 오버레이는 이 문서의 **`메타-스키마 계약`**을 절대적으로 준수해야 한다.
2.  **코어 템플릿 중립성 유지:** **`코어 템플릿`** 자체는 절대 수정하지 않으며, 구체적인 도메인 내용을 포함해서는 안 된다.
3.  **오버레이를 통한 확장:** 모든 프로젝트별/기능별 특수성은 **오버레이**를 통해서만 추가되어야 한다.
4.  **Strict 모드 최종 검증:** 최종 산출물은 반드시 `validationMode: "strict"` 검증을 통과해야 한다.
5.  **정합성 보장:** 최종 병합된 `.MDC`는 내부의 모든 교차 참조(`trigger_rule` 등)가 완벽한 정합성을 이루어야 한다.

---

### **3. `.MDC` 메타-스키마 계약 (Meta-Schema Contract)**

`[AI 지침: 이 섹션은 모든 `.MDC` 파서와 자동화 도구가 준수해야 하는 '헌법'이다.]`

```yaml
# ── Global Header ─────────────────────────────────────────────
schemaVersion: "mdc.schema.v11.0"
specVersion:   "11.0-living-codex"
createdAt:     "2025-08-25"

# ── Naming / ID Rules ─────────────────────────────────────────
idRules:
  idRegex: '^[A-Z]{2,5}-[A-Z0-9]+(-\d+)?$'
  nameRegex: '^[A-Z][A-Za-z0-9]*$'
  tagRegex:  '^[A-Za-z0-9.]+$'
  uniqueKeys: ["domain.rules.id", "tests.id", "dataSchemas.id", "pipeline.id"]
namingConventions:
  inputNameRegex:  '^I_[A-Z]{2,5}_[A-Za-z0-9]+_If::[A-Z][A-Za-z0-9]*$'
  outputNameRegex: '^[A-Z]{2,5}_On[A-Z][A-Za-z0-9]*_E$'

# ── Overlay Merge Policy ──────────────────────────────────────
mergePolicy:
  strategy: "deep-merge-last-wins"
  arrayKeyBy: ["id", "name", "step"]
  deleteOpKey: "x-op"
  replaceOpKey: "x-replace"

# ── Core/Overlay Compatibility Check ────────────────────────────
compatibilityCheck:
  requireSameSpecVersion: true

# ── Capability Gates (검증 on/off) ────────────────────────────
capabilityGates:
  usesNetworking:
    require: ["pipeline[*].authority"]
  usesGameplayTags:
    forbidWhenFalse: ["*.tag", "*Tags"]
    matchStyle: "glob"
  uiBinding:
    enum: ["MVVM", "Direct", "None"]

# ── Type Definitions ────────────────────────────────────────────
typeEnums:
  testTypes: ["functional", "performance", "replication", "fuzz", "usability"]
  pipelineWhere: ["GameThread", "GameThread_TimeSliced", "AsyncTask", "RenderThread"]
  interfaceKind: ["UInterface", "DataAsset", "GameplayTag", "EventDispatcher", "MessageBus"]

# ── Validation Modes ──────────────────────────────────────────
validationProfiles:
  permissive:
    required: ["pluginContract.pluginName", "feature.featureName"]
  strict:
    required: [
      "pluginContract.pluginName", "feature.featureName", "feature.description",
      "compatibility.version", "domain.entities", "interfaces", "pipeline",
      "tests", "performance.metricsTable"
    ]
    forbidPlaceholders: ["[PluginPrefix]", "[FeatureName]", "[FeatureDescription]"]
    requireFields:
      performance.metricsTable[*]: ["metric", "budget_ms", "timeout_s"]

# ── Cross-reference Integrity ─────────────────────────────────
integrity:
  enforceNamePatterns: true
  requireRuleLink: true
  requirePayloadStruct: true
  testTypeEnum: true
  requirePipelineKey: "oneOf(id, step)"
  uniquenessScope: "post-merge"
  uniqueKeysIfPresent: true
  requireWhereEnum: true
  requireInterfaceKindEnum: true 

# ── Error Reporting Schema ──────────────────────────────────────
validationErrorSchema:
  code: "string"
  message: "string"
  path: "json-pointer"

# ── Deprecation Lifecycle ─────────────────────────────────────
deprecation:
  allowedFields: ["deprecated", "deprecateAt", "removeIn"]
  policy: "deprecated → warn in current; removeIn ≥ next minor"

# ── Unknown Keys Policy ───────────────────────────────────────
unknownKeys:
  allowedOnlyUnder: ["extensions", "x-*"]
```

---

### **4. `.MDC` 코어 유니버설 골든 템플릿 (v11.0)**

`[AI 지침: 아래 템플릿은 모든 `.MDC` 파일의 불변의 '코어 스키마'이다.]`

```yaml
# ===================================================================
# 1. 공통 계약 (Common Contract)
# ===================================================================
pluginContract:
  pluginName: "[PluginPrefix]"
feature:
  featureName: "[FeatureName]"
  description: "[FeatureDescription]"
compatibility:
  version: "[pluginName].v1.0.0"
  migration: []
capabilities:
  usesNetworking: false
  usesGameplayTags: false
  uiBinding: "None"
validationMode: "permissive"

# ... (rest of the core template is the same: empty arrays and sane defaults)
```

---

### **5. 용어 매핑 및 호환성 가이드 (과거 버전 호환성)**

`[AI 지침: 이 섹션은 구버전 `.MDC` 스키마나 관련 문서를 해석하기 위한 공식 가이드이다. 모든 자동화 도구는 하위 호환성 처리 시 이 매핑 테이블을 참조해야 한다.]`

| 과거 용어/키 (Old Term) | 현재 표준 키 (Current Standard) | 변경 사유 (Reason for Change) |
| :--- | :--- | :--- |
| **`io`** (섹션명) | **`interfaces`** | `Z0` 메타-스키마 표준화 및 역할 명확화 |
| `where: "ThreadPool"` | `where: "AsyncTask"` | `Z0` `typeEnums.pipelineWhere` 표준화 |
| `type: "perf"` | `type: "performance"` | `Z0` `typeEnums.testTypes` 표준화 |
| `pluginContract.version` | `compatibility.version` | 모듈 자체 버전과 API 호환성 버전의 역할을 명확히 분리 |
| `pluginContract.exposes` | `extensions.x-z2.exposes` | `Z0` 코어 스키마 순수성 원칙에 따라 비표준 키를 확장 슬롯으로 이동 |
| `builder`, `batch`, `time_slice_ms` | `extensions.x-z2.pipeline.*` | `Z0` 코어 스키마 순수성 원칙에 따라 세부 구현 키를 확장 슬롯으로 이동 |

---

### **Appendix A — Verifiable Example Overlay (StaminaSystem.overlay.mdc)**

`[AI 지침: 아래 내용은 코어 템플릿을 채우는 방법에 대한 **검증 가능한(Verifiable) 참조 구현**이다.]`

```yaml
# ── Header (for compatibility check) ──────────────────────────
schemaVersion: "mdc.schema.v11.0"
specVersion:   "11.0-living-codex"
createdAt:     "2025-08-25"

# ── 1. Common ───────────────────────────────────────────────
pluginContract:
  pluginName: "STAT"
feature:
  featureName: "StaminaSystem"
  description: "스태미나 소모/재생 도메인 예시"
compatibility:
  version: "STAT.v1.0.0"
capabilities:
  usesNetworking: true
  usesGameplayTags: true
  uiBinding: "MVVM"
validationMode: "strict"

# ── 2. Domain ───────────────────────────────────────────────
domain:
  entities:
    - name: StaminaAttribute
      properties:
        - { name: Current, type: float, unit: "pt", step: 0.01, tag: "Stat.Core.Stamina.Current" }
        - { name: Max,     type: float, unit: "pt", step: 0.01, tag: "Stat.Core.Stamina.Max" }
        - { name: RegenDelay, type: float, unit: "s", step: 0.01, tag: "Stat.Core.Stamina.RegenDelay" }
        - { name: ConsumeCooldown, type: float, unit: "s", step: 0.01 }
  rules:
    - id: "RULE-STAT-02"
      description: "소모 직후 RegenDelay 동안 재생 금지"

# ── 3. Architecture ───────────────────────────────────────────
interfaces:
  inputs:
    - name: "I_STAT_Attribute_If::RequestConsume"
      params: ["Amount","CauseTag"]
  outputs:
    - name: "STAT_OnStaminaChanged_E"
      payload:
        struct: "FSTAT_ChangedPayload"
        fields: ["StatTag","PreviousValue","CurrentValue","Delta","MaxValue","Instigator","SourceTag","SourceObject"]
pipeline:
  - step: ApplyBatchedConsume          # satisfies requirePipelineKey(oneOf id, step)
    where: GameThread_TimeSliced       # in typeEnums.pipelineWhere
    authority: "ServerOnly"            # required by usesNetworking gate
    effects:
      - mutate: "Current -= Sum(Amount)"
      - trigger_rule: "RULE-STAT-02"   # integrity.requireRuleLink
      - emit: "STAT_OnStaminaChanged_E"

# ── 4. Quality ────────────────────────────────────────────────
performance:
  metricsTable:
    - metric: "ApplyBatchedConsume.DurationMs"
      budget_ms: 1.5
      timeout_s: 3
      priority: "Normal"
tests:
  - id: "FT-STAT-REGENDELAY-01"
    type: "functional"                 # in typeEnums.testTypes
    scenario: "도메인 규칙(RegenDelay) 검증"
    given: "Current=100, RegenDelay=3"
    when: "RequestConsume(30)"
    then: "t=1s -> Current=70 (변화 없음)"

---
