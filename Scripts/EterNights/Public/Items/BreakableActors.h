// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interface/HitInterface.h"
#include "BreakableActors.generated.h"

UCLASS()
class ETERNIGHTS_API ABreakableActors : public AActor, public IHitInterface
{
	GENERATED_BODY()
	
public:	
	ABreakableActors();

	virtual void GetHit_Implementation(const FVector& ImpactPoint);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	UGeometryCollectionComponent* BreakableActor;

	UPROPERTY(EditAnywhere)
	TArray< TSubclassOf<class ATreasure> > TreasureClass;

	UPROPERTY(VisibleAnywhere,BlueprintReadWrite)
	class UCapsuleComponent* Capsule;

	bool bBroken = false;
public:	
	virtual void Tick(float DeltaTime) override;

};
