// Fill out your copyright notice in the Description page of Project Settings.


#include "SlowingField.h"
#include "Components/SphereComponent.h"
#include "Engine.h"
#include "TopDownARPG.h"

// Sets default values
ASlowingField::ASlowingField()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PrimaryActorTick.bStartWithTickEnabled = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	RootComponent = SphereComponent;

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &ASlowingField::OnOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &ASlowingField::OnEndOverlap);

	UWorld* World = GetWorld();

	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("ASlowingField::ASlowingField World IsValid(World) == false"));
		return;
	}

	TimerManager = &World->GetTimerManager();
	TimerManager->SetTimer(CooldownTimerHandle, this, &ASlowingField::OnElapsedTime, CooldownTimeDilation, false, CooldownTime);

	this->Tags.Add("Area Effect");
}

void ASlowingField::OnOverlap(UPrimitiveComponent* OnOverlapppedCamp, AActor* Other, UPrimitiveComponent* OtherCamp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Other)
	{
		UE_LOG(LogTopDownARPG, Display, TEXT("ASlowingField::OnOverlap Entering Overlap..."));
		float actorTimeDialation = Other->GetActorTimeDilation();

		ObjectsOverlapped.Add({ Other, Other->CustomTimeDilation });
		Other->CustomTimeDilation = actorTimeDialation - SlowTime;
	}
}

void ASlowingField::OnEndOverlap(UPrimitiveComponent* OnOverlapppedCamp, AActor* Other, UPrimitiveComponent* OtherCamp, int32 OtherBodyIndex)
{
	if (Other)
	{
		UE_LOG(LogTopDownARPG, Display, TEXT("ASlowingField::OnEndOverlap Ending overlap..."));

		FObjectOverlapped OverlappedObject;

		for(int32 Index = 0; Index < ObjectsOverlapped.Num(); ++Index)
		{
			OverlappedObject = ObjectsOverlapped[Index];

			if (OverlappedObject.Overlapper == Other)
			{
				Other->CustomTimeDilation += SlowTime;
				ObjectsOverlapped.Remove(OverlappedObject);
				return;
			}
		}

	}
}

void ASlowingField::OnElapsedTime()
{
	//BeginDestroy();

	// remove every character from the sphere
	FObjectOverlapped lastObject;
	while (ObjectsOverlapped.Num() != 0)
	{
		lastObject = ObjectsOverlapped.Top();


		lastObject.Overlapper->CustomTimeDilation += SlowTime;
		ObjectsOverlapped.Pop();
	}

	SlowTime = 0;

	// destroy the object basically
	if (TimerManager)
	{
		TimerManager->ClearTimer(CooldownTimerHandle);
		TimerManager = nullptr;
	}

	UWorld* World = GetWorld();

	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("ASlowingField::OnElapsedTime World IsValid(World) == false"));
		return;
	}


	TimerManager = &World->GetTimerManager();
	TimerManager->SetTimer(CooldownTimerHandle, this, &ASlowingField::DestroyField, CooldownTimeDilation, false, AfterEndingEffectLiveDuration);
}

void ASlowingField::DestroyField()
{
	Destroy();
}

void ASlowingField::BeginDestroy()
{
	UE_LOG(LogTopDownARPG, Display, TEXT("Ending Life OF ASlowingField"));

	FObjectOverlapped lastObject;

	while (ObjectsOverlapped.Num() != 0)
	{
		lastObject = ObjectsOverlapped.Top();


		lastObject.Overlapper->CustomTimeDilation += SlowTime;
		ObjectsOverlapped.Pop();
	}

	SlowTime = 0;

	// destroy the object basically
	if (TimerManager)
	{
		TimerManager->ClearTimer(CooldownTimerHandle);
		TimerManager = nullptr;
	}

	Super::BeginDestroy();
}


bool operator==(const FObjectOverlapped& a, const FObjectOverlapped& b)
{
	return a.Overlapper == b.Overlapper;
}
