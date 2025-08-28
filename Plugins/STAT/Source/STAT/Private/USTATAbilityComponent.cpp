#include "USTATAbilityComponent.h"
#include "Interfaces/STAT_DefenseProvider_If.h"
#include "ProfilingDebugging/CpuProfilerTrace.h"
#include "Async/Async.h"
#include "Async/TaskGraphInterfaces.h"

USTATAbilityComponent::USTATAbilityComponent()
{
    // TODO: Initialize component
}

void USTATAbilityComponent::ApplyDamage_Implementation(float BaseDamage, AActor* Instigator, const FGameplayTagContainer& AttackTags, FName CauseTag)
{
    // TODO: Apply damage logic
}

void USTATAbilityComponent::RequestUpgradeStat_Implementation(const FGameplayTag& StatTagToUpgrade)
{
    // TODO: Upgrade stat logic
}

void USTATAbilityComponent::AddSignatureCharge_Implementation(AActor* TargetActor, FGameplayTag CauseTag, float Amount)
{
    // TODO: Signature charge logic
}

void USTATAbilityComponent::AddFlowStack_Implementation(float Amount, FGameplayTag SourceTag, AActor* Instigator)
{
    // TODO: Flow stack logic
}

void USTATAbilityComponent::ResetFlow_Implementation(FGameplayTag ReasonTag)
{
    // TODO: Reset flow logic
}

void USTATAbilityComponent::RequestOverload_Implementation(const FSTAT_OverloadParameters& Params)
{
    // TODO: Overload logic
}

void USTATAbilityComponent::NotifyFusionEvent_Implementation(FGameplayTag EventTag, AActor* Source)
{
    // TODO: Fusion event logic
}

FGameplayTagContainer USTATAbilityComponent::CollectDefenseRules()
{
    TRACE_CPUPROFILER_EVENT_SCOPE(USTATAbilityComponent::CollectDefenseRules);

    FGameplayTagContainer CombinedIgnoreTags;

    AActor* OwnerActor = GetOwner();
    if (!IsValid(OwnerActor))
    {
        return CombinedIgnoreTags;
    }

    TArray<UActorComponent*> DefenseProviders;
    OwnerActor->GetComponentsByInterface(USTAT_DefenseProvider_If::StaticClass(), DefenseProviders);

    for (UActorComponent* Component : DefenseProviders)
    {
        if (!IsValid(Component))
        {
            continue;
        }

        const FGameplayTagContainer Tags = I_STAT_DefenseProvider_If::Execute_GetIgnoreAttackTags(Component);
        CombinedIgnoreTags.AppendTags(Tags);
    }

    return CombinedIgnoreTags;
}

TFuture<bool> USTATAbilityComponent::MatchAttackVsDefense(const FGameplayTagContainer& AttackTags, const FGameplayTagContainer& IgnoreAttackTags)
{
    return UE::Tasks::Launch(UE_SOURCE_LOCATION, [AttackTags, IgnoreAttackTags]()
    {
        TRACE_CPUPROFILER_EVENT_SCOPE(MatchAttackVsDefense_AsyncTask);
        return IgnoreAttackTags.HasAny(AttackTags);
    });
}

TFuture<float> USTATAbilityComponent::ComputeFinalDamage(float BaseDamage, const FGameplayTagContainer& AttackTags, const TMap<FGameplayTag, float>& DefenderStats)
{
    return UE::Tasks::Launch(UE_SOURCE_LOCATION, [BaseDamage, AttackTags, DefenderStats]()
    {
        TRACE_CPUPROFILER_EVENT_SCOPE(ComputeFinalDamage_AsyncTask);
        float FinalDamage = BaseDamage;

        TArray<FGameplayTag> Tags;
        AttackTags.GetGameplayTagArray(Tags);

        for (const FGameplayTag& Tag : Tags)
        {
            FString TagString = Tag.ToString();
            const FString Prefix = TEXT("Damage.Type.");
            if (TagString.StartsWith(Prefix))
            {
                TagString.ReplaceInline(*Prefix, TEXT("Stat.Resistance."));
                const FGameplayTag ResistanceTag = FGameplayTag::RequestGameplayTag(*TagString);
                const float ResistanceValue = DefenderStats.FindRef(ResistanceTag);
                FinalDamage *= (1.0f - ResistanceValue);
            }
        }

        return FinalDamage;
    });
}

void USTATAbilityComponent::ApplyDamageToTarget(float FinalDamage, AActor* Instigator, const FGameplayTag& SourceTag, UObject* SourceObject)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(ApplyDamageToTarget_GameThread);

    static const FGameplayTag TAG_Stat_Core_HP_Current = FGameplayTag::RequestGameplayTag(TEXT("Stat.Core.HP.Current"));
    static const FGameplayTag TAG_Stat_Core_HP_Max = FGameplayTag::RequestGameplayTag(TEXT("Stat.Core.HP.Max"));

    const float PreviousHP = StatMap.FindRef(TAG_Stat_Core_HP_Current);
    const float MaxHP = StatMap.FindRef(TAG_Stat_Core_HP_Max);
    const float NewHP = FMath::Max(0.0f, PreviousHP - FinalDamage);
    StatMap.Add(TAG_Stat_Core_HP_Current, NewHP);

    FSTAT_ChangedPayload Payload;
    Payload.StatTag = TAG_Stat_Core_HP_Current;
    Payload.PreviousValue = PreviousHP;
    Payload.CurrentValue = NewHP;
    Payload.Delta = NewHP - PreviousHP;
    Payload.MaxValue = MaxHP;
    Payload.Instigator = Instigator;
    Payload.SourceTag = SourceTag;
    Payload.SourceObject = SourceObject;

    STAT_OnStatChanged_E.Broadcast(Payload);
}

bool USTATAbilityComponent::ValidateUpgradable(const FGameplayTag& StatTagToUpgrade)
{
    TRACE_CPUPROFILER_EVENT_SCOPE(ValidateUpgradable_GameThread);

    if (!StatDefinitionTable)
    {
        ensureMsgf(false, TEXT("StatDefinitionTable is null"));
        return false;
    }

    const FSTAT_StatDefinitionRow* Row = StatDefinitionTable->FindRow<FSTAT_StatDefinitionRow>(
        StatTagToUpgrade.GetTagName(), TEXT("ValidateUpgradable"));
    if (!Row)
    {
        ensureMsgf(false, TEXT("Stat definition not found for tag %s"), *StatTagToUpgrade.ToString());
        return false;
    }

    if (!Row->CanBeUpgraded)
    {
        return false;
    }

    const int32 CurrentUpgradeLevel = UpgradeLevelMap.FindRef(StatTagToUpgrade);
    const float RequiredPoints = Row->RequiredPointCurve
        ? Row->RequiredPointCurve->GetFloatValue(CurrentUpgradeLevel + 1)
        : 0.0f;

    const float CurrentPoints = CurrentUpgradePoints;
    if (CurrentPoints < RequiredPoints)
    {
        return false;
    }

    return true;
}

void USTATAbilityComponent::ConsumeResource()
{
    // TODO: Consume resource
}

void USTATAbilityComponent::ApplyUpgradeModifier()
{
    // TODO: Apply upgrade modifier
}

void USTATAbilityComponent::EnqueueSynergyRecalc()
{
    // TODO: Enqueue synergy recalculation
}

void USTATAbilityComponent::CalculateSynergyScore()
{
    // TODO: Calculate synergy score
}

void USTATAbilityComponent::ApplySynergyLevelTags()
{
    // TODO: Apply synergy level tags
}

void USTATAbilityComponent::CheckSignatureReady()
{
    // TODO: Check signature ready
}

void USTATAbilityComponent::Flow_OnHitGain()
{
    // TODO: Flow on hit gain
}

void USTATAbilityComponent::Flow_OnDamageTakenReset()
{
    // TODO: Flow on damage taken reset
}

void USTATAbilityComponent::Flow_DecayTick()
{
    // TODO: Flow decay tick
}

void USTATAbilityComponent::Flow_ApplyRewards()
{
    // TODO: Flow apply rewards
}

void USTATAbilityComponent::Overload_Start()
{
    // TODO: Overload start
}

void USTATAbilityComponent::Overload_Tick()
{
    // TODO: Overload tick
}

void USTATAbilityComponent::Overload_End()
{
    // TODO: Overload end
}

void USTATAbilityComponent::Fusion_CheckAmp()
{
    // TODO: Fusion check amp
}

void USTATAbilityComponent::Counterplay_OpenWindow()
{
    // TODO: Counterplay open window
}

void USTATAbilityComponent::Counterplay_Resolve()
{
    // TODO: Counterplay resolve
}

