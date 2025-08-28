#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "STAT_DefenseProvider_If.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USTAT_DefenseProvider_If : public UInterface
{
    GENERATED_BODY()
};

class I_STAT_DefenseProvider_If
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Defense")
    FGameplayTagContainer GetIgnoreAttackTags() const;
};

