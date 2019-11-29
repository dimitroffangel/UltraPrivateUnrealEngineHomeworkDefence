// Fill out your copyright notice in the Description page of Project Settings.


#include "TargetBullet.h"
#include "TopDownARPG.h"

void ATargetBullet::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	const float Distance = FVector::Dist(DestLocation, this->GetActorLocation());

	//UE_LOG(LogTopDownARPG, Display, TEXT("Cursor location is: %f"), Distance);

	// I have set it just to make the Slow effect initializing bullet end in some case (a.k.a when it is moving towards through the cursor)
	if (Distance < 120.f && !bIsMarkedDead)
	{
		UE_LOG(LogTopDownARPG, Display, TEXT("TargetBullet has reached location"));
		
		if (!bIsMarkedDead)
		{
			bIsMarkedDead = true;
			OnDestroyingBullet.Broadcast(this);
			OnDestroyingBullet.RemoveAll(this);
		}

		if (Destroy() == false)
		{
			UE_LOG(LogTopDownARPG, Error, TEXT("ATargetBullet::Tick -> Projectile is indestructable"));
		}
	}
}

void ATargetBullet::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex,
	bool bFromSweep, const FHitResult& SweepResult)
{
	if (!bIsMarkedDead)
	{
		bIsMarkedDead = true;
		OnDestroyingBullet.Broadcast(this);
		OnDestroyingBullet.RemoveAll(this);
	}

	Super::OnOverlap(OverlappedComp, Other, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}
