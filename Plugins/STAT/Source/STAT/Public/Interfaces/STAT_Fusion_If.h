#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "STAT_Fusion_If.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USTAT_Fusion_If : public UInterface
{
    GENERATED_BODY()
};

class I_STAT_Fusion_If
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Fusion")
    void NotifyFusionEvent(FGameplayTag EventTag, AActor* Source);
};

