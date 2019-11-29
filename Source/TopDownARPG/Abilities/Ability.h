// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Ability.generated.h"

UENUM(BlueprintType)
enum class EAbilityMassivenessEnum : uint8
{
	AME_EnvironmentalBoon UMETA(DisplayName ="Affects Environment"),
	AME_TargetSpecific UMETA(DisplayName = "Target specific")
};

/**
 * 
 */
UCLASS(Blueprintable)
class UAbility : public UObject
{
	GENERATED_BODY()
	
public:
    UPROPERTY(EditAnywhere)
    float CooldownTime = 5.f;
    UPROPERTY(EditAnywhere)
    float CooldownTimeDilation = 1.f;

    UFUNCTION(BlueprintCallable)
    virtual void Activate(AActor* Source);

    UFUNCTION(BlueprintImplementableEvent)
    void OnActivateBlueprint(AActor* Source);

    UFUNCTION(BlueprintCallable)
    bool IsOffCooldown() const { return bIsOffCooldown; }

	UPROPERTY(EditAnywhere)
	EAbilityMassivenessEnum EAbilityEnum;

    virtual void BeginDestroy() override;

private:
    bool bIsOffCooldown = true;
    FTimerManager* TimerManager = nullptr;
    FTimerHandle CooldownTimerHandle;
    void OnCooldownTimerExpired();
};
