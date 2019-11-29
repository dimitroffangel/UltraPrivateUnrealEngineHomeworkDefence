// Fill out your copyright notice in the Description page of Project Settings.


#include "Projectile.h"
#include "TopDownARPGCharacter.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "TopDownARPG.h"
#include "Triggers/SlowingField.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	SphereComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldStatic, ECollisionResponse::ECR_Overlap);
	RootComponent = SphereComponent;

	SphereComponent->OnComponentBeginOverlap.AddUniqueDynamic(this, &AProjectile::OnOverlap);

	OnOverlapParticle = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	OnOverlapParticle->bAutoActivate = false;

	MovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovementComponent"));
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
}

void AProjectile::OnOverlap(UPrimitiveComponent* OverlappedComp, AActor* Other, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	UE_LOG(LogTopDownARPG, Display, TEXT("%s"), *Other->GetName());

	if (IsValid(OnOverlapParticle))
	{
		OnOverlapParticle->ActivateSystem();
	}

	ATopDownARPGCharacter* Character = Cast<ATopDownARPGCharacter>(Other);
	if (IsValid(Character) && Damage > 0)
	{
		Character->TakeDamage(Damage, FDamageEvent(UDamageType::StaticClass()), nullptr, this);
	}

	UE_LOG(LogTopDownARPG, Display, TEXT("It has a tag Area Effect -> %s"), (Other->ActorHasTag(FName("Area Effect")) ? TEXT("TRUE") : TEXT("FALSE")));

	//if (Other->Tags.Find("Area Effect") != -1)
	//	return;

	//UE_LOG(LogTopDownARPG, Display, TEXT("It has a tag Area Effect -> %s"), 
	//	(SlowingField->ActorHasTag("Area Effect") ? TEXT("TRUE") : TEXT("FALSE")));


	// Some abilities must be disregarded as the slow it is. Either this or make a new Enum in CollsionChannels
	if (Other->ActorHasTag(FName("Area Effect")))
	{
		return;
	}

	if (Destroy() == false)
	{
		UE_LOG(LogTopDownARPG, Error, TEXT("AProjectile::OnOverlap -> Projectile is indestructable"));
	}
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

