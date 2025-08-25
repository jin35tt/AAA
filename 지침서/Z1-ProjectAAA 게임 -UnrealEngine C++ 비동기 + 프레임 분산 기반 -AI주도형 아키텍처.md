
## 📘 UnrealEngine C++ 비동기 + 프레임 분산 기반 AI주도형 아키텍처 (Final v3.2)

> **버전:** 3.2
> **작성일:** 2025-08-24
> **UE 버전:** 5.6.0
> **대상:** **AI 시스템 아키텍트 (주요 사용자)**, 인간 개발자
>
> **문서 구조:**
> 이 문서는 두 개의 핵심 파트와 하나의 부록으로 구성됩니다.
> - **Part A: 불변의 코어 아키텍처:** 기술적 '헌법'. 모든 C++ 코드에 적용되는 핵심 원칙과 패턴.
> - **Part B: AI 주도형 개발 워크플로우:** '응용 가이드'. Part A 기반 위에서 AI가 설계를 주도하는 절차.
> - **Appendix A: 참조 구현:** '구현 표준'. 해석의 모호함을 없애기 위한 실제 코드 예시.

---
---

## Part A: 불변의 코어 아키텍처 (The Immutable Core Architecture)

**이 파트는 본 아키텍처의 기술적 심장부이며, 모든 판단과 구현의 최종 기준이 된다.**
**<FeatureName>.MDC.md 형식을 모든 'Markdown Feature Contract' 파일의 공식 표준으로 채택합니다.**
---

### 1장: 핵심 철학 (Core Philosophy)
*   **"모든 프레임은 예산을 갖는다 (Every Frame has a Budget)"**: 모든 작업은 프레임 예산(예: 16.6ms)을 존중하여 분할 실행되어야 한다.
*   **"시간으로부터 분리하라 (Decouple from Time)"**: `Tick` 의존성을 제거하고, 이벤트 기반의 비동기 Task로 로직을 구동한다.
*   **"게임 스레드는 신성하다 (The Game Thread is Sacred)"**: 게임 스레드는 결과 적용만을 수행하고, 모든 무거운 작업은 백그라운드로 이전한다.
*   **"예측 가능성 > 순간 속도 (Predictability over Peak Speed)"**: 일관된 성능 유지를 최우선으로 하여, 최악의 상황에서도 안정성을 보장한다.

---

### 2장: 아키텍처의 구성 요소 (The Building Blocks)
| 컴포넌트 | 단일 책임 (Single Responsibility) |
| :--- | :--- |
| **`FGameTaskManager`** | **중앙 관제소:** 모든 비동기 작업의 생성, 스케줄링, 실행을 총괄한다. |
| **`FPriorityTaskQueue`** | **자원 분배기:** 우선순위에 따라 프레임 예산을 할당하여 작업 기아(Starvation)를 방지한다. |
| **`FTimeSlicedTask`** | **안전 분할기:** 게임 스레드 작업을 1ms 미만의 작은 조각으로 분할한다. |
| **`TFuture<T>`** | **결과 전달자:** 비동기 작업의 결과를 다음 작업으로 안전하게 전달하는 표준 약속(Promise)이다. |
| **`FBatchProcessor`** | **고성능 집진기:** 한 프레임의 수많은 동종 요청을 모아 단일 작업으로 처리한다. |

---

### 3장: 핵심 구현 패턴 (The Core Implementation Patterns)
*   **패턴 1: 순차적 의존성 (Chain of Command)**: `TFuture`와 `Then`을 사용해 A→B→C 순서의 작업을 명확하게 연결한다. (**Appendix A.1.1 참조**)
*   **패턴 2: 대규모 병렬 처리 (Assembly Line)**: `FBatchProcessor`를 사용해 수백 개의 동종 요청을 단일 Task로 묶어 처리한다. (**Appendix A.1.2 참조**)
*   **패턴 3: 안전한 실패 및 취소 (Safe Failure & Cancellation)**: `FCancellationToken`과 `TFuture`의 유효성 검사를 통해 오류와 취소를 안전하게 처리한다.

---

### 4장: 절대 준수 규칙 (Non-Negotiable Rules)
**AI가 생성하는 모든 C++ 코드에 예외 없이 적용되는 최종 검증 항목이다.**
| # | 규칙 | 필수 매크로 / 툴 |
| :--- | :--- | :--- |
| **1** | **UObject 접근은 항상 GameThread** | `CHECK_GAME_THREAD()` |
| **2** | **비동기 콜백에는 반드시 `TWeakObjectPtr` 사용** | `SAFE_WEAK_LAMBDA(this)` |
| **3**| **`FTimeSlicedTask`의 StepFunction은 1ms를 초과 금지** | `GENERATE_TIME_SLICE_TASK()` |
| **4**| **모든 장기 실행 Task는 Timeout & Cancel 지원** | `FCancellationToken` 전파 |
| **5**| **모든 Task Scope에 프로파일링 태그 삽입** | `TRACE_CPUPROFILER_EVENT_SCOPE_TEXT(*TaskName)` |

---
---

## Part B: AI 주도형 개발 워크플로우 (The AI-Led Development Workflow)

**이 파트는 AI 아키텍트가 Part A의 견고한 기반 위에서 시스템 설계를 주도하는 구체적인 '실행 절차'를 정의한다.**

---

### 5장: AI 주도 패러다임
AI는 C++ 코드를 직접 작성하기 전에, 시스템의 **설계 의도와 기계가 읽을 수 있는 명세**를 담은 **'Markdown Feature Contract (`.mdc`)'** 를 먼저 설계한다. 이는 AI의 역할을 단순 구현자에서 시스템 아키텍트로 격상시키는 핵심적인 패러다임 전환이다.

### 6장: Markdown Feature Contract (`.mdc`) 명세
`.mdc` 파일은 **Part A의 패턴들을 선언적으로 기술**하는 AI의 핵심 산출물이다. 헤더(Markdown)와 계약 블록(YAML)으로 구성된다. (**Appendix A.2.1의 `BlinkDash.skill.MDC.md` 참조**)

### 7장: 5단계 개발 워크플로우 (The 5-Step Workflow)
**AI는 다음 5단계를 알고리즘처럼 순서대로 수행해야 한다.**
1.  **[설계]** `.mdc` 파일 작성
2.  **[제출]** `.mdc` 텍스트 출력
3.  **[코드 생성]** (자동화 시스템) `.mdc`로부터 C++ 뼈대 생성
4.  **[수신]** 생성된 C++ 템플릿 입력 받기
5.  **[구현]** 템플릿의 `// TODO:` 섹션에 핵심 로직 작성 (Part A 규칙 준수)

### 8장: 시스템 모듈 및 적용 가이드
| 모듈/상황 | **적용 아키텍처/패턴** | **AI의 핵심 역할** |
| :--- | :--- | :--- |
| **모든 신규 게임플레이 기능** | **`.mdc` 기반 AI 주도 워크플로우** | **`.mdc` 설계 및 최종 로직 구현** |
| **고성능 코어 시스템** | **Part A의 `FBatchProcessor` 패턴** | 패턴을 직접 활용하여 C++ 코드 구현 |
| **기존 시스템 유지보수** | **Part A의 핵심 패턴 및 규칙** | 기존 코드를 분석하고 Part A의 원칙에 따라 리팩토링 |

---
---

## Appendix A: 참조 구현 (Reference Implementations)

**이 부록은 해석의 모호함을 제거하고 모든 개발자가 따라야 할 '구현 표준'을 제시한다.**

---

### A.1: 코어 아키텍처 패턴 구현 예시

#### **A.1.1: 패턴 1 - 순차적 의존성 (Chain of Command)**
*   **시나리오:** 서버에서 플레이어 데이터를 비동기로 받고, 파싱한 후, UI를 업데이트한다.

```cpp
// UPlayerProfileUI.cpp
void UPlayerProfileUI::FetchAndDisplayPlayerProfile(int32 PlayerID)
{
    SAFE_WEAK_LAMBDA(this);

    GameTaskManager::Get().BuildTask()
        .WithName(TEXT("DisplayPlayerProfile"))
        .InThreadPool([PlayerID]() -> TFuture<FPlayerData> {
            // 1. (비동기) 서버에서 데이터를 요청하는 무거운 작업
            return UMyHttpService::Get().FetchPlayerDataFromServer_Async(PlayerID);
        })
        .ThenInThreadPool([](FPlayerData RawData) -> TFuture<FPlayerProfile> {
            // 2. (비동기) 받은 데이터를 파싱하는 CPU 집약적 작업
            return UPlayerDataParser::Get().ParseProfile_Async(RawData);
        })
        .ThenInGameThread([WeakPtr_this](FPlayerProfile Profile) {
            // 3. (게임 스레드) 최종 결과를 UI에 적용
            if (WeakPtr_this.IsValid())
            {
                WeakPtr_this->PlayerNameText->SetText(FText::FromString(Profile.Name));
                WeakPtr_this->PlayerLevelText->SetText(FText::AsNumber(Profile.Level));
            }
        });
}
```

#### **A.1.2: 패턴 2 - 대규모 병렬 처리 (Assembly Line)**
*   **시나리오:** `UDamageSystem`이 한 프레임에 발생한 여러 데미지 요청을 일괄 처리한다.

```cpp
// UDamageSystem.h
struct FDamageRequest { TWeakObjectPtr<AActor> Target; float Amount; };

// UDamageSystem.cpp
void UDamageSystem::RequestDamage(AActor* Target, float Amount)
{
    // 개별 Task를 생성하지 않고, Batch Processor에 데이터만 추가
    GameTaskManager::Get().GetBatchProcessor<FDamageRequest>(TEXT("DamageBatch"))
        ->Add({Target, Amount});
}

// GameTaskManager.cpp의 Tick 등에서 매 프레임 호출
void ProcessDamageBatch()
{
    auto DamageProcessor = GameTaskManager::Get().GetBatchProcessor<FDamageRequest>(TEXT("DamageBatch"));
    if (DamageProcessor->HasPendingData())
    {
        // 버퍼에 쌓인 데이터가 있으면 단일 Time-Sliced Task로 변환하여 큐에 삽입
        DamageProcessor->ProcessAsTimeSlicedTask(
            [](TArray<FDamageRequest> BatchedRequests) -> FTimeSlicedTask {
                int32 Index = 0;
                return FTimeSlicedTask(TEXT("BatchedDamageProcessing"), 
                    [BatchedRequests, Index]() mutable -> bool {
                        // StepFunction: 매 Tick마다 N개씩 처리
                        const int32 ProcessPerTick = 5;
                        for (int32 i = 0; i < ProcessPerTick && Index < BatchedRequests.Num(); ++i, ++Index)
                        {
                            const auto& Req = BatchedRequests[Index];
                            if (Req.Target.IsValid())
                            {
                                UGameplayStatics::ApplyDamage(Req.Target.Get(), Req.Amount, ...);
                            }
                        }
                        return Index < BatchedRequests.Num(); // true면 다음 프레임에 계속
                    });
            });
    }
}
```

---

### A.2: AI 주도 워크플로우 End-to-End 예시: "Blink Dash" 스킬

#### **A.2.1: [1단계 결과물] `BlinkDash.skill.MDC.md` 전문**
```markdown
---
### **[SKILL] Blink Dash Feature Contract**
---
**상태:** Approved
**담당 아키텍트:** Meta Architect (AI)
#### **개요**
플레이어가 바라보는 방향으로 일정 거리를 빠르게 순간 이동하는 스킬.
#### **설계 의도**
- 이동 경로에 장애물이 있는지 비동기적으로 확인하여, 월드가 복잡해도 게임 스레드 부하를 최소화.
- 이동 시작과 끝에 VFX를 재생하여 시각적 피드백을 강화.
```yaml
featureName: Skill_BlinkDash
description: "바라보는 방향으로 빠르게 순간 이동하는 스킬"
inputs:
  - name: Caster (Actor)
  - name: DashDistance (float)
pipeline:
  - step: 1_CalculateTargetPoint
    description: "캐스터의 전방으로 목표 지점을 계산"
    type: synchronous
    inputs: [Caster, DashDistance]
    outputs: [TargetPoint (Vector)]
  - step: 2_CheckCollision_Async
    description: "시작점에서 목표 지점까지 비동기 라인 트레이스로 충돌 확인"
    type: async_thread_pool
    inputs: [Caster, TargetPoint]
    outputs: [FinalDestination (Vector)]
  - step: 3_PerformDash_OnGameThread
    description: "계산된 최종 목적지로 캐스터를 이동시키고 VFX 재생"
    type: gamethread
    inputs: [Caster, FinalDestination]
outputs:
  - event: OnDashComplete
  - event: OnDashBlocked
```

#### **A.2.2: [3단계 결과물] 자동 생성된 코드 뼈대**
```cpp
// USkill_BlinkDash.h (자동 생성된 템플릿)
#pragma once
#include "CoreMinimal.h"
#include "BaseSkill.h"
#include "USkill_BlinkDash.generated.h"

UCLASS()
class USkill_BlinkDash : public UBaseSkill
{
    GENERATED_BODY()
public:
    // Contract Steps
    FVector Execute_1_CalculateTargetPoint(AActor* Caster, float DashDistance);
    TFuture<FVector> Execute_2_CheckCollision_Async(AActor* Caster, FVector TargetPoint);
    void Execute_3_PerformDash_OnGameThread(AActor* Caster, FVector FinalDestination);
};
```

#### **A.2.3: [5단계 결과물] 최종 구현된 C++ 코드**
```cpp
// USkill_BlinkDash.cpp (AI가 최종 구현한 모습)
#include "USkill_BlinkDash.h"
#include "GameFramework/Actor.h"
#include "Kismet/KismetSystemLibrary.h"

// 1. 목표 지점 계산
FVector USkill_BlinkDash::Execute_1_CalculateTargetPoint(AActor* Caster, float DashDistance)
{
    if (!Caster) return FVector::ZeroVector;
    return Caster->GetActorLocation() + Caster->GetActorForwardVector() * DashDistance;
}

// 2. 비동기 충돌 확인
TFuture<FVector> USkill_BlinkDash::Execute_2_CheckCollision_Async(AActor* Caster, FVector TargetPoint)
{
    const FVector StartPoint = Caster->GetActorLocation();
    
    // Launch a task on a background thread
    return UE::Tasks::Launch(UE_SOURCE_LOCATION, [StartPoint, TargetPoint]()
    {
        FHitResult HitResult;
        // In a real project, this would be a thread-safe query to the physics scene.
        // For this example, we simulate a simple line trace.
        bool bHit = UKismetSystemLibrary::LineTraceSingle(
            /*WorldContext*/ nullptr, StartPoint, TargetPoint, 
            UEngineTypes::ConvertToTraceType(ECC_Visibility), false, {}, 
            EDrawDebugTrace::None, HitResult, true);

        if (bHit)
        {
            return HitResult.Location; // 충돌 지점 반환
        }
        return TargetPoint; // 충돌 없으면 목표 지점 반환
    });
}

// 3. 게임 스레드에서 실제 이동 수행
void USkill_BlinkDash::Execute_3_PerformDash_OnGameThread(AActor* Caster, FVector FinalDestination)
{
    if (Caster)
    {
        Caster->SetActorLocation(FinalDestination, false, nullptr, ETeleportType::TeleportPhysics);
        // Play VFX at start and end points...
        // OnDashComplete.Broadcast();
    }
}
```
---
## Appendix A: 참조 구현 (Reference Implementations)

**이 부록은 `Z2` 아키텍처의 부록 A에 정의된 '표준 .MDC.md 템플릿'을 기반으로, AI가 C++ 코드를 어떻게 완성해야 하는지 보여주는 구체적인 예시이다.**

#### **A.1: 코어 아키텍처 패턴 구현 예시**
**... (이전 v3.2 내용과 동일, 구체적인 C++ 패턴 예시 제공) ...**

#### **A.2: AI 주도 워크플로우 End-to-End 예시**

이 예시는 AI가 **`Z2` 문서의 표준 템플릿**을 사용하여 어떻게 완전한 기능을 구현하는지 보여준다.

1.  **[1단계 결과물] 표준 템플릿을 따른 `.MDC.md` 파일:**
    *   AI는 `Z2` 문서의 부록 A에 명시된 **`ZG01 -기본템플릿.MDC.md`** 와 동일한 형식으로 기능 설계를 완료한다.

2.  **[3단계 결과물] `.mdc`로부터 자동 생성된 코드 뼈대:**
    *   코드 생성기는 `.mdc`의 `pipeline` 섹션을 기반으로 다음과 같은 C++ 뼈대를 생성한다.
    ```cpp
    // USTATStaminaSystem.h (자동 생성된 템플릿)
    #pragma once
    // ... includes ...
    UCLASS()
    class USTATStaminaSystem : public UObject
    {
        GENERATED_BODY()
    public:
        // Pipeline Steps
        void EnqueueConsumeRequests(const FConsumeStaminaRequested& Payload);
        void ApplyBatchedConsume();
        // ... EnqueueRestoreRequests, ApplyBatchedRestore ...
    };
    ```

3.  **[5단계 결과물] AI가 최종 구현한 C++ 코드:**
    *   AI는 `Z1`의 핵심 구성요소(`FBatchProcessor` 등)를 사용하여 자동 생성된 함수의 내부 로직을 완성한다.
    ```cpp
    // USTATStaminaSystem.cpp (AI가 최종 구현한 모습)
    void USTATStaminaSystem::EnqueueConsumeRequests(const FConsumeStaminaRequested& Payload)
    {
        // .mdc의 'batch' 명세에 따라 BatchProcessor에 데이터 추가
        GameTaskManager::Get().GetBatchProcessor<...>("ConsumeStamina")->Add(...);
    }
    
    void USTATStaminaSystem::ApplyBatchedConsume()
    {
        // .mdc의 'where: GameThread_TimeSliced' 명세에 따라 Time-Sliced Task 생성
        // .mdc의 'effects' 명세에 따라 Stamina를 변경하고 이벤트를 발생시키는 로직 구현
    }
    // ...
    ```
---