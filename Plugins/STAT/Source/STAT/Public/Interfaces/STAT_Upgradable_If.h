#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "STAT_Upgradable_If.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USTAT_Upgradable_If : public UInterface
{
    GENERATED_BODY()
};

class I_STAT_Upgradable_If
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Upgrade")
    void RequestUpgradeStat(const FGameplayTag& StatTagToUpgrade);
};

