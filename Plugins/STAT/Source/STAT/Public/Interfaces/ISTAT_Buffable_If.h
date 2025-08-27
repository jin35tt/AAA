#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "Data/FSTAT_Structures.h"
#include "ISTAT_Buffable_If.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USTAT_Buffable_If : public UInterface
{
    GENERATED_BODY()
};

class ISTAT_Buffable_If
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Buffable")
    void ApplyStatModifier(const FSTAT_Modifier& Modifier);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Buffable")
    void RemoveStatModifier(const FGameplayTag& SourceTag);
};
