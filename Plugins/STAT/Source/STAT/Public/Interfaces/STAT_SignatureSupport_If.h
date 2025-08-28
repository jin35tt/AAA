#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "STAT_SignatureSupport_If.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USTAT_SignatureSupport_If : public UInterface
{
    GENERATED_BODY()
};

class I_STAT_SignatureSupport_If
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Signature")
    void AddSignatureCharge(AActor* TargetActor, FGameplayTag CauseTag, float Amount = 1.0f);
};

