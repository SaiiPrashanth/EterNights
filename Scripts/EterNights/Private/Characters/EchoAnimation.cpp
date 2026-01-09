// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/EchoAnimation.h"
#include "Characters/EchoCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/KismetMathLibrary.h"

void UEchoAnimation::NativeInitializeAnimation()
{
	EchoCharacter = Cast<AEchoCharacter>(TryGetPawnOwner());
	if (EchoCharacter)
	{
		MovementComponent = EchoCharacter->GetCharacterMovement();
	}
}

void UEchoAnimation::NativeUpdateAnimation(float DeltaSeconds)
{
	if (MovementComponent)
	{
		
		GroundSpeed = UKismetMathLibrary::VSizeXY(MovementComponent->Velocity);
		IsFalling =MovementComponent->IsFalling();
		CharacterState = EchoCharacter->GetCharacterState();
	}
}



