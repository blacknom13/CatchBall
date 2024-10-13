// Fill out your copyright notice in the Description page of Project Settings.

#include "BaseAICharacter.h"
#include "BaseItem.h"
#include "Net/UnrealNetwork.h"

// Sets default values
ABaseAICharacter::ABaseAICharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	NamePlateC=CreateDefaultSubobject<UWidgetComponent>("Name Plate");
	NamePlateC->SetupAttachment(GetRootComponent());
}

void ABaseAICharacter::PickUp(ABaseItem* Item)
{
	Score+=Item->ItemDA->PickupBonus;
	Item->Destroy();
}

void ABaseAICharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ABaseAICharacter, Name);
	DOREPLIFETIME(ABaseAICharacter, Score);
}

// Called when the game starts or when spawned
void ABaseAICharacter::BeginPlay()
{
	Super::BeginPlay();

}