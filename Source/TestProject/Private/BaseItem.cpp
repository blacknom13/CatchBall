// Fill out your copyright notice in the Description page of Project Settings.


#include "BaseItem.h"
#include "TestGameStateBase.h"
#include "SimpleFunctionLibrary.h"
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
	EstimatedLandingLocation = USimpleFunctionLibrary::CalculateItemLandingLocation(this);
	Item->OnComponentBeginOverlap.AddUniqueDynamic(this, &ABaseItem::ItemBeginOverlap);
	ProjectileMovement->OnProjectileStop.AddUniqueDynamic(this, &ABaseItem::OnItemHitObsticle);
	GameState=Cast<ATestGameStateBase>(GetWorld()->GetGameState());
}

void ABaseItem::ResetOnGameState(AActor* Actor)
{
	GameState->SetItemInGame(nullptr);
}

void ABaseItem::ItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor,
                                   UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep,
                                   const FHitResult& SweepResult)
{
	ABaseAICharacter* Character=Cast<ABaseAICharacter>(OtherActor);
	if (Character)
	{
		if (HasAuthority())
			Character->PickUp(this);
	}
}

void ABaseItem::OnItemHitObsticle(const FHitResult& ImpactResult)
{
	EstimatedLandingLocation=ImpactResult.Location;
	FTimerHandle AfterHitHandle;
	if (HasAuthority())
	{
		GetWorld()->GetTimerManager().SetTimer(AfterHitHandle, this, &ABaseItem::UpdateItemPosition, .03, true);
		Item->SetSimulatePhysics(true);
		SetLifeSpan(4);
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

void ABaseItem::UpdateItemPosition() const
{
	GameState->ItemLocationChanged();
}
