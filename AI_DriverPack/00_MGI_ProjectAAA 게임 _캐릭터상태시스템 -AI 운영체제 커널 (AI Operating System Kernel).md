### **`AI 운영체제 커널 (AI Operating System Kernel).md` (Final Master)**

### **0. AI 실행 환경 및 구성 (AI Execution Environment & Configuration)**

#### **0.0. AI주도 협업의 핵심 방침 (AI-Led Collaboration Core Policy)**
**본 프로젝트는 아래의 핵심 방침을 최상위 규범으로 삼으며, 모든 개발 활동은 이 원칙을 절대적으로 준수해야 합니다.**

1.  **기획 단계 (인간 주도, AI 보조):** 게임의 재미와 비전에 대한 아이디어는 인간 '게임 감독관'이 주도합니다. AI는 아이디어 구체화와 최종 기획서(GDD) 및 `B. 가변 데이터 스키마` 작성을 보조합니다. **이 단계의 최종 산출물은 '완결되고 승인된 기획 및 데이터 스키마'입니다.**

2.  **개발 단계 (AI 주도, 인간 개입 최소화):**
    *   **계획 수립:** 기획 단계가 완료되고 `B` 스키마가 업데이트되면, **AI '수석 아키텍트'는 `F. 작업 지시서`의 `스키마 동기화` Job Order를 실행하여 `B` 스키마의 변경 사항을 자동으로 감지하고, 이를 구현하기 위한 모든 기술 작업을 스스로 계획하고 실행합니다.**
    *   **구현:** AI는 자신이 수립한 계획에 따라 모든 프로그래밍, 데이터 테이블 생성, 단위 테스트 및 시각 자료(D) 업데이트를 자율적으로 수행합니다. 인간은 "AI가 더 잘하기 때문에" 이 과정에 직접 개입하여 혼선을 유발해서는 안 됩니다.
    *   **고난도 작업 처리:** 단, 시스템의 근간에 영향을 미치는 고난도(H-등급) 변경의 경우, AI는 코드 생성 전 작업 분해 계획(`subtasks`)을 먼저 제안하며, 인간 아키텍트의 검토 및 승인을 받은 후에만 실행합니다.

3.  **검증 및 피드백 단계 (인간 주도, AI 수정):**
    *   **빌드 및 테스트:** AI가 **빌드에 필요한 모든 산출물(코드, 데이터, 설정)의 생성을 완료하여 전달하면, 인간은 이를 사용하여 물리적인 빌드를 실행합니다.** 빌드가 성공하면, 인간은 '게임 감독관'으로서 시스템을 테스트하고 기획 의도와의 부합 여, 개선점 등 **경험에 기반한 피드백**을 제공합니다.
    *   **반복 개선:** AI는 인간의 피드백(또는 빌드 실패 로그)을 새로운 요구사항으로 간주하여 `B` 스키마 업데이트를 제안하거나, 즉시 수정 계획을 수립하여 시스템을 보완해 나갑니다.

4.  **문맥 주입 및 검증 원칙 (In-Context Injection & Verification Principle):**
    *   **(적용 범위)** 본 원칙은 `P0`, `p2` 템플릿, 본 `AI 운영체제 커널` 문서를 포함한 **프레임워크 전체에 적용되는 최상위 원칙**입니다.
    *   **(주입)** AI의 작업 누락 및 오해를 방지하기 위해, 모든 AI 작업 지시(특히 `C` 문서의 시스템 프롬프트와 `F` 문서의 Job Order)에는 해당 작업 수행 시 **반드시 준수해야 할 상위 원칙 문서(예: `A0`, `A1`, `B0`, `B1`, `Z1`)가 명시적으로 함께 제공**되어야 합니다.
    *   **(검증)** `applied_principles` 필드는 단순한 참조가 아니라, **`C` 문서에 정의된 '품질 게이트'에서 자동 검증의 기준으로 활용**되어야 합니다. 이를 통해 AI가 명시된 원칙을 실제로 준수했는지 보증합니다.

#### **0.0.1. 확장 운영 지침**
`gates.rules.md` 문서에 따라, 본 프로젝트는 다음의 자동화된 품질 게이트를 운영 원칙에 포함합니다.

*   **추적성 강제 (OCE-Coverage):** 모든 요구사항은 최소 1개의 구현 산출물과 1개의 테스트 케이스와 연결되어야 합니다.
*   **근거주의 원칙 (OCE-Anchor):** 모든 주장은 최소 1개 이상의 최신 근거(Evidence)를 가져야 합니다.
*   **사실 검증 (OCE-FactCheck):** 주장과 근거 간의 의미적 정합성을 시스템이 자동으로 검증합니다.
*   **ID 명명 규칙 (OCE-Naming):** 모든 시스템 엔티티 ID는 `^[A-Z]{3}-[A-Z0-9]{2,}-[0-9]{2,}$` 정규식 규칙을 준수해야 합니다.
*   **릴리스 게이트 (OCE-ReleaseGate):** 상기 핵심 게이트들이 모두 통과되어야만 릴리스가 승인됩니다.

#### **0.1. 역할 및 책임 매트릭스 (RACI Matrix)**

| 역할 (Role) | 담당자 (Assignee) | 책임 (Responsibility) |
| :--- | :--- | :--- |
| **게임 감독관 (Manager/Director)** | `인간 디자이너` | 게임의 비전 제시, '무엇을' 만들지 결정 (What & Why), 게임 디자인, 최종 콘텐츠 구현 및 밸런싱 |
| **수석 아키텍트 (Chief Architect)** | `AI 프로그래머` | 감독관의 비전을 '어떻게' 구현할지 설계 및 구축 (How), C++ 코어 시스템 개발, 성능 최적화, 안정성 확보 |

---

### **1. 이 문서의 목적**

이 문서는 ProjectAAA 게임 캐릭터상태시스템의 AI 주도 개발 프로세스의 **시작부터 끝까지 모든 단계를 총괄 지휘하는 최상위 마스터 플랜(Master Plan)**입니다.

본 문서의 역할은 다음과 같은 2단계로 구성됩니다.

1.  **설계 명세화 단계:** 첨부된 모든 소스 문서(기획서, 아키텍처 원칙 등)를 종합하여, 개발에 필요한 모든 기술적 계약, 데이터 스키마, 프로세스, 작업 계획을 담은 핵심 산출 문서(`AI Driver Pack`)를 확정합니다.
2.  **AI 주도 구현 및 검증 단계:** 확정된 `AI Driver Pack`을 기반으로 AI가 자율적으로 코드를 생성하고, 자동화된 품질 게이트를 통해 구현물의 정합성을 검증하며, 최종적으로 프로젝트의 완료까지 모든 진행 상황을 추적하는 중앙 대시보드 역할을 수행합니다.

결론적으로, 이 문서는 **'개발 전 과정을 정의하는 설계도 제작'과 'AI의 자동화된 시공 및 감리'** 라는 핵심 단계를 순차적으로 지휘하는 프로젝트의 유일한 중앙 관제탑 역할을 수행합니다.

---

### **2. 문서 관계 및 생명주기 (Document Relationship & Lifecycle)**

#### **2.1. 입력 소스 문서 (Input Source Documents)**

> **[정의]** 설계 명세화 단계를 시작하기 위해 반드시 필요한 전제 조건이자 원재료가 되는 문서들입니다.

| 문서 ID (파일 경로) | 문서명 | 핵심 역할 (무엇을 하는 문서인가?) |
| :--- | :--- | :--- |
| **최상위 개발 원칙 (Global Principles)** |
| `A0_프로그램 개발 시 절대 준수 지침.md` | A0_프로그램 개발 시 절대 준수 지침 | AI의 역할, 품질 기준, 설계 원칙 등 개발의 최상위 철학을 정의합니다. |
| `A1_UnrealEngine C++ 개발 절대 지침.md` | A1_UnrealEngine C++ 개발 절대 지침 | 플러그인 기반 모듈화, BP-C++ 역할 분리 등 언리얼 엔진 개발의 핵심 원칙을 정의합니다. |
| `B0_AI 코드 오류 자가 점검 프로토콜.md`| B0_AI 코드 오류 자가 점검 프로토콜 | AI가 오류 발생 시 자가 해결하기 위한 5단계 워크플로우와 8가지 사고 원칙을 정의합니다. |
| `B1_AI 오류 수정 및 추가_삭제 결과 보고 가이드라인.md`| B1_AI 오류 수정 및 추가_삭제 결과 보고 가이드라인 | AI가 코드 수정 결과를 보고하는 표준 형식을 정의합니다. |
| **품질 및 데이터 정의 (Quality & Data Definition)** |
| `gates.rules.md` | gates.rules.v1 | CI 파이프라인에서 프로젝트 품질을 자동으로 검증하는 규칙(추적성, 근거, 명명규칙 등)을 YAML 형식으로 정의합니다. |
| `kernel.schema.md`| kernel.schema.v1 | 프로젝트에서 추출하고 연결할 엔티티(요구사항, 아티팩트, 테스트 등)의 구조를 YAML 형식으로 정의합니다. |
| **전사적 아키텍처 (Project-Wide Architecture)** |
| `Z1_ProjectAAA 게임_ UnrealEngine C++ 비동기 + 프레임 분산 기반 협업형 아키텍처.md`| Z1_ProjectAAA 게임_ UnrealEngine C++ 비동기 + 프레임 분산 기반 협업형 아키텍처 | 부하가 큰 작업을 비동기 및 프레임 분산으로 처리하기 위한 공통 인터페이스와 규칙을 정의합니다. |
| `Z2_ProjectAAA 게임_ 모듈 개발 지침서.md`| Z2_ProjectAAA 게임_ 모듈 개발 지침서 | 플러그인 기반 모듈의 격리, 통신, 빌드, 보안, 네이밍 규칙 등 하이브리드 개발 표준을 정의합니다. |
| `G00_1_ProjectAAA_게임 어빌리티_최상위 아키텍처_전략 설계도.md` | G00_1_ProjectAAA_게임 어빌리티_최상위 아키텍처_전략 설계도 | STAT, ITEM, SKIL 시스템 간의 상호작용을 위한 최상위 아키텍처(MVVM, Dispatcher, Interfaces)를 정의합니다. |
| `G00_2_ProjectAAA_게임어빌리티_STATITEMSKIL UI-MVVM & InteractionHub 통합 템플릿 PR_실행 계획서.md` | G00_2_ProjectAAA_게임어빌리티_STATITEMSKIL UI-MVVM & InteractionHub 통합 템플릿 PR_실행 계획서 | G00_1 아키텍처를 구현하기 위한 구체적인 실행 계획과 기술 명세를 정의합니다. |
| `G01_ProjectAAA 게임_어빌리티_캐릭터 상태 시스템 개발 지침서.md` | G01_...캐릭터 상태 시스템 개발 지침서 | STAT 시스템의 상세 기술 구현 명세(Modifier, 태그 매칭, 비동기 처리, 핵심 재미 요소 알고리즘 등)를 정의합니다. |
| `G02_ProjectAAA 게임_어빌리티_아이템 시스템 개발 지침서.md` | G02_...아이템 시스템 개발 지침서 | ITEM 시스템의 상세 기술 구현 명세(에고 아이템, 유물/저주, 핵심 재미 요소 연동 등)를 정의합니다. |
| `G03_ProjectAAA 게임_어빌리티_스킬 시스템 개발 지침서.md` | G03_...스킬 시스템 개발 지침서 | SKIL 시스템의 상세 기술 구현 명세(조합형 스킬, 융합/증폭, 과부하, 핵심 재미 요소 연동 등)를 정의합니다. |
| **시스템별 기획서 (GDD - Game Design Documents)** |
| `Mp00_ProjectAAA 핵심 전투 재미 요소 기획서.md` | Mp00_ProjectAAA 핵심 전투 재미 요소 기획서 | 전투 경험의 핵심 철학(모멘텀, 시너지, 클라이맥스)과 이를 구현하기 위한 시스템(동적 시너지, 카운터플레이 등)을 기획합니다. |
| `Mp01_ProjectAAA 캐릭터 상태 시스템 _01메인 기획서.md` | Mp01_...캐릭터 상태 시스템 _01메인 기획서 | STAT 시스템의 기획 요구사항(태그 기반 스탯, 스탯 업그레이드, MVVM, 스탯 미리보기 등)을 정의합니다. |
| `Mp02_ProjectAAA 아이템 시스템 _01메인 기획서.md` | Mp02_...아이템 시스템 _01메인 기획서 | ITEM 시스템의 기획 요구사항(랜덤 옵션, 에고 아이템 성장/진화, 인벤토리 검색 등)을 정의합니다. |
| `Mp03_ProjectAAA 스킬 시스템 _01메인 기획서.md` | Mp03_...스킬 시스템 _01메인 기획서 | SKIL 시스템의 기획 요구사항(조합형 커스텀 스킬, 룬 업그레이드, 동적 툴팁 등)을 정의합니다. |
| **(기타 카테고리...)** |
| `CL01_캐릭터 상태 시스템 STAT -세부 상세 체크리스트 .md` | CL01_캐릭터 상태 시스템 STAT -세부 상세 체크리스트 | STAT 시스템의 완료를 판정하기 위한 전사적 원칙, 기획, 기술, 데이터, 테스트 요구사항을 총망라한 체크리스트입니다. |
| `CL02_아이템 시스템 ITEM -세부 상세 체크리스트.md` | CL02_아이템 시스템 ITEM -세부 상세 체크리스트 | ITEM 시스템의 완료를 판정하기 위한 종합 체크리스트입니다. |
| `CL03_스킬 시스템 SKIL -세부 상세 체크리스트.md` | CL03_스킬 시스템 SKIL -세부 상세 체크리스트 | SKIL 시스템의 완료를 판정하기 위한 종합 체크리스트입니다. |

#### **2.2. 핵심 산출 문서 (Core Output Documents)**

> **[정의]** 설계 명세화 단계의 결과물로 생성되며, AI 주도 구현 및 검증 단계에서 **참조 문서**로 사용되는 핵심 결과물들입니다.

| 산출물 분류 (Output Category) | 문서 ID (파일 경로) | 핵심 역할 (Core Role) |
| :--- | :--- | :--- |
| **A. 핵심 계약 (Core Contract)** | `/AI_DriverPack/A.STAT_Contract.md` | 시스템의 Public API, 인터페이스, 데이터 구조 등 외부와 맺는 불변의 기술적 약속을 정의합니다. |
| **B. 가변 데이터 스키마 (Mutable Data Schema)** | `/AI_DriverPack/B.STAT_Schema.md` | 게임의 밸런스, 규칙, 콘텐츠 등 자주 변경되는 모든 데이터를 YAML 형식으로 정의하는 단일 진실 공급원입니다. |
| **C. 자동화 프로세스 및 품질 게이트 (Process & Gates)** | `/AI_DriverPack/C.STAT_Process_and_Gates.md` | B(스키마)의 변경이 어떻게 코드로 변환되는지 개발 워크플로우와, 그 결과물의 품질을 보증하는 자동화 테스트 명세를 정의합니다. |
| **D. 시각적 명세 및 흐름도 (Visual Specification)** | `/AI_DriverPack/D.STAT_Visuals.md` | A, B, C 문서의 복잡한 구조와 로직을 다이어그램으로 시각화하여 이해를 돕습니다. |
| **E. 프로젝트 골격 계약 (Project Skeleton)** | `/AI_DriverPack/E.STAT_Skeleton.md` | AI가 생성해야 할 모든 폴더와 파일의 물리적 구조를 정의한 청사진입니다. |
| **F. 작업 지시서 (Job Orders)** | `/AI_DriverPack/F.STAT_JobOrders.md` | C(프로세스)에 정의된 각 개발 단계를 실행하기 위한 구체적인 AI 작업 지시서(프롬프트, 산출물, 완료 조건 등)입니다. |
| **실행 계획 및 완료 기준 (Execution Plan & DoD)** | `/AI_DriverPack/CL01_캐릭터 상태 시스템 STAT -세부 상세 체크리스트 .md` | 프로젝트의 모든 요구사항을 총망라하여, 각 기능의 최종 완료(Done) 여부를 판정하는 마스터 체크리스트입니다. |

---

## **3. AI 주도 개발 실행 프레임워크 (AI-Driven Development Execution Framework)**

> **[최종 완료 기준 | FINAL & ABSOLUTE]**
> 아래의 모든 개발 단계의 체크리스트 항목이 완수되어야만 프로젝트가 '완료(Done)'로 인정됩니다.

### **3.1 설계 명세화 단계 완료 체크리스트**

> **[게이트 조건]** 아래의 모든 항목이 체크 완료되어야만, 다음 단계로 진행할 수 있습니다.

| ID | 작업 항목 | 산출물 (파일 경로) | 상태 |
| :--- | :--- | :--- | :--- |
| **A** | **시스템의 외부 약속(API, 인터페이스) 정의** | | |
| A-1 | STAT 시스템의 불변 계약 정의 완료 | `/AI_DriverPack/A.STAT_Contract.md` | [ ] |
| **B** | **시스템의 모든 가변 데이터(규칙, 콘텐츠) 정의** | | |
| B-1 | STAT 시스템의 데이터 스키마 정의 완료 | `/AI_DriverPack/B.STAT_Schema.md` | [ ] |
| **C** | **개발 워크플로우 및 품질 보증 기준 정의** | | |
| C-1 | STAT 시스템의 자동화 프로세스 및 품질 게이트 정의 완료 | `/AI_DriverPack/C.STAT_Process_and_Gates.md`| [ ] |
| **D** | **시스템 구조 및 로직 시각화** | | |
| D-1 | STAT 시스템의 아키텍처 및 로직 흐름도 작성 완료 | `/AI_DriverPack/D.STAT_Visuals.md` | [ ] |
| **E** | **물리적 파일 및 폴더 구조 정의** | | |
| E-1 | STAT 시스템의 프로젝트 골격 정의 완료 | `/AI_DriverPack/E.STAT_Skeleton.md` | [ ] |
| **F** | **AI 실행을 위한 구체적인 작업 계획 수립** | | |
| F-1 | STAT 시스템의 AI 작업 지시서 작성 완료 | `/AI_DriverPack/F.STAT_JobOrders.md` | [ ] |
| **CL** | **프로젝트 전체의 완료 기준 정의** | | |
| CL-1| STAT 시스템의 세부 상세 체크리스트 작성 완료 | `/AI_DriverPack/CL01_캐릭터 상태 시스템 STAT -세부 상세 체크리스트 .md` | [ ] |
| **META** | **최종 산출물 검수** | | |
| META-1 | `부록) 산출물 체크리스트(파일 레벨)`의 모든 파일 생성 완료 | `/AI_DriverPack/` | [ ] |

---

### **3.2 AI 주도 구현 및 검증 단계 대시보드**

> **[게이트 조건]** 설계 명세화 단계이 완료된 후, 아래 대시보드를 통해 실제 개발을 추적하고 완료를 판정합니다.

#### **3.2.1. Active Task Dashboard — Epic 레벨**

| Epic ID | Epic/Feature | System | 핵심 산출물 | 상태 | 담당 | 목표 Sprint |
|---|---|---|---|---|---|---|
| `EPIC-STAT-01` | 태그 기반 스탯 정의 및 업그레이드 | STAT | `USTATAbilityComponent`, `DT_StatDefinition` | Planned | | S‑01 |
| `EPIC-STAT-02` | 동적 세트 시너지 | STAT | `USTATAbilityComponent::CalculateSynergyScore`, `DT_SynergyGroupRules` | Planned | | S‑01 |
| `EPIC-STAT-03` | MVVM UI 연동 및 스탯 미리보기 | STAT, UI | `USTAT_StatusViewModel`, `ISTAT_Comparable_If` | Planned | | S‑01 |
| `EPIC-CORE-01` | 전략적 카운터플레이 | CORE, STAT, SKIL, ITEM | `ISTAT_Damageable_If`, `State.Vulnerable.To.Counter.*` | Planned | | S‑02 |
| `EPIC-CORE-02` | 모멘텀(Flow) 버프 및 시그니처 차지 | CORE, STAT | `Flow.Stack`, `Signature.Charge.Stack` | Planned | | S‑02 |
| `EPIC-ITEM-03` | 유물(Relic) & 저주(Curse) | ITEM, STAT | `DA_ITEM_Base::RelicEffectLogic`, `Source.Relic` | Planned | | S‑03 |
| `EPIC-SKIL-02` | 스킬 융합(Fusion) & 융합 증폭(Amplification) | SKIL, STAT | `DT_SkillFusionRules`, `Fusion.State.Amplified` | Planned | | S‑03 |
| `EPIC-SKIL-03` | 과부하(Overload) | SKIL, STAT | `FOverloadParameters`, `I_STAT_Overloadable_If` | Planned | | S‑03 |

> 상태코드: Planned / In‑Progress / Blocked / Review / Done

#### **3.2.2. Traceability Matrix — 기획 ↔ 구현 체크**

| 기획 원천 | 요구 기능 | 구현 태스크 ID(다중 가능) | 데이터 소스(B) | DoD 요약 |
|---|---|---|---|---|
| `Mp01`| 태그 기반 동적 스탯 및 성장 규칙 | `EPIC-STAT-01`| `/AI_DriverPack/B.STAT_Schema.md`| `DT_StatDefinition` 기반 스탯 생성 및 업그레이드 기능 구현 |
| `Mp00`| 빌드 자체가 세트 효과가 되는 동적 시너지 | `EPIC-STAT-02`| `/AI_DriverPack/B.STAT_Schema.md`| `DT_SynergyGroupRules` 기반 시너지 점수 계산 및 레벨 변환 |
| `Mp01`| MVVM UI 연동 및 스탯 변화 미리보기 | `EPIC-STAT-03`| `/AI_DriverPack/B.STAT_Schema.md`| `ISTAT_Comparable_If`를 통해 가상 스탯 비교 결과를 UI에 표시 |
| `Mp00`| 보스 패턴 무력화 및 보상 | `EPIC-CORE-01`| `/AI_DriverPack/B.STAT_Schema.md`| 약점 상태 태그와 공격 태그 매칭 시 보스 그로기 및 보상 실행 |
| `Mp00`| 모멘텀(Flow) 버프 | `EPIC-CORE-02`| `/AI_DriverPack/B.STAT_Schema.md`| 공격 성공 시 스택 누적 및 스택 수에 따른 보상 적용 |
| `Mp02`| 유물(Relic) & 저주(Curse) 시스템 | `EPIC-ITEM-03`| `/AI_DriverPack/B.ITEM_Schema.md`| 유물 장착 시 고유 패시브 발동 및 저주 스탯 페널티 적용 |

#### **3.2.3. 시스템별 완료 기준 및 초세분화 체크리스트 (System-Specific DoD & Granular Checklists)**

| 시스템 (System) | 담당 체크리스트 문서 (Checklist Document) | 진행률 (Progress) | 상태 (Status) |
| :--- | :--- | :--- |:---|
| `STAT` | `/AI_DriverPack/CL01_캐릭터 상태 시스템 STAT -세부 상세 체크리스트 .md` | `(자동 집계)` | `Planned` |
| `ITEM` | `/AI_DriverPack/CL02_아이템 시스템 ITEM -세부 상세 체크리스트 .md` | `(자동 집계)` | `Planned` |
| `SKIL` | `/AI_DriverPack/CL03_스킬 시스템 SKIL -세부 상세 체크리스트 .md` | `(자동 집계)` | `Planned` |

---

#### **3.2.4. 성능·안정성·보안 — 엔지니어링 게이트 체크**
- [ ] **플러그인 기반 모듈화 (Source: A1):** STAT 시스템은 외부 C++ 의존성이 없는 독립된 플러그인으로 존재해야 합니다.
- [ ] **상호작용 및 데이터 흐름 (Source: A1, G00_1):** 모든 외부 시스템과의 상호작용은 정의된 C++ 인터페이스를 통해서만 이루어져야 합니다.
- [ ] **비동기 처리 (Source: Z1):** 복잡한 계산(예: 동적 세트 시너지 재계산)은 `UE::Tasks`와 `FTimeSlicedTask`를 사용하여 비동기 및 프레임 분산 처리해야 합니다.
- [ ] **네이밍 및 골격 (Source: Z2):** 모든 C++ 클래스, 구조체, 열거형은 `USTAT_`, `FSTAT_`, `ESTAT_` 접두사를 사용해야 합니다.
- [ ] **성능 및 안전성 (Source: Z1):** 프레임 분산 처리 로직은 단일 스텝 당 1ms를 초과할 수 없으며, 모든 비동기 콜백은 `SAFE_WEAK_LAMBDA`를 사용해야 합니다.
- [ ] **보안 및 검증 (Source: A1):** 모든 포인터 접근 전 `IsValid()` 또는 `nullptr` 검사를 의무화하여 크래시를 원천 차단해야 합니다.
- **세부 자동화 게이트 (from `gates.rules.md`):**
    - [ ] `OCE-Coverage`: 모든 요구사항은 구현과 테스트로 추적 가능한가?
    - [ ] `OCE-Anchor`: 모든 주장은 유효한 최신 근거를 가지고 있는가?
    - [ ] `OCE-FactCheck`: 주장과 근거의 의미적 정합성 점수가 임계값 이상인가?
    - [ ] `OCE-PII`: 개인정보(PII) 의심 내용이 없는가?
- **DoD**: 상기 모든 원칙이 준수되고, `CL01` 체크리스트의 모든 항목이 완료되어야 합니다.

#### **3.2.5. 테스트 전략 — FunctionalTest 시나리오 총괄**
- [ ] **STAT**: `/AI_DriverPack/CL01_캐릭터 상태 시스템 STAT -세부 상세 체크리스트 .md`의 모든 DoD 시나리오 참조.
- [ ] **ITEM**: `/AI_DriverPack/CL02_아이템 시스템 ITEM -세부 상세 체크리스트 .md`의 모든 DoD 시나리오 참조.
- [ ] **SKIL**: `/AI_DriverPack/CL03_스킬 시스템 SKIL -세부 상세 체크리스트 .md`의 모든 DoD 시나리오 참조.

#### **3.2.6. 누락·왜곡 방지용 점검 루프**
- [ ] **문맥 주입 준수 검증 (Context Injection Compliance):** CI 과정에서 `F.STAT_JobOrders.md`의 `applied_principles`에 명시된 원칙들이 실제 코드에 반영되었는지 자동 검사합니다.
- [ ] **스키마-코드 동기화 검증 (Schema-Code Sync):** `B.STAT_Schema.md`의 모든 항목(스탯 정의, 시너지 규칙 등)이 코드베이스(데이터 테이블, C++ 로직)에 1:1로 반영되었는지 검증합니다.
- [ ] **계약-구현 일치성 검증 (Contract-Implementation Match):** `A.STAT_Contract.md`에 정의된 모든 인터페이스 시그니처가 실제 구현과 정확히 일치하는지 검증합니다.

---

### **3.3 부록) 산출물 체크리스트(파일 레벨)**

> **주:** 본 체크리스트는 `3.1 설계 명세화 단계 완료 체크리스트`의 `META-1` 항목을 검증하는 최종 실물 증거입니다.

- [ ] `/AI_DriverPack/A.STAT_Contract.md`
- [ ] `/AI_DriverPack/B.STAT_Schema.md`
- [ ] `/AI_DriverPack/C.STAT_Process_and_Gates.md`
- [ ] `/AI_DriverPack/D.STAT_Visuals.md`
- [ ] `/AI_DriverPack/E.STAT_Skeleton.md`
- [ ] `/AI_DriverPack/F.STAT_JobOrders.md`
- [ ] `/AI_DriverPack/CL01_캐릭터 상태 시스템 STAT -세부 상세 체크리스트 .md`
- [ ] `/Plugins/{...}/*_Test.umap` + 자동화 테스트
    -   **주:** 이 경로는 기능의 **최종 완료 상태(Definition of Done)**를 의미합니다. 실제 개발 과정은 프로젝트의 개발 지침서(예: Z2)에 명시된 접근법(예: 하이브리드 접근법)에 따라 프로젝트 소스에서 시작한 후, 완료 직전 단계에서 플러그인으로 이전하는 것을 권장합니다. 버전 관리는 Git과 같은 외부 전문 도구에 위임하는 것을 원칙으로 합니다.

---

### **4. 대시보드 및 자동화 연동 (Dashboard & Automation Integration)**

*   **동기화 원칙:** 본 문서는 프로젝트의 **'초기화 시드(Seed)'** 역할을 합니다. 최초 생성 후, 아래의 자동화 스크립트를 통해 지정된 PM 도구와 상태가 동기화됩니다.
*   **PM 도구:** `Jira`
*   **자동화 스크립트:**
    *   **`init_dashboard.py`**: 이 문서를 파싱하여 PM 도구에 Epic, Task, Checklist를 최초 생성합니다.
    *   **`sync_status.py`**: PM 도구의 상태 변경(예: 'In-Progress' -> 'Review')을 감지하여, 이 문서의 대시보드 상태를 주기적으로 업데이트합니다.
    *   **`generate_report.py`**: 이 문서의 최신 상태를 기반으로 주간 진행률 보고서를 자동 생성합니다.