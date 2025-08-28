### **`[AI 지침: '[ProjectAAA] 캐릭터 상태 시스템-AI 운영체제 커널.md' 파일명으로 생성]`**

### **0. 기능 개발 실행 요약 (Feature Execution Summary)**

#### **0.1. 대상 애플리케이션 및 핵심 목표**

*   **프로젝트명:** `ProjectAAA`
*   **핵심 목표:** `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`를 기반으로, 태그 기반의 동적 스탯 및 상태 관리 허브인 'CharacterStatus' 기능을 개발하기 위한 마스터플랜을 수립하고 모든 실행 Task를 정의합니다.

#### **0.2. 개발의 핵심 방침 (Core Development Policy)**

*   **중앙 허브 요구사항:** 본 프로젝트의 모든 모듈 간 상호작용은 `.MDC`의 `mgiContract.comms.central_hub_required: true` 원칙에 따라, `BP_InteractionHub`를 통한 중계 방식을 절대적으로 준수해야 합니다.
*   **확장 키 일관성 규칙:** `authority` 등 `Z1`에서 제안된 비표준 확장 필드는, `.MDC` 파일의 코어 경로와 `extensions.x-z1` 경로에 동시에 미러링하는 것을 원칙으로 하여 Validator와 Planner 간의 해석 일관성을 보장합니다.

---

### **1. 문서 관계 및 생명주기 (Document Relationship & Lifecycle)**

#### **1.1. 입력 소스 문서 (Input Source Documents)**
> **[정의]** 본 개발 계획을 수립하기 위해 분석의 원재료로 사용된 문서 목록입니다.

| 문서 ID (파일 경로) | 문서명 | 핵심 역할 (무엇을 하는 문서인가?) |
| :--- | :--- | :--- |
| **최상위 개발 원칙 (Global Principles)** | `A0`, `A1`, `B0`, `B1`, `B2-SOP`, `Z0-Core`, `Z0-ProjectAAA`, `Z1`, `Z2` 등 | 프로젝트 전체에 적용될 범용 코딩, 오류 해결, 아키텍처 원칙. |
| **핵심 설계 청사진 (Core Design Blueprint)** | `M_ProjectAAA 캐릭터 상태 시스템_mdc.md` | **`TASK-00`의 '아키텍처 발자국'을 생성하는 단일 출처 청사진(SSOT).** |
| **시스템별 기획서 (GDD)** | `Mp01`, `Mp00`, `G01`, `G00_1`, `G00_2` | `.MDC`의 컨텍스트를 제공하는 요구사항 정의서. |

#### **1.2. 핵심 산출 문서 (Core Output Documents)**
> **[정의]** 본 플래닝 프로세스의 결과물로 생성되며, 실제 AI 개발 에이전트가 참조하여 작업을 수행하게 될 핵심 결과물 목록입니다.

| 산출물 분류 (Output Category) | 문서 ID (파일 경로) | 핵심 역할 (Core Role) |
| :--- | :--- | :--- |
| **마스터플랜 (Master Plan)** | `/ProjectAAA/Kernel/[ProjectAAA] 캐릭터 상태 시스템-AI 운영체제 커널.md` | 개발의 전체 Task 목록, 관계, 의존성을 정의하고 진행 상태를 추적하는 중앙 대시보드. |
| **개별 실행 Task (Actionable Tasks)** | `/ProjectAAA/Kernel/Tasks/[TASK-ID].md` | AI 개발 에이전트가 즉시 실행할 수 있는 독립적 작업 단위. |

---

### **2. Phase 1: `.MDC` 분석 및 Task 생성 파이프라인**
> **[정의]** 본 단계는 AI가 **핵심 설계 청사진(`.MDC`)**으로부터 애플리케이션의 전체 구조(`TASK-00`)와 모든 하위 구현 Task를 생성하는 자동화된 파이프라인입니다.
> 1.  **(문서 분류 및 규범 내재화):** AI는 `P1`을 제외한 모든 입력 파일을 역할에 따라 자동 분류하고, '최상위 원칙' 문서들의 핵심 규칙을 추출하여 커널의 `0.2. 개발의 핵심 방침` 섹션에 주입합니다.
> 2.  **(`.MDC` 유효성 검증):** 자동화된 'MDC Validator'가 `.MDC` 파일의 유효성을 검증합니다.
>     *   **기본 모드:** 검증 실패 시 프로세스는 **즉시 중단**되고 수정을 요청합니다.
>     *   **미리보기 모드 (`--preview`):** 검증 실패 시 경고를 출력하고, `TASK-00`을 읽기 전용(merge 불가) 상태로 시뮬레이션 생성하여 빠른 피드백 루프를 지원합니다.
> 3.  **(`TASK-00` 아키텍처 발자국 생성):** 'MDC-to-Architecture Parser'가 유효성이 검증된 `.MDC` 파일을 분석하여, `TASK-00`이 사용할 `architecturalFootprint` YAML을 자동으로 생성합니다.
> 4.  **(에픽 및 하위 Task 생성):** `.MDC`를 분석하여 에픽 Task와 구체적인 구현을 담당할 개별 하위 Task들을 생성합니다.
> 5.  **(논리 설계 생성 및 주입):** '플래너 AI'는 각 하위 구현 Task에 대해, `.MDC`를 해석하여 구체적인 **알고리즘 의사코드를 생성**하고, Task 파일의 **`4. 논리적 설계`** 섹션에 주입합니다.
> 6.  **(최종 Task 확정):** 모든 절차를 통과한 `TASK-00`과 모든 계층적 Task들이 `Defined` 상태가 되어 실행 큐에 등록됩니다.

#### **2.1. 분해된 Task 목록 (Decomposed Task List)**
> **[정의]** AI 파이프라인을 통해 최종 확정된 전체 작업 목록입니다. 본 테이블은 애플리케이션 개발 전 과정에 걸쳐 **실시간 상태 대시보드**로 기능합니다.
*   **상태(Status) 수명주기:** `Defined`, `In Progress`, `Completed`, `[HALTED_CRITICAL]`, `[HALTED_BUG]`

| Task ID | Task 요약 | 유형 | 의존 Task ID | 난이도 | 상태 |
| :--- | :--- | :--- | :--- | :--- | :--- |
| **`TASK-00`**| **[청사진] `.MDC` 기반 CharacterStatus 애플리케이션 스캐폴딩** | `A` | `N/A` | `8` | `Defined` |
| **`TASK-01`**| **[에픽] 피해 처리 (Attack/Defense Tag Matching) 파이프라인 구현** | `Meta`| `TASK-00` | `7` | `Defined` |
| ┣ `TASK-01.01` | `CollectDefenseRules` 구현 | `B` | `TASK-00` | `4` | `Defined` |
| ┣ `TASK-01.02` | `MatchAttackVsDefense` 구현 | `B` | `TASK-00` | `6` | `Defined` |
| ┣ `TASK-01.03` | `ComputeFinalDamage` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| ┣ `TASK-01.04` | `ApplyDamageToTarget` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| **`TASK-02`**| **[에픽] 스탯 업그레이드 파이프라인 구현** | `Meta`| `TASK-00` | `6` | `Defined` |
| ┣ `TASK-02.01` | `ValidateUpgradable` 구현 | `B` | `TASK-00` | `4` | `Defined` |
| ┣ `TASK-02.02` | `ConsumeResource` 구현 | `B` | `TASK-00` | `3` | `Defined` |
| ┣ `TASK-02.03` | `ApplyUpgradeModifier` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| **`TASK-03`**| **[에픽] 동적 세트 시너지 파이프라인 구현** | `Meta`| `TASK-00` | `9` | `Defined` |
| ┣ `TASK-03.01` | `EnqueueSynergyRecalc` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| ┣ `TASK-03.02` | `CalculateSynergyScore` 구현 | `B` | `TASK-00` | `8` | `Defined` |
| ┣ `TASK-03.03` | `ApplySynergyLevelTags` 구현 | `B` | `TASK-00` | `7` | `Defined` |
| **`TASK-04`**| **[에픽] 시그니처 차지 (Climax) 파이프라인 구현** | `Meta`| `TASK-00` | `7` | `Defined` |
| ┣ `TASK-04.01` | `AddSignatureCharge` (SigCharge_Accumulate) 구현 | `B` | `TASK-00` | `6` | `Defined` |
| ┣ `TASK-04.02` | `CheckSignatureReady` (SigCharge_CheckReady) 구현 | `B` | `TASK-00` | `4` | `Defined` |
| **`TASK-05`**| **[에픽] Flow (모멘텀) 파이프라인 구현** | `Meta`| `TASK-00` | `8` | `Defined` |
| ┣ `TASK-05.01` | `Flow_OnHitGain` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| ┣ `TASK-05.02` | `Flow_OnDamageTakenReset` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| ┣ `TASK-05.03` | `Flow_DecayTick` 구현 | `B` | `TASK-00` | `6` | `Defined` |
| ┣ `TASK-05.04` | `Flow_ApplyRewards` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| **`TASK-06`**| **[에픽] Overload (과부하) 파이프라인 구현** | `Meta`| `TASK-00` | `7` | `Defined` |
| ┣ `TASK-06.01` | `Overload_Start` 구현 | `B` | `TASK-00` | `6` | `Defined` |
| ┣ `TASK-06.02` | `Overload_Tick` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| ┣ `TASK-06.03` | `Overload_End` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| **`TASK-07`**| **[에픽] Fusion (스킬 융합) 파이프라인 구현** | `Meta`| `TASK-00` | `8` | `Defined` |
| ┣ `TASK-07.01` | `Fusion_NotifyEvent` 구현 | `B` | `TASK-00` | `6` | `Defined` |
| ┣ `TASK-07.02` | `Fusion_CheckAmp` 구현 | `B` | `TASK-00` | `5` | `Defined` |
| **`TASK-08`**| **[에픽] Counterplay (카운터) 파이프라인 구현** | `Meta`| `TASK-00` | `8` | `Defined` |
| ┣ `TASK-08.01` | `Counterplay_OpenWindow` 구현 | `B` | `TASK-00` | `6` | `Defined` |
| ┣ `TASK-08.02` | `Counterplay_Resolve` 구현 | `B` | `TASK-00` | `7` | `Defined` |

#### **2.2. 예외 처리 프로토콜 (Exception Handling Protocol)**
> **[정의]** Task 수행 중 결함 발견 시, 중앙 관리 AI가 실행하는 공식 대응 절차입니다.
*   **프로토콜 1: 계획 회로 차단기 (A-Type Task의 기본값)**
*   **프로토콜 2: 개별 Task 재시도 (B-Type Task의 기본값)**


*   **상태 전이 규칙 (State Transition Rules):**

| 현재 상태 | 목표 상태 | 전환 조건 및 필수 증빙 |
| :--- | :--- | :--- |
| `[HALTED_BUG]` | `In Progress` | **원인 분석 보고서** 및 **해결 방안(PR/커밋 링크 등)** 첨부 |
| `[HALTED_CRITICAL]`| `Defined` | **상위 계약(`.MDC` 등) 변경 내역(diff)** 및 **변경 승인자** 기록 |

---
