#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Items.generated.h"

class USphereComponent;

UENUM()
enum class EItemState : uint8
{
	EIS_Hovering,
	EIS_PickedUp

};

UCLASS()
class ETERNIGHTS_API AItems : public AActor
{
	GENERATED_BODY()
	
public:	
	AItems();
	virtual void Tick(float DeltaTime) override;

protected:

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Functions")
	float Amplitude = 0.25f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Sine Functions")
	float TimeConstant = 5.f;

	UFUNCTION(BlueprintPure)
	float GetSinValue();

	UFUNCTION(BlueprintPure)
	float GetCosValue();

	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComponent;

	UFUNCTION()
	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	UPROPERTY(VisibleAnywhere,BlueprintReadOnly)
	UStaticMeshComponent* ItemMesh;

	EItemState ItemState = EItemState::EIS_Hovering;

	UPROPERTY(EditAnywhere)
	class UNiagaraComponent* EmberFx;

private:	
	float RunningTime = 0.f;

	
};
