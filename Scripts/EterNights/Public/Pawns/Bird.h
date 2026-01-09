#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "InputActionValue.h"
#include "Bird.generated.h"

class UCapsuleComponent;
class UInputMappingContext;
class UInputAction;
class UCameraComponent;
class USpringArmComponent;

UCLASS()
class ETERNIGHTS_API ABird : public APawn
{
	GENERATED_BODY()

public:

	ABird();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	UInputMappingContext* BirdInputMapping;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	UInputAction* FlyForwardAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input Mapping")
	UInputAction* MouseAxisAction;

	void FlyForward(const FInputActionValue& Value);
	void MouseAxis(const FInputActionValue& Value);

private:	

	UPROPERTY(VisibleAnywhere)
	UCapsuleComponent* CapsuleComponent;

	UPROPERTY(VisibleAnywhere)
	USkeletalMeshComponent* BirdMesh;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;
};
