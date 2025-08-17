### **A. 불변의 핵심 계약: 캐릭터 상태(STAT) 시스템**

#### **1. 문서의 지위 및 정책 (Document Status & Policy)**
*   **절대적 권한**: 본 문서는 `캐릭터 상태(STAT) 시스템`에 대한 최상위 기술적 기준이며, 다른 모든 산출물(설계서, 코드, 위키 등)과 내용이 충돌할 경우 본 문서가 무조건 우선한다.
*   **머지 조건 (Merge Condition)**: 본 계약의 조항을 위반하는 모든 코드 변경은 CI(지속적 통합) 과정에서 **자동으로 실패(FAIL) 처리**하는 것을 원칙으로 한다. 예외는 공식적인 아키텍처 리뷰를 통해 서면 승인을 받은 경우에만 한정된다.
*   **버전 관리 정책**: 본 계약의 내용은 아래 **'계약 개정 절차'**를 통해서만 변경 가능하며, 변경 시에는 관련된 모든 기능에 대한 전체 회귀 테스트가 반드시 수행되어야 한다.
*   **계약 개정 절차 (Architecture Change Request, ACR)**:
    1.  **발의 (Propose):** 본 계약의 변경을 원하는 개발자는 'ACR' 문서를 작성하여, ①변경의 필요성, ②기술적 근거, ③예상 파급 효과(영향받는 모든 모듈 목록), ④상세한 마이그레이션 계획을 기술한다.
    2.  **리뷰 (Review):** 아키텍트 그룹(또는 지정된 리뷰어)이 ACR을 공식적으로 검토하고 승인 여부를 결정한다.
    3.  **실행 (Execute):** 최종 승인 시, 본 계약의 버전은 내부 메타데이터로 상향 조정되며, 관련된 모든 `C(품질 게이트)`와 `F(Job Order)` 템플릿도 함께 개정된다.

#### **2. 아키텍처 원칙 및 강제 규약 (Architectural Principles & Mandatory Regulations)**
*   **플러그인 기반 모듈화 (Source: A1):** STAT 시스템은 외부 C++ 의존성이 없는 독립적인 Game Feature Plugin으로 존재해야 한다. 다른 플러그인의 헤더 파일을 `#include`하는 행위는 엄격히 금지된다.
*   **상호작용 및 데이터 흐름 (Source: A1, G00_1):** 모든 외부 시스템(ITEM, SKIL 등)과의 상호작용은 본 문서 `3. 핵심 시그니처`에 정의된 C++ 인터페이스(`UInterface`)를 통해서만 이루어져야 한다. 이벤트 전파는 `BP_InteractionHub`를 통해 중계되는 `Event Dispatcher`를 사용한다.
*   **비동기 처리 (Source: Z1):** 다수의 액터에게 영향을 주거나 복잡한 계산(예: 동적 세트 시너지 재계산)이 필요한 모든 로직은 반드시 `UE::Tasks`와 `FTimeSlicedTask`를 사용하여 비동기 및 프레임 분산 처리해야 한다.
*   **네이밍 및 골격 (Source: Z2):** 모든 C++ 클래스, 구조체, 열거형은 `USTAT_`, `FSTAT_`, `ESTAT_` 접두사를 사용해야 한다. 게임플레이 태그는 `Stat.*` 접두사를 사용해야 한다.
*   **성능 및 안전성 (Source: Z1):** 프레임 분산 처리(Time-Slicing) 로직은 단일 스텝(Step) 당 **1ms**를 초과할 수 없으며, 모든 비동기 콜백은 `SAFE_WEAK_LAMBDA` 매크로를 사용하여 약한 참조(`Weak Ptr`)를 사용해야 한다.
*   **보안 및 검증 (Source: A1):** 모든 포인터 접근 전 `IsValid()` 또는 `nullptr` 검사를 의무화하여 크래시를 원천 차단한다. 개발 중 논리 오류는 `check`, `ensure` 매크로를 통해 조기에 발견한다.

#### **3. 핵심 시그니처 (Immutable Signatures)**
*   **핵심 인터페이스 (Core Interfaces):**
    ```cpp
    // 피해를 받을 수 있는 모든 액터가 구현해야 하는 인터페이스
    UINTERFACE(MinimalAPI, Blueprintable)
    class USTAT_Damageable_If : public UInterface {};

    class ISTAT_Damageable_If
    {
        GENERATED_BODY()
    public:
        UFUNCTION(BlueprintNativeEvent, Category = "STAT|Damageable")
        void ApplyDamage(float BaseDamage, AActor* Instigator, const FGameplayTagContainer& AttackTags, FName CauseTag);
    };

    // 버프/디버프 등 스탯 Modifier를 적용받을 수 있는 액터가 구현해야 하는 인터페이스
    UINTERFACE(MinimalAPI, Blueprintable)
    class USTAT_Buffable_If : public UInterface {};

    class ISTAT_Buffable_If
    {
        GENERATED_BODY()
    public:
        UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "STAT|Buffable")
        void ApplyStatModifier(const FSTAT_Modifier& Modifier);

        UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "STAT|Buffable")
        void RemoveStatModifier(const FGameplayTag& SourceTag);
    };
    
    // 스탯 비교 미리보기 기능을 지원하기 위한 인터페이스
    UINTERFACE(MinimalAPI, Blueprintable)
    class USTAT_Comparable_If : public UInterface {};

    class ISTAT_Comparable_If
    {
        GENERATED_BODY()
    public:
        // 가상 Modifier 목록을 기반으로 현재 스탯과의 차이를 계산하여 반환
        UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "STAT|Comparable")
        TMap<FGameplayTag, float> GetStatDeltaComparedTo(const TArray<FSTAT_Modifier>& ModifiersToCompare);
    };
    ```

*   **핵심 데이터 구조 (Core Data Structures):**
    ```cpp
    // 모든 스탯 변경의 기본 단위
    USTRUCT(BlueprintType)
    struct FSTAT_Modifier
    {
        GENERATED_BODY()
        // ... (상세 내용은 B.STAT_Schema.md에서 정의)
    };

    // 스탯 변경 시 외부로 전파될 이벤트 정보 구조체
    USTRUCT(BlueprintType)
    struct FSTAT_ChangedPayload
    {
        GENERATED_BODY()
        // ... (상세 내용은 B.STAT_Schema.md에서 정의)
    };
    ```
