// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy/Enemy.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Components/CapsuleComponent.h"
#include "Component/AttributeComponent.h"
#include "HUD/HealthBarComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AIController.h"
#include "Navigation/PathFollowingComponent.h"
#include "Perception/PawnSensingComponent.h"

AEnemy::AEnemy()
{
	PrimaryActorTick.bCanEverTick = true;
	GetMesh()->SetGenerateOverlapEvents(true);
	GetMesh()->SetCollisionObjectType(ECollisionChannel::ECC_WorldDynamic);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Camera, ECollisionResponse::ECR_Ignore);
	

	HealthBarComp = CreateDefaultSubobject<UHealthBarComponent>(TEXT("HealthBarComponent"));
	HealthBarComp->SetupAttachment(GetRootComponent());

	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationPitch = false;
	bUseControllerRotationRoll = false;
	bUseControllerRotationYaw = false;
	
	PawnSense = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComponent"));
	PawnSense->SetPeripheralVisionAngle(45.f);
	PawnSense->SightRadius = 4000.f;
}



float AEnemy::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (Attributes && HealthBarComp) {
		Attributes->ReceiveDamage(DamageAmount);
		HealthBarComp->SetHealthPercent(Attributes->GetHealth());
	}
	CombatTarget = EventInstigator->GetPawn();
	EnemyState = EEnemyState::EES_Chasing;
	GetCharacterMovement()->MaxWalkSpeed = 300.f;
	MoveToActor(CombatTarget);
	return DamageAmount;
}



void AEnemy::Die()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && DeathMontage) {
		AnimInstance->Montage_Play(DeathMontage);
		FName SectionName = FName();
		const int32 RandInt = FMath::RandRange(0, 1);
		switch (RandInt)
		{
		case 0:
			SectionName = FName("Death1");
			DeathPose = EDeathPose::EDP_DeathPose1;
			break;
		case 1:
			SectionName = FName("Death2");
			DeathPose = EDeathPose::EDP_DeathPose2;
			break;
		}
		AnimInstance->Montage_JumpToSection(SectionName, DeathMontage);
	}
	GetCapsuleComponent()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn,ECollisionResponse::ECR_Ignore);
	if (HealthBarComp) {
		HealthBarComp->SetVisibility(false);
	}
	SetLifeSpan(5.f);

}

void AEnemy::BeginPlay()
{
	Super::BeginPlay();

	if (HealthBarComp) {
		HealthBarComp->SetVisibility(false);
	}
	
	if(PawnSense){
	PawnSense->OnSeePawn.AddDynamic(this,&AEnemy::PawnSeen);
	}


	EnemyController = Cast<AAIController>(GetController());
	MoveToActor(PatrolTarget);
}

bool AEnemy::InTargetRange(AActor* Target, float Radius)
{
	if (Target == nullptr) return false;
	const double EnemyDistance = (Target->GetActorLocation() - GetActorLocation()).Size();
	return EnemyDistance <= Radius;
	
}

void AEnemy::PatrolTimerOver()
{
	MoveToActor(PatrolTarget);
}

AActor* AEnemy::TargetSelection()
{
	TArray<AActor*> UniqueTargets;
	for (AActor* UniqueTarget : PatrolTargets) {
		if (UniqueTarget != PatrolTarget) {
			UniqueTargets.AddUnique(UniqueTarget);
		}
	}
	const int32 TotalPatrolTargets = UniqueTargets.Num();
	if (TotalPatrolTargets > 0)
	{

		const int32 TargetSelection = FMath::RandRange(0, TotalPatrolTargets - 1);
		return UniqueTargets[TargetSelection];
	}
	return nullptr;
}

void AEnemy::MoveToActor(AActor* Target)
{
	if (EnemyController == nullptr || Target == nullptr) return;
	FAIMoveRequest MoveRequest;
	MoveRequest.SetGoalActor(Target);
	MoveRequest.SetAcceptanceRadius(15.f);
	EnemyController->MoveTo(MoveRequest);

}

void AEnemy::PawnSeen(APawn* SeenPawn)
{
	if(EnemyState == EEnemyState::EES_Chasing) return;
	if(SeenPawn->ActorHasTag(FName("Echo"))){
		if(EnemyState != EEnemyState::EES_Attacking){
			EnemyState = EEnemyState::EES_Chasing;
			CombatTarget = SeenPawn;
			MoveToActor(CombatTarget);
		}
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetCharacterMovement()->MaxWalkSpeed = 300.f;

	}
}

void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if(EnemyState > EEnemyState::EES_Patrolling)
	{
	CheckCombatTarget();
	}
	else
	{
	CheckPatrolTarget();
	}
	

}

void AEnemy::CheckPatrolTarget()
{
	if (InTargetRange(PatrolTarget, PatrolRadius))
	{

		PatrolTarget = TargetSelection();
		GetWorldTimerManager().SetTimer(PatrolTimer, this, &AEnemy::PatrolTimerOver, 5.f);

	}
	
}

void AEnemy::CheckCombatTarget()
{
	if (!InTargetRange(CombatTarget, CombatRadius)) {
		CombatTarget = nullptr;
		if (HealthBarComp) {
			HealthBarComp->SetVisibility(false);
		}
		EnemyState = EEnemyState::EES_Patrolling;
		GetCharacterMovement()->MaxWalkSpeed = 125.f;
		MoveToActor(PatrolTarget);
		
	}
	else if(!InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Chasing){
		EnemyState = EEnemyState::EES_Chasing;
		GetCharacterMovement()->MaxWalkSpeed = 300.f;
		MoveToActor(CombatTarget);
	}
	else if(InTargetRange(CombatTarget, AttackRadius) && EnemyState != EEnemyState::EES_Attacking){

		EnemyState = EEnemyState::EES_Attacking;
		
	}
}

void AEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void AEnemy::GetHit_Implementation(const FVector& ImpactPoint)
{
	if (HealthBarComp) {
		HealthBarComp->SetVisibility(true);
	}
	if (Attributes && !Attributes->isAlive()) {
		Die();
	}
	else {
		DirectionalHitReact(ImpactPoint);
	}
	
	
	if (HitSound) {
		UGameplayStatics::PlaySoundAtLocation(this, HitSound, ImpactPoint);
	}
	if (BloodVFX && GetWorld()) {
		UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), BloodVFX, ImpactPoint);
	}

}



