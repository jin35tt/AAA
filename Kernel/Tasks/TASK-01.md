### **Task ID: TASK-01**
### **Task: [에픽] 피해 처리 (Attack/Defense Tag Matching) 파이프라인 구현**
### **유형: Meta**

---
### **1. 목표 (Objective)**
*   이 에픽 Task의 목표는 `.MDC`에 정의된 피해 처리 파이프라인의 모든 단계를 구현하는 하위 Task들(`TASK-01.01` ~ `TASK-01.04`)의 성공적인 완료를 총괄하고 보장하는 것입니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
*   **Meta-Task의 경우:**
    - [ ] 모든 하위 Task(`TASK-01.01`, `TASK-01.02`, `TASK-01.03`, `TASK-01.04`)가 `Completed` 상태가 되어야 한다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `B0`, `B1`** 등의 모든 지침을 최우선으로 준수해야 합니다.
*   **프로젝트 규범:** 하위 Task들은 `G01` 개발 지침서의 코딩 컨벤션을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   `[AI 지침: Meta-Task는 이 섹션이 필요 없습니다.]`

---
### **5. 작업 지시 (Instructions)**
1.  이 Task는 하위 Task들의 진행을 관리합니다. 모든 하위 Task가 완료되면 이 Task도 완료됩니다.

---
### **6. 예외 처리 프로토콜 (Exception Handling Protocol)**
*   본 Task는 **`A-Type`** 프로토콜을 따릅니다. 하위 Task에서 중대한 문제 발생 시, 이 에픽 전체가 `[HALTED_CRITICAL]` 상태로 전환될 수 있습니다.
*   Task 수행 중 문제 발생 시, 즉시 실행을 중단하고 `2.2. 예외 처리 프로토콜`의 **상태 전이 규칙**에 따라 상태를 변경하고 보고해야 합니다.

### **7. 참조 소스 (Reference from Source)**
*   **핵심 청사진:** `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`
*   **요구사항 컨텍스트:** `Mp01_ProjectAAA 캐릭터 상태 시스템 _01메인 기획서.md` (특히 `9. 핵심 상호작용 모델: 공격/방어 태그 매칭 시스템`)

### **8. 소스 요구사항 매핑 (Source Requirement Mapping)**
*   `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`:
    ```yaml
    # 4.1 피해 처리 (Attack/Defense Tag Matching)
    pipeline:
      - step: CollectDefenseRules
        where: "GameThread"
        authority: "ServerOnly"
      - step: MatchAttackVsDefense
        where: "AsyncTask"
        authority: "ServerOnly"
      - step: ComputeFinalDamage
        where: "AsyncTask"
        authority: "ServerOnly"
      - step: ApplyDamageToTarget
        where: "GameThread"
        authority: "ServerOnly"
    ```

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-01"
  type: meta_task
  path: "N/A"
  owner: "ai-agent-planner"
  description: "[에픽] 피해 처리 (Attack/Defense Tag Matching) 파이프라인 구현"

---