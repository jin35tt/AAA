#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "STAT_Flow_If.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USTAT_Flow_If : public UInterface
{
    GENERATED_BODY()
};

class I_STAT_Flow_If
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Flow")
    void AddFlowStack(float Amount, FGameplayTag SourceTag, AActor* Instigator);

    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Flow")
    void ResetFlow(FGameplayTag ReasonTag);
};

