#pragma once

#include "UObject/Interface.h"
#include "GameplayTagContainer.h"
#include "STATTypes.h"
#include "STAT_Overloadable_If.generated.h"

UINTERFACE(MinimalAPI, Blueprintable)
class USTAT_Overloadable_If : public UInterface
{
    GENERATED_BODY()
};

class I_STAT_Overloadable_If
{
    GENERATED_BODY()

public:
    UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category="STAT|Overload")
    void RequestOverload(const FSTAT_OverloadParameters& Params);
};

