// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SlowingField.generated.h"




USTRUCT()
struct FObjectOverlapped
{
	GENERATED_BODY()

	AActor* Overlapper;
	float BeforeTimeDilation;
};

UCLASS()
class TOPDOWNARPG_API ASlowingField : public AActor
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	float SlowTime;

	UPROPERTY(EditAnywhere)
	float CooldownTime = 3.f;

	UPROPERTY(EditAnywhere)
	float CooldownTimeDilation = 1.f;

	UPROPERTY(EditAnywhere)
	float AfterEndingEffectLiveDuration = 3.f;

public:
	// Sets default values for this actor's properties
	ASlowingField();

	virtual void BeginDestroy() override;

protected:
	UPROPERTY(VisibleAnywhere, Category = Gameplay)
	class USphereComponent* SphereComponent;

	//UPROPERTY(VisibleAnywhere, Category = Gameplay)
	//class UParticleSystemComponent* OnOverlapParticle;

	UFUNCTION()
	void OnOverlap(UPrimitiveComponent* OnOverlapppedCamp, AActor* Other, UPrimitiveComponent* OtherCamp, int32 OtherBodyIndex, bool bFromSweep,
			const FHitResult& SweepResult);

	UFUNCTION()
	void OnEndOverlap(UPrimitiveComponent* OrOverlapppedCamp, AActor* Other, UPrimitiveComponent* OtherCamp, int32 OtherBodyIndex);

private:
	bool bIsOffCooldown = true;
	FTimerManager* TimerManager = nullptr;
	FTimerHandle CooldownTimerHandle;
	TArray<FObjectOverlapped> ObjectsOverlapped;
	void OnElapsedTime();
	void DestroyField();
};
