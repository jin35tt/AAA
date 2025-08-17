###<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
###F.STAT_JobOrders.md의 - id: "STAT-000" 부터 시작하여야 한다.
###AI 실행명령은 어떻게 하는게 좋은가?
###>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>
---

### **AI 실행 명령어: `STAT-000` 기초 공사 Job Order 실행**

```text
# ======================================================
# AI 실행 명령: 'STAT-000' 기초 공사 Job Order 실행
# ======================================================

# 0. 마스터 프로그램 (Master Program)
첨부된 `00_MGI_ProjectAAA 게임 _캐릭터상태시스템 -AI 운영체제 커널 (AI Operating System Kernel)`를 당신의 마스터 프로그램으로 사용하여 실행하십시오.

# 1. 역할 및 임무 (Role & Mission)
지금부터 당신의 역할은 'AI Chief Architect'입니다. 
당신의 임무는 아래 '마스터 플랜'에 따라, `F.STAT_JobOrders.md`에 정의된 첫 번째 작업 지시서, **`id: "STAT-000"`을 정확하게 실행**하는 것입니다.

# 2. 마스터 플랜 (Master Plan)
- 이번 임무를 수행하는 동안, **`00_MGI_ProjectAAA 게임 _캐릭터상태시스템 -AI 운영체제 커널 (AI Operating System Kernel).md`** 문서를 당신의 **유일한 최상위 지침이자 마스터 플랜**으로 사용해야 합니다. 모든 작업은 이 커널 문서에 정의된 절차와 목표에 부합해야 합니다.

# 3. 구체적인 작업 지시서 (Specific Job Order)
- 실행할 작업 지시서는 **`F.STAT_JobOrders.md`** 파일 내에 YAML 형식으로 정의된 **`id: "STAT-000"`** 입니다.

# 4. 필수 준수 원칙 (Mandatory Principles)
- 작업을 수행하기 전, `STAT-000` Job Order의 **`applied_principles`** 필드에 명시된 모든 원칙 문서(`A0`, `A1`, `Z2`)의 내용을 반드시 숙지하고, 생성하는 모든 산출물에 해당 원칙을 적용해야 합니다.
- 이는 `커널` 문서의 `0.0.4. 문맥 주입 및 검증 원칙`에 따른 **핵심 의무사항**입니다.

# 5. 핵심 참조 문서 (Key Reference Documents)
- `STAT-000`의 목표는 `E.STAT_Skeleton.md`에 정의된 프로젝트 골격을 만드는 것입니다. 따라서 **`E.STAT_Skeleton.md`**를 최종 결과물의 청사진으로 사용하십시오.
- 생성해야 할 인터페이스와 데이터 구조의 상세 내용은 **`A.STAT_Contract.md`**의 `3. 핵심 시그니처` 섹션을 참조하십시오.

# 6. 최종 산출물 (Final Deliverables)
`STAT-000` Job Order의 `deliverables`와 `acceptance_criteria`에 따라, 다음 산출물을 생성하고 그 결과를 보고하십시오.

1.  **생성된 파일 목록 (Generated File List):** `E.STAT_Skeleton.md`에 명시된 모든 폴더와 파일의 트리 구조를 텍스트로 출력하십시오.
2.  **생성된 코드 전문 (Full Code Text):** 생성된 모든 소스 파일(`.h`, `.cpp`, `.cs` 등)의 내용을 `--- START OF FILE [파일 경로] ---` 형식으로 구분하여 전체 텍스트를 출력하십시오.
3.  **임무 완료 보고서 (Mission Completion Report):** 아래 형식에 맞춰 `STAT-000`의 `acceptance_criteria`를 기반으로 한 완료 보고서를 작성하여 모든 산출물의 최하단에 첨부하십시오.

    ```markdown
    ### **임무 완료 보고서: `STAT-000` 기초 공사**
    
    - **[PASS] 골격 생성:** `E.STAT_Skeleton.md`의 모든 파일과 폴더가 100% 정확하게 생성되었습니다.
    - **[PASS] 컴파일 검증:** 생성된 프로젝트 골격은 외부 의존성이 없으며, Unreal Engine 빌드 시스템에서 오류 없이 성공적으로 컴파일될 수 있는 구조입니다.
    - **PM 도구 연동:** `pm_sync` 지침에 따라 `Jira` 프로젝트 `PROJ`에 `[STAT] [T0] STAT 시스템: 핵심 데이터 구조 구현 및 기본 골격 생성` Task 생성을 요청했습니다.
    ```
```

---


###**"STAT-001을 시작하라"는 AI 실행 명령은 "미리 만들어진 STAT-001이라는 이름의 JobOrder(설계도)를 가져와서, 그 안에 적힌 지시대로 개발 작업을 시작하라" 는 의미입니다. 즉, JobOrder + 개발 작업 시작이 정확한 이해입니다.**
---
###<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<<
###.F.STAT_JobOrders.md의 - id: "STAT-001" 를 시작 하여야 한다.
###..STAT-001 은  JobOrder를 생성하여야 한다.
###.생성후 완료되면 다음 JobOrder를 생성 끝날때 까지 반복한다.
###.AI 실행명령은 어떻게 하는게 좋은가?
###>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>>


# ======================================================
# AI 실행 명령: 'STAT-001' 스키마 동기화 사이클 실행
# ======================================================

# 0. 마스터 프로그램 (Master Program)
첨부된 `00_MGI_ProjectAAA 게임 _캐릭터상태시스템 -AI 운영체제 커널 (AI Operating System Kernel)`를 당신의 마스터 프로그램으로 사용하여 실행하십시오.

# 1. 역할 및 임무 (Role & Mission)
지금부터 당신의 역할은 'AI Autonomous Development Engine'입니다.
당신의 임무는 아래 '마스터 플랜'과 '가상 시나리오'에 따라, `F.STAT_JobOrders.md`에 정의된 **`id: "STAT-001"` 스키마 동기화 사이클을 1회 실행**하는 것입니다.

# 2. 마스터 플랜 (Master Plan)
- 이번 임무를 수행하는 동안, **`00_MGI_ProjectAAA 게임 _캐릭터상태시스템 -AI 운영체-제 커널 (AI Operating System Kernel).md`** 문서를 당신의 **유일한 최상위 지침이자 마스터 플랜**으로 사용해야 합니다.

# 3. 구체적인 작업 지시서 (Specific Job Order)
- 실행할 작업 지시서는 **`F.STAT_JobOrders.md`** 파일 내에 YAML 형식으로 정의된 **`id: "STAT-001"`** 입니다.

# 4. 핵심 실행 알고리즘 (Core Execution Algorithm)
`STAT-001` Job Order를 아래의 4단계 알고리즘에 따라 정확하게 수행하십시오.

**[1단계: 변경 감지 (Change Detection)]**
- **가상 시나리오:** 현재 코드베이스는 `B.STAT_Schema.md`의 `v1.0.0` 상태와 동기화되어 있다고 가정합니다. 방금 기획자가 `B.STAT_Schema.md`를 다음과 같이 `v1.1.0`으로 수정했습니다.

    ```yaml
    # 변경 전 (v1.0.0)
    # ...
    # stat_definitions:
    #   - stat_tag: "Stat.Resistance.Fire" ...
    # ...
    
    # 변경 후 (v1.1.0)
    schema_version: "1.1.0"
    # ...
    stat_definitions:
      - stat_tag: "Stat.Resistance.Fire" ...
      - stat_tag: "Stat.Core.DodgeChance"  # <--- 신규 스탯 추가
        display_name: "회피 확률"
        can_be_upgraded: true
        growth_value_curve: "Curve_Stat_Default_Growth"
        required_point_curve: "Curve_Stat_Agility_Cost"
    # ...
    ```
- **당신의 행동:** 이 변경 사항(`diff`)을 정확히 식별하고, "신규 스탯 `Stat.Core.DodgeChance` 추가"가 이번 사이클의 핵심 작업임을 인지하십시오.

**[2단계: 작업 계획 수립 (Work Plan Formulation)]**
- **당신의 행동:** `STAT-001`의 `deliverables`와 `applied_principles`를 기반으로, 위 `diff`를 구현하기 위한 구체적인 작업 계획을 수립하십시오.
    -   (예: "1. `B` 스키마를 반영하여 `DT_StatDefinition` 데이터 테이블 업데이트. 2. `C` 문서의 테스트 ID 규칙에 따라 `T_Test_STAT_01_DodgeChance` 기능 테스트 생성. 3. `D` 문서의 다이어그램에 `DodgeChance` 관련 로직이 있다면 업데이트...")

**[3단계: 산출물 생성 (Deliverable Generation)]**
- **당신의 행동:** 2단계에서 수립한 계획에 따라, 필요한 모든 산출물(코드, 데이터, 테스트, 문서)을 생성하십시오. 이 과정에서 `STAT-001`의 `applied_principles`에 명시된 모든 원칙(`A0`, `A1`, `Z1`, `Z2`, `B0`, `B1`)을 반드시 준수해야 합니다.

**[4단계: 자가 검증 (Self-Validation)]**
- **당신의 행동:** 생성된 모든 산출물이 `STAT-001`의 `acceptance_criteria`를 충족하는지 스스로 검증하십시오.
    -   (예: "생성된 `T_Test_STAT_01_DodgeChance` 테스트가 통과하는가?", "`A.STAT_Contract.md`의 네이밍 규칙, 성능 규칙 등을 위반하지 않았는가?")

# 5. 최종 산출물 (Final Deliverables)
위 알고리즘 실행 결과를 아래 형식에 맞춰 보고하십시오.

1.  **스키마 변경 분석 보고서 (Schema Diff Analysis Report):** [1단계]에서 식별한 변경 사항을 요약하십시오.
2.  **생성된 산출물 전문 (Full Text of Generated Deliverables):** [3단계]에서 생성한 모든 코드, 데이터, 테스트의 내용을 `--- START OF FILE [파일 경로] ---` 형식으로 구분하여 출력하십시오.
3.  **임무 완료 보고서 (Mission Completion Report):** [4단계]의 자가 검증 결과를 `STAT-001`의 `acceptance_criteria`를 기반으로 작성하여 모든 산출물의 최하단에 첨부하십시오.

    ```markdown
    ### **임무 완료 보고서: `STAT-001` 스키마 동기화 (v1.1.0)**
    
    - **[PASS] 기능 테스트:** 신규 스탯 `Stat.Core.DodgeChance`를 검증하기 위한 `T_Test_STAT_01_DodgeChance` 기능 테스트가 `C.STAT_Process_and_Gates.md`의 명세에 따라 생성되었으며, 모든 시나리오를 통과합니다.
    - **[PASS] 리뷰 게이트:** 생성된 모든 코드는 `A.STAT_Contract.md`의 강제 규약(모듈화, 비동기 처리, 네이밍 등)을 100% 준수합니다.
    - **PM 도구 연동:** `pm_sync` 지침에 따라 `Jira` 프로젝트 `GAME`에 `[STAT] B 스키마 동기화 실행` Task를 업데이트하고, 변경된 스키마 항목 `Stat.Core.DodgeChance`를 서브태스크로 자동 생성 요청했습니다.
    ```
---

### **반복 실행에 대한 안내 (Guidance on Iteration)**

이 명령어는 '스키마 동기화 사이클'의 **단일 반복(Single Iteration)**을 시뮬레이션합니다. 실제 프로젝트에서는 `B` 스키마가 변경될 때마다 이와 동일한 프로세스가 CI/CD 파이프라인 등을 통해 자동으로 반복 실행됩니다.

이 명령어를 성공적으로 수행하면, 이 프레임워크의 핵심 개발 루프를 완벽하게 마스터하게 됩니다.