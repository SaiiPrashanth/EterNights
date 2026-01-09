// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/HitInterface.h"
#include "BaseCharacter.generated.h"

UCLASS()
class ETERNIGHTS_API ABaseCharacter : public ACharacter, public IHitInterface
{
	GENERATED_BODY()

public:
	ABaseCharacter();
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(ECollisionEnabled::Type CollisionType);

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class AWeapon* EquippedWeapon;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* DeathMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* HitReactMontage;

	virtual void Attack();
	virtual void Die();

	virtual void PlayAttackMontage();


	UFUNCTION(BlueprintCallable)
	virtual void AttackEnd();

	virtual bool CanAttack();

	UPROPERTY(VisibleAnywhere)
	class UAttributeComponent* Attributes;

	void DirectionalHitReact(const FVector& ImpactPoint);
	void PlayHitReactMontage(const FName& SectionName);

	UPROPERTY(EditAnywhere)
	USoundBase* HitSound;

	UPROPERTY(EditAnywhere)
	UParticleSystem* BloodVFX;
};
