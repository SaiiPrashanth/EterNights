// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/BaseCharacter.h"
#include "Items/Weapon.h"
#include "Components/BoxComponent.h"
#include "Component/AttributeComponent.h"



ABaseCharacter::ABaseCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	Attributes = CreateDefaultSubobject<UAttributeComponent>(TEXT("Attributes"));

}

void ABaseCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABaseCharacter::Attack()
{
}

void ABaseCharacter::Die()
{
}

void ABaseCharacter::PlayAttackMontage()
{
}

void ABaseCharacter::AttackEnd()
{
}

bool ABaseCharacter::CanAttack()
{
	return false;
}

void ABaseCharacter::DirectionalHitReact(const FVector& ImpactPoint)
{	const FVector Forwardvector = GetActorForwardVector();
	const FVector ImpactPointXY(ImpactPoint.X, ImpactPoint.Y, GetActorLocation().Z);
	const FVector ImpactVector = (ImpactPointXY - GetActorLocation()).GetSafeNormal();
	const double CosTheta = FVector::DotProduct(Forwardvector, ImpactVector);
	double Theta = FMath::Acos(CosTheta);
	Theta = FMath::RadiansToDegrees(Theta);

	FVector CrossProduct = FVector::CrossProduct(Forwardvector, ImpactVector);
	if (CrossProduct.Z < 0) { Theta *= -1.f; }


	if (-45 <= Theta && Theta < 45) { PlayHitReactMontage(FName("FromFront")); }
	else if (-135 <= Theta && Theta < -45) { PlayHitReactMontage(FName("FromLeft")); }
	else if (45 <= Theta && Theta < 135) { PlayHitReactMontage(FName("FromRight")); }
	else
	{
		PlayHitReactMontage(FName("FromBack"));

	}
}

void ABaseCharacter::PlayHitReactMontage(const FName& SectionName)
{
		UAnimInstance* AnimInst = GetMesh()->GetAnimInstance();
	if (AnimInst && HitReactMontage) {
		AnimInst->Montage_Play(HitReactMontage);
		AnimInst->Montage_JumpToSection(SectionName, HitReactMontage);
	}
}

void ABaseCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABaseCharacter::SetWeaponCollision(ECollisionEnabled::Type CollisionType)
{	if (EquippedWeapon && EquippedWeapon->GetWeaponCollider())
	{
		EquippedWeapon->GetWeaponCollider()->SetCollisionEnabled(CollisionType);
		EquippedWeapon->IgnoreActors.Empty();
	}
}


