#include "Items/Weapon.h"
#include "Characters/EchoCharacter.h"
#include "Components/BoxComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SphereComponent.h"
#include "Interface/HitInterface.h"
#include "NiagaraComponent.h"

AWeapon::AWeapon()
{
	BoxComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComponent"));
	BoxComponent->SetupAttachment(ItemMesh);
	BoxComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	BoxComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComponent->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);

	BoxTraceStart = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceStart"));
	BoxTraceStart->SetupAttachment(ItemMesh);

	BoxTraceEnd = CreateDefaultSubobject<USceneComponent>(TEXT("BoxTraceEnd"));
	BoxTraceEnd->SetupAttachment(ItemMesh);
}

void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWeapon::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	Super::OnSphereOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex, bFromSweep, SweepResult);
}

void AWeapon::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	Super::OnSphereEndOverlap(OverlappedComponent, OtherActor, OtherComp, OtherBodyIndex);
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
	BoxComponent->OnComponentBeginOverlap.AddDynamic(this, &AWeapon::OnBoxOverlap);
}

void AWeapon::OnBoxOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	const FVector BoxStart = BoxTraceStart->GetComponentLocation();
	const FVector BoxEnd = BoxTraceEnd->GetComponentLocation();
	TArray<AActor*> ActorsToIgnore;
	for (AActor* Actor : IgnoreActors) {
		ActorsToIgnore.AddUnique(Actor);
	}
	FHitResult HitResult;
	UKismetSystemLibrary::BoxTraceSingle(this, BoxStart, BoxEnd, FVector(5.f, 5.f, 5.f), BoxTraceStart->GetComponentRotation(), ETraceTypeQuery::TraceTypeQuery1, false, ActorsToIgnore, EDrawDebugTrace::None, HitResult, true);
	if (HitResult.GetActor()) {
		UGameplayStatics::ApplyDamage(
			HitResult.GetActor(),
			WeaponDamage,
			GetInstigator()->GetController(),
			this,
			UDamageType::StaticClass()
		);

		IHitInterface* HitInterface = Cast<IHitInterface>(HitResult.GetActor());
		if (HitInterface) {
			HitInterface->Execute_GetHit(HitResult.GetActor(), HitResult.ImpactPoint);
		}
		IgnoreActors.AddUnique(HitResult.GetActor());


	}
	GenerateField(HitResult.ImpactPoint);
}

void AWeapon::AttachWeapon(USceneComponent* Mesh, FName SocketName)
{
	FAttachmentTransformRules AttachmentRules(EAttachmentRule::SnapToTarget, true);
	ItemMesh->AttachToComponent(Mesh, AttachmentRules, SocketName);
}

void AWeapon::Equip(USceneComponent* Mesh, FName SocketName, AActor* NewOwner, APawn* NewInstigator)
{
	SetOwner(NewOwner);
	SetInstigator(NewInstigator);
	AttachWeapon(Mesh, SocketName);
	ItemState = EItemState::EIS_PickedUp;
	EmberFx->Deactivate();

	if (EquipSound) {
		UGameplayStatics::PlaySoundAtLocation(this, EquipSound, GetActorLocation());
	}

	if (SphereComponent) {
		SphereComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}
