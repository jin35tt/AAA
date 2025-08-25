# Z2-ProjectAAA 게임 -UnrealEngine C++ 모듈 -AI주도형 아키텍처(Final v1.4)

> **버전:** 1.4
> **작성일:** 2025-08-24
> **대상:** **AI 시스템 아키텍트 (주요 사용자)**, 인간 개발자
>
> **목적:**
> 본 문서는 **AI가 ProjectAAA의 모듈 구조 설계를 주도**하기 위한 단일 소스 오브 트루스(Single Source of Truth)이다. AI는 `Z1` 아키텍처에 따라 기능의 **'논리'** 를 설계하고, 본 `Z2` 아키텍처에 따라 기능이 속할 **'구조(플러그인)'** 와 **'관계'** 를 설계한다.
>
> **모듈화의 최종 목표는 독립적이고 교체 가능한 `Game Feature Plugin`을 생성하는 것이다. 모든 모듈은 예외 없이 처음부터 플러그인으로 생성되어야 한다.**

---


### 1. AI 주도 모듈 아키텍처 패러다임

#### 1-1. 핵심 원칙: 설계의 통합
AI는 하나의 `Markdown Feature Contract (.mdc)` 파일을 통해 기능의 **논리적 흐름(`Z1`)**과 **구조적 위치(`Z2`)**를 함께 설계한다. `.mdc`는 이제 로직과 모듈 구조를 모두 정의하는 완전한 설계 명세서이다.
**<FeatureName>.MDC.md 형식을 모든 'Markdown Feature Contract' 파일의 공식 표준으로 채택합니다.**

#### 1-2. AI-Native 플러그인 워크플로우
AI는 다음 3단계 워크플로우를 통해 새로운 기능을 포함하는 플러그인을 설계하고 구현한다. 이 워크플로우는 **유일하고 표준화된 절차**이다.
1.  **[1단계: 통합 설계]** `pluginContract`가 포함된 `.mdc` 파일 작성.
2.  **[2단계: 원클릭 생성]** 코드 생성 스크립트를 통해 플러그인 폴더 구조 및 C++ 뼈대 자동 생성.
3.  **[3단계: 핵심 구현]** 자동 생성된 뼈대의 `// TODO:` 섹션에 핵심 로직 구현.

---

### 2. `pluginContract`: 논리와 구조를 잇는 다리

`pluginContract`는 `.mdc` 파일의 YAML 블록 최상단에 위치하며, AI가 모듈의 구조와 관계를 명세하는 핵심 영역이다.

```yaml
# Z2 아키텍처와 Z1 아키텍처를 연결하는 핵심 명세
pluginContract:
  pluginName: STAT
  version: "1.2.0"
  dependencies: [ITEM, SKIL]
  exposes:
    - type: UInterface
      name: ISTAT_StaminaOwner_If
# --- 이하 Z1에서 정의한 featureName, pipeline 등 ---
```

---

### 3. 플러그인 통신 표준
**'완전한 격리'** 원칙을 위해, 플러그인 간 통신은 다음의 우선순위와 규칙을 엄격히 따른다. 이 모든 통신 수단은 **C++ 레벨에서는 노출만 하고, 최종적인 연결은 8장 '블루프린트 통합 가이드'에 따라 블루프린트에서 수행**한다.

| 우선순위 | 기술 | 목적 | **C++ 역할 (노출)** |
| :--- | :--- | :--- | :--- |
| **1** | **`UInterface`** | **1:1 직접 상호작용** | 인터페이스 함수 정의 |
| **2** | **`Data Asset`** | **중앙 데이터 참조** | 데이터 구조 정의 |
| **3** | **`Gameplay Tag`** | **상태 및 속성 표현** | 태그 선언 및 Getter/Setter 제공 |
| **4** | **`Event Dispatcher`** | **1:N 소유자 기반 이벤트**| 이벤트 선언 및 Broadcast 함수 제공 |
| **5** | **`Message Bus`** | **N:M 전역 브로드캐스트**| 메시지 구조체 정의 및 Publish 함수 제공 |

---

### 4. 아키텍처 철학

*   **완전한 격리 (Principle of Isolation):** 다른 플러그인의 C++ 헤더/클래스 직접 참조 금지.
*   **명확한 노출 (Principle of Exposure):** C++는 알고리즘만 담당, 블루프린트에는 최소 API만 노출.
*   **블루프린트 조립 원칙 (Principle of Blueprint Assembly) (신설/강화):** C++ 플러그인은 격리된 기능 블록을 제공하고, 이들 간의 최종적인 조립과 상호작용은 **오직 블루프린트 계층에서만** 이루어져야 한다는 절대 원칙이다. 이는 `A1` 아키텍처 철학의 직접적인 계승 및 강제 사항이다.
*   **AI 우선주의 (Principle of AI-First):** 모든 아키텍처와 워크플로우는 AI의 자동화 및 설계 효율을 극대화하는 방향으로 결정된다.

---

### 5. 보안·검증 파이프라인
CI 빌드 시 **'C++ 플러그인 간 직접 참조'** 를 탐지하여 실패 처리하는 규칙을 포함한다.


---

### 6. 모듈 네이밍 규칙
`.mdc`의 `pluginContract.pluginName`에 정의된 **3-4자 대문자 약어**를 모든 식별자의 **접두어(Prefix)** 로 사용한다.

---

### 7. 블루프린트 통합 가이드 (Blueprint Integration Guide)

#### 7-1. 역할의 명확한 분리
*   **C++ 플러그인 계층 (The "Engine" Layer):** 고성능의 격리된 '엔진 부품'을 제공한다. **절대 다른 플러그인의 존재를 인지해서는 안 된다.**
*   **블루프린트 계층 (The "Assembly" Layer):** C++ 부품들을 **조립하여 실제 게임플레이 로직을 완성하는 유일한 장소**이다.

#### 7-2. End-to-End 예시: 스킬 사용 시 스태미나 소모

이 예시는 `SKIL` 플러그인과 `STAT` 플러그인이 서로를 전혀 모르는 상태에서 블루프린트를 통해 어떻게 상호작용하는지를 보여준다.

##### **[Step 1] `SKIL` 플러그인 (C++): 메시지 발행**
`SKIL` 플러그인은 스킬 사용 시, 자신의 상태 변경을 `Message Bus`에 알릴 뿐이다.

```cpp
// SKILMessage.h
USTRUCT(BlueprintType)
struct FSKIL_OnSkillUsedMessage
{
    GENERATED_BODY()
    
    UPROPERTY(BlueprintReadOnly)
    TWeakObjectPtr<AActor> Caster;

    UPROPERTY(BlueprintReadOnly)
    float StaminaCost;
};

// SKILActionComponent.cpp
void USKILActionComponent::PerformSkill()
{
    // ... 스킬 로직 수행 ...
    
    // 로직 성공 시, 메시지를 만들어 전역으로 발행한다.
    FSKIL_OnSkillUsedMessage Message;
    Message.Caster = GetOwner();
    Message.StaminaCost = CurrentSkill->StaminaCost;
    UGameplayMessageSubsystem::Get(GetWorld()).BroadcastMessage(TAG_SKIL_Event_Used, Message);
}
```

##### **[Step 2] `STAT` 플러그인 (C++): 기능 노출**
`STAT` 플러그인은 외부(블루프린트)에서 호출할 수 있는 스태미나 감소 함수를 노출할 뿐이다.

```cpp
// STATStaminaComponent.cpp
UFUNCTION(BlueprintCallable, Category = "STAT|Stamina")
void USTATStaminaComponent::STAT_ApplyStaminaCost(float Cost)
{
    CurrentStamina = FMath::Max(0.f, CurrentStamina - Cost);
    STAT_OnStaminaChanged_E.Broadcast(CurrentStamina);
}
```

##### **[Step 3] 블루프린트 계층: 두 플러그인 조립**
`BP_PlayerCharacter`와 같은 중앙 블루프린트가 두 플러그인을 '연결'하는 접착제 역할을 수행한다.

**[Blueprint Graph: `BP_PlayerCharacter`]**

```
// 1. 이벤트 그래프의 BeginPlay 또는 초기화 함수에서 메시지를 구독한다.
Event Node: BeginPlay
    -> Call Function: Get Gameplay Message Subsystem
    -> Call Function: Subscribe
        (Channel): TAG_SKIL_Event_Used
        (Delegate): Create Event (OnSkillUsed_CustomEvent)

// 2. 메시지를 수신했을 때 실행될 커스텀 이벤트를 만든다.
Custom Event: OnSkillUsed_CustomEvent
    (Output Pin: Message - FSKIL_OnSkillUsedMessage)
    -> Get Component By Class: STATStaminaComponent
    -> Call Function: STAT_ApplyStaminaCost
        (Input Pin: Cost <- Drag from Message.StaminaCost)
```
이 구조를 통해 `SKIL`과 `STAT` 플러그인은 C++ 레벨에서 완벽히 분리되어 있으며, 게임의 특정 규칙("스킬을 쓰면 스태미나가 소모된다")은 블루프린트에서 유연하게 변경될 수 있다.

---

### **Appendix A: 표준 .MDC.md 템플릿 및 해설**

**이 부록은 AI가 생성해야 할 모든 `.MDC.md` 파일의 공식적인 '골든 템플릿(Golden Template)'이다.**

#### **A.1: 템플릿 전문 (`ZG01 -기본템플릿.MDC.md`)**
```markdown
# STAT.Stamina — 스태미나 시스템 (Why)
- 목표: 모든 스태미나 소모/회복을 **비동기 + 프레임 분산** 규칙으로 처리하고, 외부 시스템과는 **간접 통신**만으로 결합한다.
- 결과: 어떤 프로젝트에서도 동일 규칙으로 플러그인화하여 안전하게 통합 가능.

```yaml
# ------ Machine-Readable Contract (What) ------
pluginContract:
  pluginName: STAT
  version: "1.2.0"
  dependencies: []
  exposes:
    - type: UInterface
      name: ISTAT_StaminaOwner_If
    - type: EventDispatcher
      name: STAT_OnStaminaChanged_E
    - type: DataAsset
      name: DA_STAT_AttributeSet
feature:
  featureName: StaminaSystem
  description: "이벤트 기반 비동기 파이프라인으로 스태미나 소모/회복을 처리..."
  tags: [Stat.Core, Stat.Resource.Stamina]
io:
  inputs:
    - name: ConsumeStaminaRequested
      kind: MessageBus
      # ...
  outputs:
    - name: STAT_OnStaminaChanged_E
      kind: EventDispatcher
      # ...
pipeline:
  - step: EnqueueConsumeRequests
    where: ThreadPool
    builder: { name: "STAT.EnqueueConsume", priority: Normal, ... }
    batch: { key: "ConsumeStamina", flush: frame_end }
  - step: ApplyBatchedConsume
    where: GameThread_TimeSliced
    time_slice_ms: 1
    guardrails: { weak_capture: true, ... }
    effects:
      - mutate: "Stamina -= Sum(Amount)"
      - emit: "STAT_OnStaminaChanged_E"
  # ... (이하 생략된 pipeline steps) ...
nonNegotiableRules:
  - AlwaysGameThreadForUObject
  - WeakLambdaForAsyncCallbacks
  - ...
components:
  use:
    - FGameTaskManager
    - FBatchProcessor
    - ...
tests:
  - id: "TST-STAT-Perf-01"
    type: perf
    # ...
```
#### **A.2: 섹션별 해설**
| 섹션 | **AI의 역할** |
| :--- | :--- |
| **Header (Markdown)** | 기능의 **설계 의도와 목표(Why)**를 서술한다. |
| **`pluginContract`** | 이 기능이 속할 **플러그인의 구조와 관계(Z2)**를 정의한다. |
| **`feature`** | 이 기능의 **고유한 이름과 설명**을 정의한다. |
| **`io`** | 이 시스템이 외부와 소통하는 **공식적인 입/출력 창구(API)**를 정의한다. |
| **`pipeline`** | 기능의 **핵심 비동기 로직(Z1)**을 단계별로 명세한다. |
| **`nonNegotiableRules`** | 생성될 코드가 준수해야 할 **Z1의 절대 규칙**을 선언한다. |
| **`components`** | 파이프라인 구현에 필요한 **Z1의 핵심 구성요소**를 명시한다. |
| **`tests`** | 이 설계가 통과해야 할 **자동화 테스트 케이스**를 정의한다. |

---

---

### 8. 블루프린트 통합 패턴 카탈로그

#### 패턴 1: 직접 상호작용 (`UInterface`)
*   **상황:** 플레이어가 문(Door)과 상호작용할 때.
*   **C++:** `Door` 액터가 `IINTERACT_Interactable_If`를 구현하고, `INTERACT_Execute` 함수를 오버라이드.
*   **Blueprint:** 플레이어가 상호작용 키를 누르면, 바라보는 액터를 가져와 `Does Implement Interface` 노드로 확인 후, `INTERACT_Execute` (Message) 노드를 호출.

#### 패턴 2: 전역 이벤트 방송 (`Message Bus`)
*   **상황:** 업적 달성, 레벨 업 등 전역적인 이벤트.
*   **C++:** 이벤트를 발생시키는 플러그인(`ACHIEVEMENT`)이 메시지 구조체(`FACH_UnlockedMessage`)를 정의하고 `BroadcastMessage` 호출.
*   **Blueprint:** UI를 담당하는 `HUD` 블루프린트가 해당 메시지를 구독(Subscribe)하여 화면에 알림을 표시.

#### 패턴 3: 소유된 상태 변경 (`Event Dispatcher`)
*   **상황:** 플레이어의 체력이 변경될 때마다 UI를 갱신.
*   **C++:** `USTATHealthComponent`가 `STAT_OnHealthChanged_E` (Multicast Delegate)를 선언하고, 체력 변경 시 `Broadcast` 호출.
*   **Blueprint:** `WBP_HealthBar` 위젯이 생성될 때, 플레이어의 `USTATHealthComponent` 참조를 얻어와 `Bind Event to STAT_OnHealthChanged_E` 노드를 사용해 자신의 업데이트 함수를 연결.

---