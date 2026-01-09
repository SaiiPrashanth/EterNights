// Fill out your copyright notice in the Description page of Project Settings.

#include "Items/BreakableActors.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "Items/Treasure.h"
#include "Components/CapsuleComponent.h"

ABreakableActors::ABreakableActors()
{
	PrimaryActorTick.bCanEverTick = true;
	BreakableActor = CreateDefaultSubobject<UGeometryCollectionComponent>(TEXT("BreakableActor"));
	SetRootComponent(BreakableActor);
	BreakableActor->SetGenerateOverlapEvents(true);
	BreakableActor->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	BreakableActor->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule Component"));
	Capsule->SetupAttachment(GetRootComponent());
	Capsule->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Block);
}

void ABreakableActors::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (bBroken) { return; }
	bBroken = true;
	UWorld* World = GetWorld();
	if (World && TreasureClass.Num() > 0)
	{
		const int32 RandNum = FMath::RandRange(0, TreasureClass.Num() - 1);
		World->SpawnActor<ATreasure>(TreasureClass[RandNum], GetActorLocation() + FVector(0.f, 0.f, 75.f), GetActorRotation());
	}
}

void ABreakableActors::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABreakableActors::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

