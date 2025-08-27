#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Data/FSTAT_Structures.h"
#include "GameplayTagContainer.h"
#include "STAT_Tags.h"
#include "USTATAbilityComponent.generated.h"

/**
 * Component managing stat modifiers and broadcasting stat changes.
 */
UCLASS(ClassGroup=(STAT), meta=(BlueprintSpawnableComponent))
class USTATAbilityComponent : public UActorComponent
{
    GENERATED_BODY()
public:
    /** Apply a stat modifier to the owning actor. */
    void ApplyModifier(const FSTAT_Modifier& Modifier);

    /** Remove an existing modifier by its source tag. */
    void RemoveModifierByTag(const FGameplayTag& SourceTag);

    /** Add momentum flow stacks per Mp00 core fun factor. */
    UFUNCTION(BlueprintCallable, Category="STAT|FunFactor")
    void AddFlow(float Amount);

    /** Increase signature charge used for powerful abilities. */
    UFUNCTION(BlueprintCallable, Category="STAT|FunFactor")
    void AddSignatureCharge(float Amount);

    /** Delegate fired when a stat value changes. */
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChanged, const FSTAT_ChangedPayload&);
    FOnStatChanged OnStatChanged;

private:
    /** Current stat values keyed by gameplay tag. */
    TMap<FGameplayTag, float> StatValues;
};
