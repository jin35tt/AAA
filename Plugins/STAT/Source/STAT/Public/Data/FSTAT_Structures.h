#pragma once

#include "CoreMinimal.h"
#include "GameplayTagContainer.h"
#include "UObject/WeakObjectPtr.h"
#include "FSTAT_Structures.generated.h"

/** Operation types for stat modifiers. */
UENUM(BlueprintType)
enum class EModifierOp : uint8
{
    Add,
    Multiply
};

/** All stat changes are represented with this structure. */
USTRUCT(BlueprintType)
struct FSTAT_Modifier
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadWrite, Category="STAT")
    FGameplayTag TargetStatTag;

    UPROPERTY(BlueprintReadWrite, Category="STAT")
    EModifierOp Operation = EModifierOp::Add;

    UPROPERTY(BlueprintReadWrite, Category="STAT")
    float Value = 0.f;

    UPROPERTY(BlueprintReadWrite, Category="STAT")
    FGameplayTag SourceTag;

    UPROPERTY()
    TWeakObjectPtr<UObject> SourceObject;
};

/** Payload broadcast when a stat value changes. */
USTRUCT(BlueprintType)
struct FSTAT_ChangedPayload
{
    GENERATED_BODY()

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    FGameplayTag StatTag;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    float PreviousValue = 0.f;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    float CurrentValue = 0.f;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    float Delta = 0.f;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    float MaxValue = 0.f;

    UPROPERTY()
    TWeakObjectPtr<AActor> Instigator;

    UPROPERTY(BlueprintReadOnly, Category="STAT")
    FGameplayTag SourceTag;

    UPROPERTY()
    TWeakObjectPtr<UObject> SourceObject;
};
