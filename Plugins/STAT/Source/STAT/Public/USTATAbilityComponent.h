#pragma once

#include "Components/ActorComponent.h"
#include "Interfaces/STAT_Damageable_If.h"
#include "Interfaces/STAT_Upgradable_If.h"
#include "Interfaces/STAT_SignatureSupport_If.h"
#include "Interfaces/STAT_Flow_If.h"
#include "Interfaces/STAT_Overloadable_If.h"
#include "Interfaces/STAT_Fusion_If.h"
#include "STATTypes.h"
#include "USTATAbilityComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSTAT_OnStatChanged_E);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSTAT_OnSignatureChargeChanged_E);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSTAT_OnSignatureChargeReady_E);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSTAT_OnFlowStackChanged_E);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSTAT_OnOverloadStateChanged_E);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSTAT_OnFusionAmpChanged_E);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FSTAT_OnCounterplayWindow_E);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class STAT_API USTATAbilityComponent : public UActorComponent,
    public ISTAT_Damageable_If,
    public I_STAT_Upgradable_If,
    public I_STAT_SignatureSupport_If,
    public I_STAT_Flow_If,
    public I_STAT_Overloadable_If,
    public I_STAT_Fusion_If
{
    GENERATED_BODY()

public:
    USTATAbilityComponent();

    // Event Dispatchers
    UPROPERTY(BlueprintAssignable, Category="STAT|Event")
    FSTAT_OnStatChanged_E STAT_OnStatChanged_E;

    UPROPERTY(BlueprintAssignable, Category="STAT|Event")
    FSTAT_OnSignatureChargeChanged_E STAT_OnSignatureChargeChanged_E;

    UPROPERTY(BlueprintAssignable, Category="STAT|Event")
    FSTAT_OnSignatureChargeReady_E STAT_OnSignatureChargeReady_E;

    UPROPERTY(BlueprintAssignable, Category="STAT|Event")
    FSTAT_OnFlowStackChanged_E STAT_OnFlowStackChanged_E;

    UPROPERTY(BlueprintAssignable, Category="STAT|Event")
    FSTAT_OnOverloadStateChanged_E STAT_OnOverloadStateChanged_E;

    UPROPERTY(BlueprintAssignable, Category="STAT|Event")
    FSTAT_OnFusionAmpChanged_E STAT_OnFusionAmpChanged_E;

    UPROPERTY(BlueprintAssignable, Category="STAT|Event")
    FSTAT_OnCounterplayWindow_E STAT_OnCounterplayWindow_E;

    // ISTAT_Damageable_If
    virtual void ApplyDamage_Implementation(float BaseDamage, AActor* Instigator, const FGameplayTagContainer& AttackTags, FName CauseTag) override;

    // I_STAT_Upgradable_If
    virtual void RequestUpgradeStat_Implementation(const FGameplayTag& StatTagToUpgrade) override;

    // I_STAT_SignatureSupport_If
    virtual void AddSignatureCharge_Implementation(AActor* TargetActor, FGameplayTag CauseTag, float Amount) override;

    // I_STAT_Flow_If
    virtual void AddFlowStack_Implementation(float Amount, FGameplayTag SourceTag, AActor* Instigator) override;
    virtual void ResetFlow_Implementation(FGameplayTag ReasonTag) override;

    // I_STAT_Overloadable_If
    virtual void RequestOverload_Implementation(const FSTAT_OverloadParameters& Params) override;

    // I_STAT_Fusion_If
    virtual void NotifyFusionEvent_Implementation(FGameplayTag EventTag, AActor* Source) override;

    // Internal Methods
    UFUNCTION(BlueprintCallable, Category="STAT|Damage")
    FGameplayTagContainer CollectDefenseRules();

    UFUNCTION(BlueprintCallable, Category="STAT|Damage")
    void MatchAttackVsDefense();

    UFUNCTION(BlueprintCallable, Category="STAT|Damage")
    void ComputeFinalDamage();

    UFUNCTION(BlueprintCallable, Category="STAT|Damage")
    void ApplyDamageToTarget();

    UFUNCTION(BlueprintCallable, Category="STAT|Upgrade")
    void ValidateUpgradable();

    UFUNCTION(BlueprintCallable, Category="STAT|Upgrade")
    void ConsumeResource();

    UFUNCTION(BlueprintCallable, Category="STAT|Upgrade")
    void ApplyUpgradeModifier();

    UFUNCTION(BlueprintCallable, Category="STAT|Synergy")
    void EnqueueSynergyRecalc();

    UFUNCTION(BlueprintCallable, Category="STAT|Synergy")
    void CalculateSynergyScore();

    UFUNCTION(BlueprintCallable, Category="STAT|Synergy")
    void ApplySynergyLevelTags();

    UFUNCTION(BlueprintCallable, Category="STAT|Signature")
    void CheckSignatureReady();

    UFUNCTION(BlueprintCallable, Category="STAT|Flow")
    void Flow_OnHitGain();

    UFUNCTION(BlueprintCallable, Category="STAT|Flow")
    void Flow_OnDamageTakenReset();

    UFUNCTION(BlueprintCallable, Category="STAT|Flow")
    void Flow_DecayTick();

    UFUNCTION(BlueprintCallable, Category="STAT|Flow")
    void Flow_ApplyRewards();

    UFUNCTION(BlueprintCallable, Category="STAT|Overload")
    void Overload_Start();

    UFUNCTION(BlueprintCallable, Category="STAT|Overload")
    void Overload_Tick();

    UFUNCTION(BlueprintCallable, Category="STAT|Overload")
    void Overload_End();

    UFUNCTION(BlueprintCallable, Category="STAT|Fusion")
    void Fusion_CheckAmp();

    UFUNCTION(BlueprintCallable, Category="STAT|Counterplay")
    void Counterplay_OpenWindow();

    UFUNCTION(BlueprintCallable, Category="STAT|Counterplay")
    void Counterplay_Resolve();
};

