### **C. 자동화 프로세스 및 품질 게이트: 캐릭터 상태(STAT) 시스템**

#### **1. 개발 워크플로 정의 (Defining the Development Workflow)**
*   **`T0`: 기초 공사 (The Foundational Scaffolding)**:
    *   **[정의]** 실제 기능 개발(T1)에 앞서, `A.STAT_Contract.md`와 `B.STAT_Schema.md` 문서에 기반하여 프로젝트의 물리적인 폴더 구조(`E.STAT_Skeleton.md` 참조)와 핵심 파일을 구축하는 **필수적인 최초의 태스크**. 이 작업은 `F.STAT_JobOrders.md`의 `STAT-000` Job Order를 통해 실행된다.
*   **`T1`: 스키마 동기화 사이클 (The Schema Synchronization Cycle)**:
    *   **[정의]** `T0` 이후 반복되는 실제 기능 개발의 표준 프로세스. **`B.STAT_Schema.md` 문서의 변경을 감지하여 코드베이스와 자동으로 동기화**하는 `생성 → 리뷰 → 테스트 → 병합` 사이클을 의미한다. 이 작업은 `F.STAT_JobOrders.md`의 `STAT-001` Job Order를 통해 실행된다.
*   **`T-H`: 고난도 스키마 변경 처리 워크플로우 (Hybrid Workflow for H-Level Changes)**
    *   **[정의]** 동적 세트 시너지 계산 알고리즘 변경 등 시스템의 근간에 영향을 미치는 복잡한 `B` 스키마 변경을 위한 하이브리드 워크플로우. AI는 코드 생성 대신 작업 분해 계획(`subtasks`)을 먼저 제안하고, 인간 아키텍트의 승인을 받은 후 실행한다.

#### **2. 프롬프트 팩 구성 요소 (Prompt Pack Components)**
*   **`Codegen System Prompt` (코드 생성 프롬프트) 작성 가이드**:
    *   **[원칙]** AI의 작업 누락 및 오해를 방지하기 위해, 시스템 프롬프트에는 **반드시 준수해야 할 상위 원칙 문서(`A0`, `A1`, `Z1`, `Z2`, `B0`, `B1`)의 핵심 내용이 직접 주입**되어야 한다.
    *   *(예시)*
        > **SYSTEM:** 당신은 **`A0_프로그램 개발 시 절대 준수 지침`**과 **`A1_UnrealEngine C++ 개발 절대 지침`**, **`Z1_...비동기...아키텍처`**를 절대적으로 준수하는 **'수석 아키텍트'**입니다. 당신의 모든 산출물은 **10가지 품질 척도**를 만족해야 하며, 코드 생성 중 오류 발생 시 **`B0_AI 코드 오류 자가 점검 프로토콜`**에 따라 자율적으로 해결해야 합니다. `A.STAT_Contract.md` 계약을 준수하고 `B.STAT_Schema.md`의 YAML을 해석하여 다음 산출물을 생성하십시오: `.h/.cpp`, `FunctionalTest`, `DataAsset`, `D.STAT_Visuals.md` 업데이트.
*   **`PR Reviewer Prompt` (리뷰 자동화 프롬프트) 작성 가이드**:
    *   *(필수 항목)* 이 체크리스트는 반드시 **`A.STAT_Contract.md`의 강제 규약들과 1:1로 대응**되어야 한다. (예: `[ ] Z1 비동기 처리 규칙을 준수하는가?`, `[ ] A1 플러그인 격리 원칙을 준수하는가?`) 최종 결과는 명확한 **PASS/FAIL**로 귀결되어야 한다.

#### **3. 품질 게이트 명세 (Quality Gate Specification)**
*   **기능 테스트 명세 (Functional Test Specification)**:
    *   **원칙:** `B.STAT_Schema.md`에 정의된 핵심 기능 항목들은 반드시 하나 이상의 자동화된 기능 테스트 케이스와 1:1로 대응되어야 한다.
    *   **테스트 ID 규칙:** `T_Test_STAT_[EpicID]_[Feature]` 형식으로 명명하여 추적성을 보장한다.
    *   **필수 테스트 케이스 목록:**
        *   `T_Test_STAT_01_TagBasedStat`: `B` 스키마에 새 스탯 태그를 추가했을 때, 해당 스탯이 캐릭터에 정상적으로 적용되는지 검증.
        *   `T_Test_STAT_01_StatUpgrade`: `can_be_upgraded: true`인 스탯이 정상적으로 업그레이드되고 값이 반영되는지 검증.
        *   `T_Test_STAT_02_DynamicSynergy`: `B` 스키마의 `synergy_group_rules`에 따라 태그 조합 시 `synergy_level_thresholds`에 맞는 보너스가 적용되는지 검증.
        *   `T_Test_STAT_03_MVVMBinding`: 스탯 변경 시, ViewModel을 통해 UI에 바인딩된 값이 자동 갱신되는지 모의(Mock) UI로 검증.
        *   `T_Test_STAT_03_StatPreview`: `ISTAT_Comparable_If::GetStatDeltaComparedTo`가 정확한 스탯 변화량을 반환하는지 검증.
*   **문맥 주입 준수 검증 (Context Injection Compliance Gate):**
    *   **원칙:** CI 과정에서 자동화된 리뷰어는 `F.STAT_JobOrders.md`의 `applied_principles`에 명시된 원칙들이 실제 코드 변경분에 반영되었는지 검사해야 한다. (예: `Z1`이 명시된 경우, `UE::Tasks` 또는 `FTimeSlicedTask` 관련 코드가 포함되었는지 확인)

#### **4. 자동화 연동 계획 (Automation Integration Plan)**
*   **PM 도구 연동 어댑터 (PM Tool Integration Adapter):**
    *   CI 파이프라인은 'PM Sync Adapter' 스텝을 포함해야 한다. 이 어댑터는 `F.STAT_JobOrders.md`의 Job Order YAML 파일을 파싱하고, `pm_sync` 블록의 정보를 기반으로 `Jira`의 REST API를 호출하여 `EPIC-STAT-01`, `EPIC-STAT-02` 등의 이슈를 생성/업데이트하며, 생성된 이슈 키(ID)를 실행 로그에 반드시 기록해야 한다.