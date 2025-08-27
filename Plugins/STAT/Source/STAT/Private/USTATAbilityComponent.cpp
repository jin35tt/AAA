#include "Components/USTATAbilityComponent.h"

void USTATAbilityComponent::ApplyModifier(const FSTAT_Modifier& Modifier)
{
    float* CurrentValue = StatValues.Find(Modifier.TargetStatTag);
    if (!CurrentValue)
    {
        StatValues.Add(Modifier.TargetStatTag, 0.f);
        CurrentValue = StatValues.Find(Modifier.TargetStatTag);
    }

    const float Previous = *CurrentValue;
    switch (Modifier.Operation)
    {
    case EModifierOp::Add:
        *CurrentValue += Modifier.Value;
        break;
    case EModifierOp::Multiply:
        *CurrentValue *= Modifier.Value;
        break;
    default:
        break;
    }

    FSTAT_ChangedPayload Payload;
    Payload.StatTag = Modifier.TargetStatTag;
    Payload.PreviousValue = Previous;
    Payload.CurrentValue = *CurrentValue;
    Payload.Delta = *CurrentValue - Previous;
    Payload.SourceTag = Modifier.SourceTag;
    Payload.SourceObject = Modifier.SourceObject;
    OnStatChanged.Broadcast(Payload);
}

void USTATAbilityComponent::RemoveModifierByTag(const FGameplayTag& SourceTag)
{
    // TODO: Implement modifier removal logic
}

void USTATAbilityComponent::AddFlow(float Amount)
{
    FSTAT_Modifier Mod;
    Mod.TargetStatTag = STATTags::TAG_Stat_Flow_Stack;
    Mod.Operation = EModifierOp::Add;
    Mod.Value = Amount;
    ApplyModifier(Mod);
}

void USTATAbilityComponent::AddSignatureCharge(float Amount)
{
    FSTAT_Modifier Mod;
    Mod.TargetStatTag = STATTags::TAG_Stat_Signature_Charge_Stack;
    Mod.Operation = EModifierOp::Add;
    Mod.Value = Amount;
    ApplyModifier(Mod);
}
