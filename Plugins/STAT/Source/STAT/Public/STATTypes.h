#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "STATTypes.generated.h"

class AActor;
class UObject;

UENUM(BlueprintType)
enum class EModifierOp : uint8
{
    // TODO: Define modifier operations
};

USTRUCT(BlueprintType)
struct FSTAT_Modifier
{
    GENERATED_BODY()
    // TODO: Define modifier fields
};

USTRUCT(BlueprintType)
struct FSTAT_ChangedPayload
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    FGameplayTag StatTag;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    float PreviousValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    float CurrentValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    float Delta = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    float MaxValue = 0.0f;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    TWeakObjectPtr<AActor> Instigator;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    FGameplayTag SourceTag;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    TWeakObjectPtr<UObject> SourceObject;
};

USTRUCT(BlueprintType)
struct FSTAT_SignaturePayload
{
    GENERATED_BODY()
    // TODO: Define payload fields
};

USTRUCT(BlueprintType)
struct FSTAT_ReadyPayload
{
    GENERATED_BODY()
    // TODO: Define payload fields
};

USTRUCT(BlueprintType)
struct FSTAT_FlowPayload
{
    GENERATED_BODY()
    // TODO: Define payload fields
};

USTRUCT(BlueprintType)
struct FSTAT_OverloadParameters
{
    GENERATED_BODY()
    // TODO: Define parameter fields
};

USTRUCT(BlueprintType)
struct FSTAT_OverloadPayload
{
    GENERATED_BODY()
    // TODO: Define payload fields
};

USTRUCT(BlueprintType)
struct FSTAT_FusionState
{
    GENERATED_BODY()
    // TODO: Define state fields
};

USTRUCT(BlueprintType)
struct FSTAT_CounterplayWindowPayload
{
    GENERATED_BODY()
    // TODO: Define payload fields
};

