#pragma once

#include "CoreMinimal.h"
#include "../Characters/BaseCharacter.h"
#include "../Characters/CharacterState.h"
#include "Enemy.generated.h"


UCLASS()
class ETERNIGHTS_API AEnemy : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AEnemy();

	virtual void Tick(float DeltaTime) override;

	void CheckPatrolTarget();

	void CheckCombatTarget();

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void GetHit_Implementation(const FVector& ImpactPoint);


	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;



	virtual void Die() override;

	UFUNCTION()
	void PawnSeen(APawn* SeenPawn);

protected:
	virtual void BeginPlay() override;





	UPROPERTY(VisibleAnywhere)
	class UHealthBarComponent* HealthBarComp;

	UPROPERTY(BlueprintReadOnly)
	EDeathPose DeathPose = EDeathPose::EDP_Alive;

	UPROPERTY(VisibleAnywhere)
	AActor* CombatTarget;

	UPROPERTY(EditAnywhere)
	float CombatRadius = 500.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	AActor* PatrolTarget;

	UPROPERTY(EditAnywhere, Category = "AI")
	TArray<AActor*> PatrolTargets;

	UPROPERTY(EditAnywhere, Category = "AI")
	float PatrolRadius = 200.f;

	UPROPERTY(EditAnywhere, Category = "AI")
	float AttackRadius = 150.f;

	UPROPERTY()
	class AAIController* EnemyController;

	bool InTargetRange(AActor* Target, float Radius);

	FTimerHandle PatrolTimer;

	void PatrolTimerOver();

	AActor* TargetSelection();
	void MoveToActor(AActor* Target);

	UPROPERTY(VisibleAnywhere)
	class UPawnSensingComponent* PawnSense;

	EEnemyState EnemyState = EEnemyState::EES_Patrolling;
};
