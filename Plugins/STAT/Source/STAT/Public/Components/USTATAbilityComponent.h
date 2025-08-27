#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "FSTAT_Structures.h"
#include "GameplayTagContainer.h"
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

    /** Delegate fired when a stat value changes. */
    DECLARE_MULTICAST_DELEGATE_OneParam(FOnStatChanged, const FSTAT_ChangedPayload&);
    FOnStatChanged OnStatChanged;
};
