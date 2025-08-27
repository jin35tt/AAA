# Z2-ProjectAAA 게임 -UnrealEngine C++ 모듈 -AI주도형 아키텍처 (v5.0 - Final)

> **버전:** 5.0
> **작성일:** 2025-08-25
> **대상:** **AI 시스템 아키텍트 (주요 사용자)**, 인간 개발자
>
> **[문서의 위상]**
> 본 문서는 **`Z0-ProjectAAA ... MDC -AI주도형 아키텍처`** 문서의 하위 규범입니다. 본 문서의 모든 규칙은 `Z0`에 정의된 `.MDC` 파일의 **구조적(Structural)인 부분**들, 특히 `pluginContract`, `compatibility`, `interfaces`, `dataSchemas` 섹션을 작성하기 위한 세부 지침으로 기능합니다. **모든 예시와 규칙은 Z0의 메타-스키마를 최우선으로 준수합니다.**

---


### 1. 모듈 아키텍처 철학

*   **완전한 격리 (Principle of Isolation):** 다른 플러그인의 C++ 헤더/클래스를 직접 참조하는 것을 절대 금지한다.
*   **명확한 노출 (Principle of Exposure):** C++는 `.MDC`에 명시된 최소한의 API만 노출하고, 알고리즘 구현에 집중한다.
*   **블루프린트 조립 원칙 (Principle of Blueprint Assembly):** C++ 플러그인 간의 최종적인 조립과 상호작용은 **오직 블루프린트 계층에서만** 이루어져야 한다.
*   **AI 우선주의 (Principle of AI-First):** 모든 구조와 워크플로우는 AI가 `.MDC`를 통해 설계를 자동화하고 코드를 생성하는 데 최적화되어야 한다.

---

### 2. `.MDC` 구조 명세 (`pluginContract`, `compatibility` 섹션)

AI는 `.MDC`를 통해 기능의 모듈 구조와 API 호환성 버전을 명세해야 한다.

*   `pluginContract.version`: **코드/모듈 아티팩트 자체의 Semantic Version**을 의미한다. (선택적)
*   `compatibility.version`: **외부에 노출되는 API(Interfaces/Payloads)의 Semantic Version**을 의미하며, 하위 호환성이 깨지는 변경 시 이 버전을 올려야 한다. (`strict` 모드에서 필수)

```yaml
# Z0에 정의된 .MDC 내에서, Z2의 규칙을 따르는 모듈 명세 영역
pluginContract:
  pluginName: STAT # [규칙] 3-4자 대문자 약어.
  dependencies: []  # [규칙] C++ 레벨의 직접 참조는 절대 금지.
compatibility:
  version: "STAT.v1.0.0" # [규칙] API 호환성 버전은 여기에 위치.
```

---

### 3. 플러그인 간 통신 표준 (`interfaces` 섹션)
플러그인 간 통신은 `.MDC`의 `interfaces` 섹션에 명시되며, 아래의 우선순위와 규칙을 엄격히 따른다. **`kind` 필드는 반드시 `Z0`의 `typeEnums.interfaceKind`에 정의된 값 중 하나를 사용해야 한다.**

> **[A1 절대 원칙]** 시스템 간의 C++ 레벨 **함수 호출**은 오직 `kind: UInterface`를 통해서만 이루어져야 한다. `MessageBus`나 `EventDispatcher`는 데이터 브로드캐스트 및 블루프린트 조립을 위한 메커니즘이며, 직접적인 함수 호출로 간주되지 않는다.

| 우선순위 | 기술 | `.MDC` `interfaces.kind` | 목적 |
| :--- | :--- | :--- | :--- |
| **1** | **`UInterface`** | `UInterface` | **1:1 직접 함수 호출** |
| **2** | **`Data Asset`** | `DataAsset` | **중앙 데이터 참조** |
| **3** | **`Gameplay Tag`** | `GameplayTag`| **상태 및 속성 표현** |
| **4** | **`Event Dispatcher`** | `EventDispatcher` | **1:N 소유자 기반 이벤트 (BP 조립)**|
| **5** | **`Message Bus`** | `MessageBus` | **N:M 전역 브로드캐스트 (BP 조립)**|

---

### 4. `.MDC` 확장 명세 (`extensions` 섹션)
`exposes`, `builder`, `batch`, `time_slice_ms`, `guardrails` 등 `Z0`의 코어 스키마에 포함되지 않은 Z2 고유의 세부 실행 정보는, `extensions` 섹션 아래에 `x-z2` 네임스페이스를 사용하여 명시한다.

```yaml
# .MDC 파일 내의 예시
capabilities:
  usesNetworking: true

pipeline:
  - step: ApplyBatchedConsume
    where: "AsyncTask"
    authority: "ServerOnly"

extensions:
  x-z2:
    exposes:
      - type: UInterface
        name: ISTAT_StaminaOwner_If
    pipeline:
      ApplyBatchedConsume:
        builder: { name: "STAT.EnqueueConsume", priority: Normal }
        batch:   { key: "ConsumeStamina", flush: frame_end }
        time_slice_ms: 1
    components:
      use: [FGameTaskManager, FBatchProcessor]
```
---

### 5. 보안·검증 파이프라인
CI 빌드 시 **'C++ 플러그인 간 직접 참조'**를 탐지하여 빌드를 실패 처리하는 규칙을 포함한다. 이 CI 규칙은 `Z0` 메타-스키마의 `pluginContract.dependencies`가 항상 비어 있어야 한다는 규칙의 기술적 강제 수단이다.

---

### 6. 모듈 네이밍 규칙
`.MDC`의 `pluginContract.pluginName`에 정의된 **3-4자 대문자 약어**를 모든 관련 C++ 클래스, 블루프린트, 데이터 에셋의 **접두어(Prefix)** 로 사용한다. (예: `USTAT_StaminaComponent`, `DT_STAT_Attributes`)

---

### 7. 블루프린트 통합 가이드 (Blueprint Integration Guide)

#### 7-1. 역할의 명확한 분리
*   **C++ 플러그인 계층 (The "Engine" Layer):** 고성능의 격리된 '엔진 부품'을 제공한다.
*   **블루프린트 계층 (The "Assembly" Layer):** C++ 부품들을 **조립하여 실제 게임플레이 로직을 완성하는 유일한 장소**이다.

#### 7-2. End-to-End 예시: 스킬 사용 시 스태미나 소모
(기존 예시는 내용이 정확하고 유용하므로 그대로 유지합니다. 이 예시는 이제 "`.MDC`에 설계된 통신 계약이 블루프린트에서 어떻게 실현되는가"를 보여주는 훌륭한 사례가 됩니다.)

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