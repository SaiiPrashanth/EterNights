#include "Characters/EchoCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "GroomComponent.h"
#include "Items/Items.h"
#include "Items/Weapon.h"
#include "Animation/AnimMontage.h"
#include "Components/BoxComponent.h"

AEchoCharacter::AEchoCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(RootComponent);

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	HairGroom = CreateDefaultSubobject<UGroomComponent>(TEXT("HairGroom"));
	HairGroom->SetupAttachment(GetMesh());
	HairGroom->AttachmentName = FString("head");

	EyeBrowGroom = CreateDefaultSubobject<UGroomComponent>(TEXT("EyeBrowGroom"));
	EyeBrowGroom->SetupAttachment(GetMesh());
	EyeBrowGroom->AttachmentName = FString("head");

	AutoPossessPlayer = EAutoReceiveInput::Player0;
}

void AEchoCharacter::BeginPlay()
{
	Super::BeginPlay();

	Tags.Add(FName("Echo"));

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(EchoInputMapping, 0);
		}
	}

	
}
void AEchoCharacter::Equip()
{
	AWeapon* OverlappingWeapon = Cast<AWeapon>(OverlappingItem);
	if (OverlappingWeapon)
	{
		OverlappingWeapon->Equip(GetMesh(), FName("RightHandSocket"),this,this);
		CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;
		OverlappingItem = nullptr;
		EquippedWeapon = OverlappingWeapon;
	}
	else
	{
		if (CanDisarm())
		{
			PlayEquipMontage(FName("Disarm"));
			ActionState = EActionState::EAS_Equipping;
			CharacterState = ECharacterState::ECS_Unequipped;
		}
		else if (CanArm())
		{
			PlayEquipMontage(FName("Arm"));
			ActionState = EActionState::EAS_Equipping;
			CharacterState = ECharacterState::ECS_EquippedOneHandedWeapon;

		}

	}
}

void AEchoCharacter::Disarm()
{
	if (EquippedWeapon){
		EquippedWeapon->AttachWeapon(GetMesh(), FName("SpineSocket"));

	}
}

void AEchoCharacter::Arm()
{
	if (EquippedWeapon)
	{
		EquippedWeapon->AttachWeapon(GetMesh(), FName("RightHandSocket"));
	}
}

void AEchoCharacter::MouseAxis(const FInputActionValue& Value)
{
	const FVector2D MouseValue = Value.Get<FVector2D>();
	if (Controller != nullptr)
	{
		AddControllerYawInput(MouseValue.X);
		AddControllerPitchInput(MouseValue.Y);
	}
}

void AEchoCharacter::Movement(const FInputActionValue& Value)
{
	if (ActionState != EActionState::EAS_UnOccupied)
	{
		return;
	}

	const FVector2D MovementValue = Value.Get<FVector2D>();
	const FRotator Rotation = Controller->GetControlRotation();
	const FRotator YawRotation(0, Rotation.Yaw, 0);
	const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	AddMovementInput(Direction, MovementValue.Y);

	const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
	AddMovementInput(RightDirection, MovementValue.X);
}



void AEchoCharacter::Attack()
{
	if (CanAttack())
	{
		PlayAttackMontage();
		ActionState = EActionState::EAS_Attacking;
	}
}

void AEchoCharacter::PlayAttackMontage()
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && AttackMontage)
	{
		AnimInstance->Montage_Play(AttackMontage);
		int32 SectionIndex = FMath::RandRange(0, 1);
		FName SectionName;
		switch (SectionIndex)
		{
		case 0:
			SectionName = FName("Attack1");
			break;
		case 1:
			SectionName = FName("Attack2");
			break;
		default:
			return;
		}
		AnimInstance->Montage_JumpToSection(SectionName, AttackMontage);
	}
}

void AEchoCharacter::PlayEquipMontage(FName SectionName)
{
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	if (AnimInstance && EquipMontage)
	{
		AnimInstance->Montage_Play(EquipMontage);
		AnimInstance->Montage_JumpToSection(SectionName, EquipMontage);
	}
}

void AEchoCharacter::AttackEnd()
{
	ActionState = EActionState::EAS_UnOccupied;
}

bool AEchoCharacter::CanAttack()
{
	return ActionState == EActionState::EAS_UnOccupied && CharacterState != ECharacterState::ECS_Unequipped;
}

bool AEchoCharacter::CanArm()
{
	return ActionState == EActionState::EAS_UnOccupied && CharacterState == ECharacterState::ECS_Unequipped && EquippedWeapon;
}

bool AEchoCharacter::CanDisarm()
{
	return ActionState == EActionState::EAS_UnOccupied && CharacterState != ECharacterState::ECS_Unequipped;
}





void AEchoCharacter::FinishedEquipping()
{
	ActionState = EActionState::EAS_UnOccupied;
}

void AEchoCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEchoCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(MouseAxisAction, ETriggerEvent::Triggered, this, &AEchoCharacter::MouseAxis);
		EnhancedInputComponent->BindAction(MovementAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Movement);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(EquipAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Equip);
		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AEchoCharacter::Attack);
	}
}


