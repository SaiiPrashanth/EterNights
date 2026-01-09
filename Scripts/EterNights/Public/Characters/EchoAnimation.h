// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CharacterState.h"
#include "EchoAnimation.generated.h"


UCLASS()
class ETERNIGHTS_API UEchoAnimation : public UAnimInstance
{
	GENERATED_BODY()
	

public:

	virtual void NativeInitializeAnimation() override;

	virtual void NativeUpdateAnimation(float DeltaSeconds) override;

	UPROPERTY(BlueprintReadOnly)
	class AEchoCharacter* EchoCharacter;

	UPROPERTY(BlueprintReadOnly,Category = "Movement")
	class UCharacterMovementComponent* MovementComponent;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	float GroundSpeed;

	UPROPERTY(BlueprintReadOnly, Category = "Movement")
	bool IsFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Character State")
	ECharacterState CharacterState;
};
