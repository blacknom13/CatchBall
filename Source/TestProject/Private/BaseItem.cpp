// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "TestGameStateBase.h"
#include "Net/UnrealNetwork.h"

void ABaseItem::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseItem, ItemDA);
}


// Sets default values
ABaseItem::ABaseItem()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	AActor::SetReplicateMovement(true);
	Item = CreateDefaultSubobject<UStaticMeshComponent>("Item");
	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovement");
	ProjectileMovement->MaxSpeed = 4000;
	ProjectileMovement->InitialSpeed = 1000;

	Item->SetGenerateOverlapEvents(true);
	Item->SetCollisionEnabled(ECollisionEnabled::Type::QueryAndPhysics);
	Item->SetCollisionObjectType(ECC_GameTraceChannel1);
	Item->SetCollisionResponseToAllChannels(ECR_Ignore);
	Item->SetCollisionResponseToChannel(ECC_WorldStatic, ECR_Block);
	Item->SetCollisionResponseToChannel(ECC_WorldDynamic, ECR_Block);
	Item->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
}

void ABaseItem::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	if (HasAuthority())
	{
		SetItemStaticMesh();
		ProjectileMovement->SetUpdatedComponent(Item);
	}
}

// Called when the game starts or when spawned
void ABaseItem::BeginPlay()
{
	Super::BeginPlay();
	OnDestroyed.AddUniqueDynamic(this, &ABaseItem::ResetOnGameState);
	EstimatedNextLocation = CalculateItemLandingLocation();
	Item->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABaseItem::ItemBeginOverlap);
	ProjectileMovement->OnProjectileStop.AddUniqueDynamic(this, &ABaseItem::ProjectileMovementEnded);
	GameState = Cast<ATestGameStateBase>(GetWorld()->GetGameState());
}

void ABaseItem::ResetOnGameState(AActor* Actor)
{
	GameState->SetItemInGame(nullptr);
}

void ABaseItem::ItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                 UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                 const FHitResult& SweepResult)
{
	ABaseAICharacter* Character = Cast<ABaseAICharacter>(OtherActor);
	if (Character)
	{
		if (HasAuthority())
			Character->PickUp(this);
	}
}

void ABaseItem::ProjectileMovementEnded(const FHitResult& Hit)
{
	if (HasAuthority())
	{
		FTimerHandle AfterHitHandle;
		Item->SetSimulatePhysics(true);
		SetLifeSpan(4);
		GetWorld()->GetTimerManager().SetTimer(AfterHitHandle, this, &ABaseItem::UpdateItemPosition, .1, true);
	}
}

void ABaseItem::OnRep_ItemDA()
{
	SetItemStaticMesh();
}

void ABaseItem::SetItemStaticMesh() const
{
	if (ItemDA && ItemDA->ItemMesh.LoadSynchronous())
		Item->SetStaticMesh(ItemDA->ItemMesh.Get());
}

void ABaseItem::UpdateItemPosition()
{
	EstimatedNextLocation = CalculateNextItemLocation();
	GameState->ItemLocationChanged();
}

FVector ABaseItem::CalculateNextItemLocation() const
{
	//Calculates next predicted position for the item
	FVector StartingPosition = GetActorLocation();
	FVector V0 = GetVelocity();
	FVector V0xy = FVector(V0.X, V0.Y, 0);

	return StartingPosition + V0xy * (V0xy.Length() < 300 ? 0 : .5);
}

FVector ABaseItem::CalculateItemLandingLocation() const
{
	// Solve simple mechanical movement problem
	FVector Velocity = GetVelocity();
	FVector VelocityXY = FVector(Velocity.X, Velocity.Y, 0);
	float Gravity = ProjectileMovement->GetGravityZ();

	float A = .5 * Gravity;
	float B = -Velocity.Z;
	float C = GetActorLocation().Z;
	float Delta = B * B - 4 * A * C;

	if (Delta < 0)
	{
		return FVector(0, 0, 0);
	}

	float R1 = FMath::Abs((-B + FMath::Sqrt(Delta)) / (2 * A));
	float R2 = FMath::Abs((-B - FMath::Sqrt(Delta)) / (2 * A));

	float Time = Velocity.Z > 0 ? FMath::Max(R1, R2) : FMath::Min(R1, R2);

	FVector LandingLocation = VelocityXY * Time + GetActorLocation();
	return FVector(LandingLocation.X, LandingLocation.Y, 0);
}
