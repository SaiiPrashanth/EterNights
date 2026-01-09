#include "Items/Items.h"
#include "EterNights/DebugMacros.h"
#include "EterNights/EterNights.h"
#include "Characters/EchoCharacter.h"
#include "Components/SphereComponent.h"
#include "NiagaraComponent.h"

AItems::AItems()
{
	PrimaryActorTick.bCanEverTick = true;
	ItemMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ItemMeshComponent"));
	RootComponent = ItemMesh;

	SphereComponent = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComponent"));
	SphereComponent->SetupAttachment(ItemMesh);

	EmberFx = CreateDefaultSubobject<UNiagaraComponent>(TEXT("EmberFx"));
	EmberFx->SetupAttachment(GetRootComponent());
}

void AItems::BeginPlay()
{
	Super::BeginPlay();
	SphereComponent->OnComponentBeginOverlap.AddDynamic(this, &AItems::OnSphereOverlap);
	SphereComponent->OnComponentEndOverlap.AddDynamic(this, &AItems::OnSphereEndOverlap);
}

float AItems::GetSinValue()
{
	return Amplitude * FMath::Sin(RunningTime * TimeConstant);
}

float AItems::GetCosValue()
{
	return Amplitude * FMath::Cos(RunningTime * TimeConstant);
}

void AItems::OnSphereOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{

	AEchoCharacter* Echo = Cast<AEchoCharacter>(OtherActor);
	if(Echo){
		Echo->SetOverlappingItem(this);
	}
	

}

void AItems::OnSphereEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	AEchoCharacter* Echo = Cast<AEchoCharacter>(OtherActor);
	if (Echo) {
		Echo->SetOverlappingItem(nullptr);
	}
	
}

void AItems::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	RunningTime += DeltaTime;

	if (ItemState == EItemState::EIS_Hovering)
	{
		AddActorWorldOffset(FVector(0.f, 0.f, GetSinValue()));
	}

	
}
