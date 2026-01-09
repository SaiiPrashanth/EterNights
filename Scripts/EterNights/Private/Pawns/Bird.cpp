#include "Pawns/Bird.h"
#include "Components/CapsuleComponent.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"



ABird::ABird()
{
	PrimaryActorTick.bCanEverTick = true;

	CapsuleComponent = CreateDefaultSubobject<UCapsuleComponent>(TEXT("CapsuleComponent"));
	CapsuleComponent->SetCapsuleRadius(20.f);
	CapsuleComponent->SetCapsuleHalfHeight(20.f);
	SetRootComponent(CapsuleComponent);

	BirdMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("BirdMeshComponent"));
	BirdMesh->SetupAttachment(GetRootComponent());

	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComponent"));
	SpringArmComponent->SetupAttachment(GetRootComponent());

	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	AutoPossessPlayer = EAutoReceiveInput::Player0;


}

void ABird::BeginPlay()
{
	Super::BeginPlay();

	if (APlayerController* PlayerController = Cast<APlayerController>(GetController()))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(BirdInputMapping, 0);
		}
	}
}

void ABird::MouseAxis(const FInputActionValue& Value)
{
	const FVector2D MouseValue = Value.Get<FVector2D>();
	if (GetController() != nullptr)
	{
		AddControllerYawInput(MouseValue.X);
		AddControllerPitchInput(MouseValue.Y);
	}
}

void ABird::FlyForward(const FInputActionValue& Value)
{
	const float DirectionValue = Value.Get<float>();
	if ((GetController() != nullptr) && (DirectionValue != 0))
	{
		FVector Forward = GetActorForwardVector();
		AddMovementInput(Forward, DirectionValue);
	}

}

void ABird::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ABird::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(FlyForwardAction, ETriggerEvent::Triggered, this, &ABird::FlyForward);
		EnhancedInputComponent->BindAction(MouseAxisAction, ETriggerEvent::Triggered, this, &ABird::MouseAxis);
	}
}

