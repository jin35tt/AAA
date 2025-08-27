#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "FSTAT_Structures.h"
#include "ISTAT_Comparable_If.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USTAT_Comparable_If : public UInterface
{
    GENERATED_BODY()
};

class ISTAT_Comparable_If
{
    GENERATED_BODY()
public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Comparable")
    TMap<FGameplayTag, float> GetStatDeltaComparedTo(const TArray<FSTAT_Modifier>& ModifiersToCompare);
};
