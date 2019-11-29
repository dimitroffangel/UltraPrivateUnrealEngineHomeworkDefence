// Fill out your copyright notice in the Description page of Project Settings.


#include "Slow.h"
#include "TopDownARPG.h"
#include "Engine/World.h"
#include "Blueprint/AIBlueprintHelperLibrary.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Engine/LatentActionManager.h"
#include "TopDownARPGCharacter.h"
#include "Projectiles/TargetBullet.h"
#include "Runtime/Engine/Classes/Components/DecalComponent.h"

void USlow::Activate(AActor* Source)
{
	Super::Activate(Source);

	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("USlow::Activate IsValid(World) == false"));
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Source;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	FVector SpawnLocation = Source->GetActorLocation() + Source->GetActorForwardVector() * 100.0f;

	ATargetBullet* Projectile = World->SpawnActor<ATargetBullet>(BulletClass, SpawnLocation, Source->GetActorRotation(), SpawnParameters);

	if (IsValid(Projectile) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("USlow::Activate IsValid(Projectile) == false"));
		return;
	}

	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(Source);

	if (IsValid(Character) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("USlow::Activate IsValid(Character) == false"));
		return;
	}

	UDecalComponent* CursorToWorld = Character->GetCursorToWorld();

	if (IsValid(Character) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("USlow::Activate IsValid(CursorToWorld) == false"));
		return;
	}

	Projectile->SetDestLocation(CursorToWorld->GetComponentLocation());

	Projectile->OnDestroyingBullet.AddUObject(this, &USlow::OnBulletReachingDestination);

	//FLatentActionInfo LatentActionInfo;

	//UKismetSystemLibrary::MoveComponentTo(Projectile->GetOwner()->GetRootComponent(), CursorToWorld->GetWorldLocation(),
	//	CursorToWorld->GetWorldRotation(), true, false, 3, true,
	//		EMoveComponentAction::Move, LatentActionInfo);
}

void USlow::OnBulletReachingDestination(AActor* Source)
{
	UWorld* World = GetWorld();
	if (IsValid(World) == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("USlow::OnBulletReachingDestination IsValid(World) == false"));
	}

	FActorSpawnParameters SpawnParameters;
	SpawnParameters.Owner = Source;
	SpawnParameters.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;

	FVector SpawnLocation = Source->GetActorLocation();

	AActor* OnHitActor = World->SpawnActor<AActor>(OnHitSpawningClass, SpawnLocation, Source->GetActorRotation(), SpawnParameters);

	if (IsValid(OnHitActor) == false)
	{
		UE_LOG(LogTopDownARPG, Warning, TEXT("USlow::OnBulletReachingDestination IsValid(Projectile) == false"));
		return;
	}
}