// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Treasure.generated.h"

/**
 * 
 */
UCLASS()
class ETERNIGHTS_API ATreasure : public AItems
{
	GENERATED_BODY()
	

protected:
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UPROPERTY(EditAnywhere, Category = "Sound")
	class USoundBase* PickupSound;
};
