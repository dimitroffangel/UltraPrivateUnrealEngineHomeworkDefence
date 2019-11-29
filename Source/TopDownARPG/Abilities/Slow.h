// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Ability.h"
#include "Slow.generated.h"

/**
 * 
 */
UCLASS()
class TOPDOWNARPG_API USlow : public UAbility
{
	GENERATED_BODY()
	
public:
	virtual void Activate(AActor* Source) override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class ATargetBullet> BulletClass;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<class AActor> OnHitSpawningClass;

	UFUNCTION(BlueprintCallable)
	void OnBulletReachingDestination(AActor* Source);
};
