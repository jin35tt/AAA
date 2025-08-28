#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "STAT_Damageable_If.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USTAT_Damageable_If : public UInterface
{
    GENERATED_BODY()
};

class ISTAT_Damageable_If
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Damage")
    void ApplyDamage(float BaseDamage, AActor* Instigator, const FGameplayTagContainer& AttackTags, FName CauseTag);
};

