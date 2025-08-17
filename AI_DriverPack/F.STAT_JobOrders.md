### **F. 작업 지시서(Job Order): 캐릭터 상태(STAT) 시스템**

#### **I. Job Order 표준 템플릿 (YAML)**

##### **템플릿 1: `STAT-000 (기초 공사)`**
```yaml
# ==========================================================
# F — 기초 공사(Foundation)를 위한 표준 작업 지시서
# ==========================================================
- id: "STAT-000"
  title: "[T0] STAT 시스템: 핵심 데이터 구조 구현 및 기본 골격 생성"
  description: "A, B, C 문서를 기반으로 E.STAT_Skeleton.md에 명시된 모든 파일과 폴더를 생성하고, 핵심 인터페이스 및 데이터 구조의 보일러플레이트 코드를 작성한다."
  applied_principles:
    - "A0_프로그램 개발 시 절대 준수 지침.md"
    - "A1_UnrealEngine C++ 개발 절대 지침.md"
    - "Z2_ProjectAAA 게임_ 모듈 개발 지침서.md"
  
  deliverables:
    - "E.STAT_Skeleton.md에 명시된 모든 파일 및 폴더 구조"
    - "A.STAT_Contract.md에 정의된 인터페이스 및 구조체의 C++ 헤더 파일"
  
  acceptance_criteria:
    - "E.STAT_Skeleton.md의 모든 파일과 폴더가 정확히 생성되었는가?"
    - "생성된 프로젝트가 에러 없이 컴파일되는가?"
    
  pm_sync:
    tool: "jira"
    project_key: "PROJ"
    issue_type: "Task"
    summary_template: "[STAT] {title}"
```

##### **템플릿 2: `STAT-001 (스키마 동기화)`**
```yaml
# ==========================================================
# F — 스키마 동기화(Schema Sync)를 위한 표준 작업 지시서
# ==========================================================
- id: "STAT-001"
  title: "[T1] STAT 시스템: [B]스키마 변경 사항 자동 동기화"
  description: "B.STAT_Schema.md의 변경 사항을 감지하여, 관련 코드, 데이터 테이블, 자동화 테스트, 시각적 명세를 자동으로 생성 및 업데이트한다."
  applied_principles:
    - "A0_프로그램 개발 시 절대 준수 지침.md"
    - "A1_UnrealEngine C++ 개발 절대 지침.md"
    - "Z1_ProjectAAA 게임_ UnrealEngine C++ 비동기 + 프레임 분산 기반 협업형 아키텍처.md"
    - "Z2_ProjectAAA 게임_ 모듈 개발 지침서.md"
    - "B0_AI 코드 오류 자가 점검 프로토콜.md"
    - "B1_AI 오류 수정 및 추가_삭제 결과 보고 가이드라인.md"
  
  deliverables:
    - "스키마 변경에 따른 C++ 코드 수정/생성 (예: 스탯 계산 로직)"
    - "스키마 변경에 따른 UDataTable 에셋 업데이트"
    - "C.STAT_Process_and_Gates.md에 명시된 관련 Functional Test 코드 생성"
    - "D.STAT_Visuals.md 다이어그램 업데이트"

  acceptance_criteria:
    functional_tests_must_pass: ["C.STAT_Process_and_Gates.md에 정의된 모든 관련 테스트 케이스"]
    reviewer_gates_must_pass: ["A.STAT_Contract.md의 모든 강제 규약"]
    
  pm_sync:
    tool: "jira"
    project_key: "GAME"
    issue_type: "Task"
    summary_template: "[STAT] B 스키마 동기화 실행"
    labels: ["auto","schema-sync","stat"]
    generate_subtasks_from: "schema_diff" # 변경된 각 스키마 항목을 Jira 서브태스크로 자동 생성
```