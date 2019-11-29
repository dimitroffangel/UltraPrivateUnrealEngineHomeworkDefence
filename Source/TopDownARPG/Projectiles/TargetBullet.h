// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectiles/Projectile.h"
#include "TargetBullet.generated.h"

/**
 * 
 */

// Multicast Delegate with only one param Actor (useful comment 101)
DECLARE_MULTICAST_DELEGATE_OneParam(FMultiCastDelegate, AActor*);

UCLASS()
class TOPDOWNARPG_API ATargetBullet : public AProjectile
{
	GENERATED_BODY()

public:
	virtual void OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, 
		bool bFromSweep, const FHitResult& SweepResult) override;

	void SetDestLocation(FVector Vector) { DestLocation = Vector; }

	//UPROPERTY(BlueprintReadWrite)
	FMultiCastDelegate OnDestroyingBullet;

private:
	UPROPERTY(EditAnywhere)
	FVector DestLocation;

	bool bIsMarkedDead = false;
	
public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
