### **B2-최종 점검을 위한 표준 운영 절차(Standard Operating Procedure, SOP).md (v2.1 Proposed)**

---

#### **범용 최종 점검 체크리스트 (SOP v2.1)**

---

#### **Phase 0: 입력 소스 및 메타데이터 무결성 검증 (Input Source & Metadata Integrity Verification)**
*이 단계는 검증할 파일 자체가 규칙을 준수하는지 확인합니다.*

*   **[0.1] 파일명 정책 준수 (Filename Policy Compliance):**
    *   [ ] 파일명 자체에 금지된 요소(예: 버전 정보 `_v1`)가 포함되지 않았는가?
    *   [ ] 파일명이 시스템의 표준 명명 규칙을 따르는가?
*   **[0.2] 파일 형식 및 역할 명확성 (File Format & Role Clarity):**
    *   [ ] 파일 확장자가 내용과 일치하며, 파일명이 그 역할을 명확히 나타내는가?
*   **[0.3] 정책 메타데이터 검증 (Policy Metadata Verification) (신설)**:
    *   [ ] **(MGI 계약 확인)** 모든 GDD, `.MDC` 등 핵심 설계 문서 최상단에 `mgiContract` YAML 블록이 존재하는가?
    *   [ ] **(문서 Lint)** 문서 본문에 `“반드시 중앙 허브”`와 같은 금칙어 패턴이 포함된 경우, `mgiContract.comms.central_hub_required: true` 선언과 일치하는가? 불일치 시 **FAIL**.

---


#### **Phase 1: 계획 검증 (Planning Verification)`**

*   **`[1.1] CoT 감사 로그 생성:`** AI는 `planning_log.md`를 먼저 생성한다.
*   **`[1.2] 계획-요구사항 교차 검증:`** 생성된 `planning_log.md`의 계획이 `[2.0]`의 '마스터 리스트'와 일치하는지 검증한다. 불일치 시 즉시 **FAIL**.

---

#### **Phase 2: 내용 무결성 검증 (Content Integrity Verification)**

*   **`[2.0] 요구사항 범위 검증 (Requirement Scope Validation)`**
    *   [ ] **(마스터 목록 추출)** 요구사항 문서에서 전체 산출물 목록을 기계적으로 추출한다.
    *   [ ] **(생성 계획 대조)** AI의 생성 계획이 마스터 목록의 모든 항목을 포함하는지 교차 검증한다.
    *   [ ] **(게이트 조건)** 누락된 항목이 있을 경우, 즉시 **FAIL**을 반환하고 중단한다.
*   **[2.1] 최신성 (Recency):**
    *   [ ] 사용자의 가장 마지막 지침, 수정 요청, 피드백이 모두 반영되었는가?
*   **[2.2] 정확성 (Accuracy):**
    *   [ ] 내용이 원본 소스 문서의 사실과 일치하는가? (왜곡 방지)
*   **[2.3] 완전성 (Completeness):**
    *   [ ] 원본의 핵심 요구사항이 생략되지 않았는가? (누락 방지)
*   **[2.4] 가독성 (Readability):**
    *   [ ] 명백한 오탈자나 문법적 오류가 없는가? (오타 방지)
*   **`[2.5] 잔여 지시문 부재 검증 (Absence of Remnant Directives):`**
    *   [ ] 최종 산출물에 `"[AI 지침:"`, `"[TODO:"` 같은 패턴이 존재하지 않는가?
*   **`[2.6] 정량적 일치 검증 (Quantitative Matching):`**
    *   [ ] (GDD의 Epic 개수) == (대시보드의 Epic 개수) 등 정량적 지표가 일치하는가?

---

#### **Phase 3: 기능적 무결성 검증 (Functional Integrity Verification)**

*   **[3.1] (템플릿/지시서 문서) 실행 가능성 (Executability):**
    *   [ ] AI를 위한 실행 지침 블록(예: `[META INSTRUCTION BLOCK]`)이 존재하는가?
*   **[3.2] (데이터/스키마 문서) 파싱 가능성 (Parsability):**
    *   [ ] 기계가 읽어야 할 코드 블록(YAML, JSON 등)이 문법적으로 유효한가?
*   **[3.3] (실행 계획/Job Order 문서) 구조 준수 (Structural Compliance):**
    *   [ ] 정의된 템플릿의 모든 필수 필드가 포함되어 있는가?

---

#### **Phase 4: 시스템 무결성 검증 (System Integrity Verification)**
*이 단계는 문서 세트 전체의 상호 관계와 시스템으로서의 완결성을 확인합니다.*

*   **[4.1] 상호 참조 유효성 (Cross-Reference Validity):**
    *   [ ] 문서 간 참조 경로 및 파일명이 정확하고 유효한가?
*   **[4.2] 워크플로우 연속성 (Workflow Continuity):**
    *   [ ] 이전 단계의 산출물이 다음 단계의 입력으로 올바르게 연결되는가?
*   **`[4.3] 실행 가능한 규칙 매트릭스 검증 (Executable Rule Matrix Verification) (고도화)`**:
    *   **[4.3.1] 규칙 키 추출:** 시스템의 모든 핵심 문서(GDD, Z1, Z2, `.MDC` 등)에서 `mgiContract` 블록을 스캔하여 정책 키-값 쌍(예: `comms.mode`, `comms.central_hub_required`)을 추출한다.
    *   **[4.3.2] 규칙 매트릭스 생성:** 추출된 키-값 쌍으로 '규칙 매트릭스'를 구성한다.
        | Document | `comms.mode` | `comms.central_hub_required` |
        | :--- | :--- | :--- |
        | GDD_v1.6.md | `(undefined)` | `true` |
        | ZG01.MDC.md | `MessageBus` | `false` |
    *   **[4.3.3] 정책 단언(Assertion) 실행:** 사전에 정의된 규칙으로 매트릭스의 유효성을 검증한다.
        *   **Assertion 1:** `IF GDD.comms.central_hub_required == true THEN ZG01.comms.mode != 'MessageBus'`.
    *   **[4.3.4] 게이트 조건:** 단 하나의 단언이라도 실패할 경우, **SOP는 즉시 'FAIL'을 반환하고** 충돌하는 규칙과 문서를 명시하여 보고한다. (예: "FAIL: GDD의 중앙 허브 정책과 MDC의 분산 통신(MessageBus) 정책이 충돌합니다.")

---