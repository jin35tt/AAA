### **Task ID: TASK-00**
### **Task: [청사진] .MDC 기반 CharacterStatus 애플리케이션 스캐폴딩**
### **유형: A. 불변의 핵심 계약**

---
### **1. 목표 (Objective)**
*   `M_ProjectAAA 캐릭터 상태 시스템_mdc.md` 청사진을 분석하여, 'STAT' 플러그인의 전체 C++ 파일 구조, 핵심 클래스, 인터페이스, 데이터 구조체의 뼈대(skeleton)를 정의하는 '아키텍처 발자국'을 생성합니다. 이 Task는 실제 코드를 구현하는 것이 아니라, 모든 후속 개발 Task의 기반이 될 구조를 확립하는 것입니다.

---
### **2. 완료 조건 (Definition of Done - DoD)**
- [ ] `10. 아키텍처 발자국` 섹션이 `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`에 명시된 모든 인터페이스, 데이터 구조, 핵심 클래스를 포함하여 정확하게 채워져야 합니다.
- [ ] 생성될 모든 파일의 경로와 핵심 UObject의 스레딩 정책이 명시되어야 합니다.

---
### **3. 준수 원칙 (Applied Principles)**
*   **절대 규범:** **`A0`, `A1`, `Z0`, `Z1`, `Z2`** 등의 모든 아키텍처 및 개발 지침을 최우선으로 준수하여 구조를 설계해야 합니다.
*   **프로젝트 규범:** `G01_ProjectAAA 게임_어빌리티_캐릭터 상태 시스템 개발 지침서.md`의 네이밍 컨벤션(`STAT` 접두사 등)을 준수해야 합니다.

---
### **4. 논리적 설계 (알고리즘 의사코드)**
*   이 Task는 Meta-Task이므로, 구체적인 알고리즘 구현이 아닌 아키텍처 구조 생성을 목표로 합니다.

---
### **5. 작업 지시 (Instructions)**
1.  AI 개발 에이전트는 아래 **`10. 아키텍처 발자국`**에 명시된 파일과 디렉토리 구조를 `ProjectAAA` 프로젝트 내에 생성해야 합니다.
2.  각 헤더 파일에는 명시된 클래스, 구조체, 인터페이스의 선언부를 포함한 뼈대 코드를 작성해야 합니다. (구현부는 `// TODO`로 비워둡니다.)

---
### **6. 예외 처리 프로토콜 (Exception Handling Protocol)**
*   본 Task는 **`A-Type`** 프로토콜을 따릅니다.
*   Task 수행 중 문제 발생 시, 즉시 실행을 중단하고 `2.2. 예외 처리 프로토콜`의 **상태 전이 규칙**에 따라 상태를 변경하고 보고해야 합니다.

### **7. 참조 소스 (Reference from Source)**
*   **핵심 청사진:** `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`
*   **요구사항 컨텍스트:** `G01_ProjectAAA 게임_어빌리티_캐릭터 상태 시스템 개발 지침서.md`, `Mp01_ProjectAAA 캐릭터 상태 시스템 _01메인 기획서.md`, `Mp00_ProjectAAA 핵심 전투 재미 요소 기획서.md`

### **8. 소스 요구사항 매핑 (Source Requirement Mapping)**
*   `M_ProjectAAA 캐릭터 상태 시스템_mdc.md`: `pluginContract`, `interfaces`, `dataSchemas`, `pipeline` 섹션의 모든 내용을 분석하여 C++ 아키텍처로 변환합니다.

### **9. 커널 데이터베이스 등록 (Kernel Database Registration)**
# 이 Task의 결과물을 시스템에 등록합니다.
- entity: artifact
  id: "ART-TASK-00"
  type: code_architecture_scaffold
  path: "/Plugins/STAT/"
  owner: "ai-agent-architect"
  description: "[청사진] .MDC 기반 CharacterStatus 애플리케이션 스캐폴딩"

### **10. 아키텍처 발자국 (Architectural Footprint)**
```yaml
architecturalFootprint:
  files:
    - path: "/Plugins/STAT/Source/STAT/Public/STATTypes.h"
      creates:
        - struct: "FSTAT_Modifier"
        - struct: "FSTAT_ChangedPayload"
        - struct: "FSTAT_SignaturePayload"
        - struct: "FSTAT_ReadyPayload"
        - struct: "FSTAT_FlowPayload"
        - struct: "FSTAT_OverloadParameters"
        - struct: "FSTAT_OverloadPayload"
        - struct: "FSTAT_FusionState"
        - struct: "FSTAT_CounterplayWindowPayload"
        - enum: "EModifierOp"
    - path: "/Plugins/STAT/Source/STAT/Public/Interfaces/STAT_Damageable_If.h"
      creates:
        - uinterface: "ISTAT_Damageable_If"
          methods:
            - "void ApplyDamage(float BaseDamage, AActor* Instigator, const FGameplayTagContainer& AttackTags, FName CauseTag);"
    - path: "/Plugins/STAT/Source/STAT/Public/Interfaces/STAT_Upgradable_If.h"
      creates:
        - uinterface: "I_STAT_Upgradable_If"
          methods:
            - "void RequestUpgradeStat(const FGameplayTag& StatTagToUpgrade);"
    - path: "/Plugins/STAT/Source/STAT/Public/Interfaces/STAT_SignatureSupport_If.h"
      creates:
        - uinterface: "I_STAT_SignatureSupport_If"
          methods:
            - "void AddSignatureCharge(AActor* TargetActor, FGameplayTag CauseTag, float Amount = 1.0f);"
    - path: "/Plugins/STAT/Source/STAT/Public/Interfaces/STAT_Flow_If.h"
      creates:
        - uinterface: "I_STAT_Flow_If"
          methods:
            - "void AddFlowStack(float Amount, FGameplayTag SourceTag, AActor* Instigator);"
            - "void ResetFlow(FGameplayTag ReasonTag);"
    - path: "/Plugins/STAT/Source/STAT/Public/Interfaces/STAT_Overloadable_If.h"
      creates:
        - uinterface: "I_STAT_Overloadable_If"
          methods:
            - "void RequestOverload(const FSTAT_OverloadParameters& Params);"
    - path: "/Plugins/STAT/Source/STAT/Public/Interfaces/STAT_Fusion_If.h"
      creates:
        - uinterface: "I_STAT_Fusion_If"
          methods:
            - "void NotifyFusionEvent(FGameplayTag EventTag, AActor* Source);"
    - path: "/Plugins/STAT/Source/STAT/Public/USTATAbilityComponent.h"
      creates:
        - uclass: "USTATAbilityComponent"
          inherits: "UActorComponent"
          implements: ["ISTAT_Damageable_If", "I_STAT_Upgradable_If", "...etc"]
          eventDispatchers:
            - "STAT_OnStatChanged_E"
            - "STAT_OnSignatureChargeChanged_E"
            - "STAT_OnSignatureChargeReady_E"
            - "STAT_OnFlowStackChanged_E"
            - "STAT_OnOverloadStateChanged_E"
            - "STAT_OnFusionAmpChanged_E"
            - "STAT_OnCounterplayWindow_E"
          methods:
            - "void CollectDefenseRules();"
            - "void MatchAttackVsDefense();"
            - "void ComputeFinalDamage();"
            - "void ApplyDamageToTarget();"
            - "void ValidateUpgradable();"
            - "void ConsumeResource();"
            - "void ApplyUpgradeModifier();"
            - "void EnqueueSynergyRecalc();"
            - "void CalculateSynergyScore();"
            - "void ApplySynergyLevelTags();"
            - "void AddSignatureCharge();"
            - "void CheckSignatureReady();"
            - "void Flow_OnHitGain();"
            - "void Flow_OnDamageTakenReset();"
            - "void Flow_DecayTick();"
            - "void Flow_ApplyRewards();"
            - "void Overload_Start();"
            - "void Overload_Tick();"
            - "void Overload_End();"
            - "void Fusion_NotifyEvent();"
            - "void Fusion_CheckAmp();"
            - "void Counterplay_OpenWindow();"
            - "void Counterplay_Resolve();"
    - path: "/Plugins/STAT/Source/STAT/Private/USTATAbilityComponent.cpp"
  uobjects:
    - name: "USTATAbilityComponent"
      thread: "GameThread|GameThread_TimeSliced|AsyncTask"
  dependencies:
    - module: "Core"
    - module: "CoreUObject"
    - module: "Engine"
    - module: "GameplayTags"
    - module: "NetCore"
```
---
