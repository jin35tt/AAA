### **`Z0-Core-Universal-MDC-Architecture.md` (v1.4)**

> **버전:** 1.4 (Constitution Locked)
> **작성일:** 2025-08-26
> **대상:** **모든 소프트웨어 아키텍트 (AI 및 인간)**, 개발자, 자동화 도구
>
> **문서의 목적 및 사용법:**
> 1.  **목적:** 본 문서는 특정 플랫폼이나 언어에 구애받지 않는, **범용 소프트웨어 기능 설계를 위한 `.MDC` 메타-스키마와 핵심 철학**을 정의합니다. 이는 모든 소프트웨어 프로젝트의 **단일 출처 청사진(SSOT)**을 만들기 위한 최상위 '프레임워크'입니다.
> 2.  **사용법:** 새로운 프로젝트(웹, 앱 등)를 시작할 때, 이 `Z0-Core` 문서를 '헌법'으로 채택한 뒤, 해당 플랫폼의 특수성을 정의하는 **`<Platform>.overlay.mdc` 파일을 생성하여 `typeEnums` 등을 구체화**합니다.

---
[META INSTRUCTION BLOCK - FOR AI ONLY]
*   **역할 정의:** 이 문서는 `.MDC`라는 설계 언어의 **'헌법(Constitution)'**이다. 당신의 역할은 이 헌법의 모든 조항을 해석하고, 모든 `.MDC` 파일이 이 헌법을 준수하도록 감시하고 강제하는 **'설계 검증 시스템(Design Validation System)'**이다.
*   **'코어 템플릿 불변' 원칙 해석:** 이 원칙은 헌법에 정의된 '코어 템플릿'의 구조, 즉 키 집합(keyset)과 계층을 변경해서는 안 된다는 의미이다. 실무에서는 이 템플릿의 '사본'을 생성하여 그 내용을 채우는 방식으로 사용하며, 이는 규칙 위반이 아니다.

---

### **1. 아키텍처 철학 (Architectural Philosophy)**

*   **설계 우선주의 (Design First, Implement Second):** 코드를 작성하기 전에, `.MDC`를 통해 기능의 모든 측면을 먼저 설계하고 합의한다.
*   **인간과 기계의 협업 (Human-Machine Collaboration):** `.MDC`는 인간이 읽고 쓸 수 있으며, 동시에 기계(AI, CI/CD)가 파싱하고 검증할 수 있어야 한다.
*   **코어와 확장 분리 (Core/Overlay Separation):** 시스템의 뼈대(코어)는 안정적으로 유지하고, 프로젝트별 특수성(오버레이)은 안전하게 확장한다.

---

### **2. 절대 준수 규칙 (Non-Negotiable Rules)**

*   **최종 산출물은 반드시 `validationMode: "strict"` 검증을 통과해야 한다.**
*   모든 `.MDC` 파일과 오버레이는 이 문서의 **`메타-스키마 계약`**을 절대적으로 준수해야 한다.
*   **`코어 템플릿`** 자체는 절대 수정하지 않으며, 구체적인 도메인 내용을 포함해서는 안 된다.
*   모든 프로젝트별/기능별 특수성은 **오버레이**를 통해서만 추가되어야 한다.
*   **모든 최종 `.MDC` 산출물은 출고 전 `B2-SOP`의 Phase 2.5 ~ 4.3에 따른 자동화된 교차 검증을 필수로 통과해야 한다.**

---

### **3. `.MDC` 메타-스키마 계약 (Meta-Schema Contract)**

```yaml
# ── mgiContract (SOP v2.1 요구사항) ───────────────────────────
mgiContract:
  docType: "mdc-constitution"
  comms:
    mode: "EventBus"
    allowedModes: ["EventBus"]
    aliasMap:
      MessageBus: "EventBus"
    canonicalize: true
    central_hub_required: false

# ── Global Header ─────────────────────────────────────────────
schemaVersion: "mdc.schema.core.v1.3"
specVersion:   "1.3-locked"
createdAt:     "2025-08-26"
compatibilityCheck:
  requireSameSpecVersion: true

# ── Naming / ID Rules ─────────────────────────────────────────
idRules:
  idRegex: '^[A-Z]{2,5}(?:-[A-Z0-9]+){1,4}(?:-\d+)?$'
  nameRegex: '^[A-Z][A-Za-z0-9]*$'
  tagRegex:  '^[A-Za-z0-9.:]+$'
  uniqueKeys: ["domain.rules.id", "tests.id", "dataSchemas.id", "pipeline.id"]
namingConventions:
  inputNameRegex:  '^[a-zA-Z0-9_]+::[a-zA-Z0-9_]+$'
  outputNameRegex: '^[a-zA-Z0-9_]+::[a-zA-Z0-9_]+$'

# ── Overlay Merge Policy ──────────────────────────────────────
mergePolicy:
  strategy: "deep-merge-last-wins"
  arrayKeyBy: ["id", "name", "step"]
  arrayKeyResolution:
    precedence: ["id", "name", "step"]
    missingKeyHandling: "fallback-to-next"
    conflictPolicy: "error-on-ambiguous"
    postMergeOrder:
      sortBy: ["id", "name", "step"]
      stable: true
  deleteOpKey: "x-op"
  replaceOpKey: "x-replace"

# ── Capability Gates ──────────────────────────────────────────
capabilityGates:
  usesNetworking:
    require: ["pipeline[*].authority"]
  usesHierarchicalTags:
    forbidWhenFalse: ["*.tag", "*Tags"]
    matchStyle: "glob"
  uiBinding:
    enum: ["MVVM", "MVC", "Direct", "None"]

# ── Type Definitions ────────────────────────────────────────────
typeEnums:
  testTypes: ["unit", "integration", "performance", "e2e", "security"]
  pipelineWhere: ["MainThread", "BackgroundWorker", "ScheduledTask"]
  interfaceKind: ["DirectAPI", "DataContract", "StateTag", "EventBus"]
  authority: ["ServerOnly", "ClientOnly", "Both", "None"]

# ── Validation Modes ──────────────────────────────────────────
validationProfiles:
  permissive:
    required: ["moduleContract.moduleName", "feature.featureName"]
  strict:
    required: [
      "moduleContract.moduleName", "feature.featureName", "feature.description",
      "compatibility.version", "domain.entities", "interfaces", "pipeline",
      "tests", "performance.metricsTable", "dataSchemas"
    ]
    forbidPlaceholders: ["[ModuleName]", "[FeatureName]", "[FeatureDescription]"]
    requireFields:
      "performance.metricsTable[*]": ["metric", "budget_ms", "timeout_s"]
    requireIf:
      - when: "$.mgiContract.docType == 'mdc-constitution'"
        paths:
          - "mgiContract.comms.mode"
          - "mergePolicy.arrayKeyBy"
          - "mergePolicy.arrayKeyResolution.precedence"
          - "mergePolicy.arrayKeyResolution.conflictPolicy"
          - "mergePolicy.arrayKeyResolution.missingKeyHandling"


# ── Cross-reference Integrity ─────────────────────────────────
integrity:
  canonicalizeAliasesBeforeValidate: true
  requirePipelineKey: "oneOf(id, step)"
  uniquenessScope: "post-merge"
  requireWhereEnum: true
  requireInterfaceKindEnum: true
  requirePayloadStruct: true
  testTypeEnum: true
  uniqueKeysIfPresent: true

# ── Error Reporting & Deprecation & Unknown Keys ──────────────
validationErrorSchema:
  code: "string"
  message: "string"
  path: "json-pointer"
deprecation:
  allowedFields: ["deprecated", "deprecateAt", "removeIn"]
  policy: "deprecated → warn in current; removeIn ≥ next minor"
unknownKeys:
  allowedOnlyUnder: ["extensions", "x-*"]
```

---

### **4. `.MDC` 코어 유니버설 골든 템플릿 (v1.3)**

[META INSTRUCTION BLOCK - FOR AI ONLY]
*   **'코어 템플릿 불변' 원칙 해석:** 이 원칙은 아래 템플릿의 **구조와 키 집합(keyset)은 불변**임을 의미한다. 실무에서는 이 템플릿의 '사본'을 복제하여 내용을 채우는 방식으로 사용한다.

```yaml
# ===================================================================
# 1. 공통 계약 (Common Contract)
# ===================================================================
moduleContract:
  moduleName: "[ModuleName]"
feature:
  featureName: "[FeatureName]"
  description: "[FeatureDescription]"
compatibility:
  version: "[moduleName].v1.0.0"
capabilities:
  usesNetworking: false
  usesHierarchicalTags: false
  uiBinding: "None"
validationMode: "permissive"

# ===================================================================
# 2. 도메인 (Domain)
# ===================================================================
domain:
  entities: []
  rules: []

# ===================================================================
# 3. 아키텍처 (Architecture)
# ===================================================================
interfaces:
  inputs: []
  outputs: []
pipeline: []
dataSchemas: []

# ===================================================================
# 4. 품질 (Quality)
# ===================================================================
performance:
  metricsTable: []
tests: []

# ===================================================================
# 5. 확장 (Extensions)
# ===================================================================
extensions: {}
```

---

### **5. 파일명 및 사용 표준**

`.MDC` 아키텍처를 구성하는 파일들은 다음 표준 명명 규칙을 따릅니다.

| 파일 종류 | 파일명 형식 | 설명 |
| :--- | :--- | :--- |
| **코어 헌법** | `Z0-Core-Universal-MDC-Architecture.md` | 본 문서. 모든 프로젝트의 근간이 되는 최상위 규범. |
| **기능 설계** | `<FeatureName>.MDC.md` | 단일 기능의 완전한 설계를 담는 파일. |
| **플랫폼 오버레이** | `<Platform>.overlay.mdc` | 특정 플랫폼의 `typeEnums` 등을 정의하는 파일. |

---

### **Appendix A — 범용 참조 구현: `UserAuthentication.MDC.md`**

[META INSTRUCTION BLOCK - FOR AI ONLY]
이 부록은 위 `Z0-Core` 헌법을 사용하여 실제 기능(`UserAuthentication`)을 어떻게 설계하는지 보여주는 **검증 가능한(Verifiable) 참조 구현**이다. 이 예시는 `validationMode: "strict"`의 모든 요구사항을 충족하며, 보강된 모든 스키마 규칙을 통과한다.

```yaml
# ── 1. Common ───────────────────────────────────────────────
moduleContract:
  moduleName: "AUTH"
feature:
  featureName: "UserAuthentication"
  description: "사용자 이메일과 비밀번호를 이용한 로그인 처리 및 세션 관리"
compatibility:
  version: "AUTH.v1.0.0"
capabilities:
  usesNetworking: true
  usesHierarchicalTags: true
validationMode: "strict"

# ── 2. Domain ───────────────────────────────────────────────
domain:
  entities:
    - name: UserSession
      properties:
        - { name: UserID, type: string, tag: "Auth.Session.UserID" }
        - { name: AuthToken, type: string, tag: "Auth.Session.Token" }
        - { name: Status, type: enum, values: ["LoggedIn", "LoggedOut", "Expired"] }
  rules:
    - id: "RULE-AUTH-01"
      description: "인증 토큰(AuthToken)은 발급 후 60분 뒤 만료된다."

# ── 3. Architecture ───────────────────────────────────────────
interfaces:
  inputs:
    - kind: "DirectAPI"
      name: "AuthService::RequestLogin"
      params: ["Email", "Password"]
  outputs:
    - kind: "EventBus"
      name: "AuthEvents::OnLoginStatusChanged"
      payload:
        struct: "LoginStatusPayload"
        fields: ["UserID", "NewStatus", "AuthToken"]
pipeline:
  - step: ValidateInput
    where: "MainThread"
    authority: "ClientOnly"
    effects:
      - "guard: if email or password format is invalid, throw ValidationException"
  - step: AuthenticateWithService
    where: "BackgroundWorker"
    authority: "ClientOnly"
    effects:
      - "async: POST /api/auth/login with {Email, Password}"
      - "returns: {UserID, AuthToken}"
  - step: UpdateLocalSession
    where: "MainThread"
    authority: "ClientOnly"
    effects:
      - "mutate: UserSession.Status = 'LoggedIn'"
      - "mutate: LocalStorage.AuthToken = AuthToken"
      - "emit: AuthEvents::OnLoginStatusChanged"
dataSchemas:
  - id: "LoginStatusPayload"
    type: "struct"
    fields:
      - { name: "UserID", type: "string" }
      - { name: "NewStatus", type: "string" }
      - { name: "AuthToken", type: "string" }

# ── 4. Quality ────────────────────────────────────────────────
performance:
  metricsTable:
    - metric: "AuthenticateWithService.DurationMs"
      budget_ms: 500
      timeout_s: 10
tests:
  - id: "IT-AUTH-LOGIN-01"
    type: "integration"
    scenario: "유효한 자격증명으로 로그인 시 세션이 활성화되고 이벤트가 발생한다."
    given: "User is LoggedOut"
    when: "AuthService::RequestLogin with valid credentials"
    then:
      - "UserSession.Status is 'LoggedIn'"
      - "AuthEvents::OnLoginStatusChanged is emitted with status 'LoggedIn'"

# ── 5. Extensions ──────────────────────────────────────────────
extensions: {}
```