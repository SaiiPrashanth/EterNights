#pragma once

#include "CoreMinimal.h"
#include "BaseCharacter.h"
#include "InputActionValue.h"
#include "CharacterState.h"
#include "EchoCharacter.generated.h"

class UCameraComponent;
class USpringArmComponent;
class UInputAction;
class UInputMappingContext;
class UGroomComponent;
class AItems;
class UAnimMontage;


UCLASS()
class ETERNIGHTS_API AEchoCharacter : public ABaseCharacter
{
	GENERATED_BODY()

public:

	AEchoCharacter();


	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Input Mapping")
	UInputAction* MouseAxisAction;

	UPROPERTY(EditAnywhere, Category = "Input Mapping")
	UInputMappingContext* EchoInputMapping;

	UPROPERTY(EditAnywhere, Category = "Input Mapping")
	UInputAction* MovementAction;

	UPROPERTY(EditAnywhere, Category = "Input Mapping")
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere, Category = "Input Mapping")
	UInputAction* EquipAction;

	UPROPERTY(EditAnywhere, Category = "Input Mapping")
	UInputAction* AttackAction;

	void MouseAxis(const FInputActionValue& Value);
	void Movement(const FInputActionValue& Value);
	void Equip();

	UFUNCTION(BlueprintCallable)
	void Disarm();
	
	UFUNCTION(BlueprintCallable)
	void Arm();

	virtual void Attack() override;

	virtual void PlayAttackMontage() override;

	void PlayEquipMontage(FName SectionName);

	UFUNCTION(BlueprintCallable)
	void FinishedEquipping();

	virtual void AttackEnd() override;

	virtual bool CanAttack() override;

	bool CanArm();
	bool CanDisarm();



private:	

	ECharacterState CharacterState = ECharacterState::ECS_Unequipped;

	UPROPERTY(BlueprintReadWrite, meta = (AllowPrivateAccess = "true"))
	EActionState ActionState = EActionState::EAS_UnOccupied;

	UPROPERTY(VisibleAnywhere)
	UCameraComponent* CameraComponent;

	UPROPERTY(VisibleAnywhere)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* HairGroom;

	UPROPERTY(VisibleAnywhere)
	UGroomComponent* EyeBrowGroom;

	UPROPERTY(VisibleInstanceOnly)
	AItems* OverlappingItem;

	UPROPERTY(EditDefaultsOnly, Category = "Montage")
	UAnimMontage* EquipMontage;





public:
	
	FORCEINLINE void SetOverlappingItem(AItems* item) { OverlappingItem = item; }
	FORCEINLINE ECharacterState GetCharacterState() const { return CharacterState; }
};
