#pragma once

#include "CoreMinimal.h"
#include "Items.h"
#include "Weapon.generated.h"



UCLASS()
class ETERNIGHTS_API AWeapon : public AItems
{
	GENERATED_BODY()
	
public:

	AWeapon();

	virtual void Tick(float DeltaTime) override;

	void AttachWeapon(USceneComponent* Mesh, FName SocketName);

	void Equip(USceneComponent* Mesh, FName SocketName, AActor* NewOwner, APawn* NewInstigator);

	TArray<AActor*> IgnoreActors;

protected:

	virtual void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult) override;
	
	virtual void OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex) override;

	UPROPERTY(VisibleAnywhere, Category = "Weapon")
	class UBoxComponent* BoxComponent;
	
	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceStart;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* BoxTraceEnd;

	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, Category = "Sound")
	USoundBase* EquipSound;

	UFUNCTION()
	void OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION(BlueprintImplementableEvent)
	void GenerateField(const FVector& ImpactPoint);

	UPROPERTY(EditAnywhere, Category = "Weapon Property")
	float WeaponDamage = 30.f;

public:
	FORCEINLINE UBoxComponent* GetWeaponCollider() const { return BoxComponent; }
};
