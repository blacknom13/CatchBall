// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ItemInterface.h"
#include "GameFramework/Actor.h"
#include "BaseItem.generated.h"

UCLASS()
class TESTPROJECT_API ABaseItem : public AActor, public IItemInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ABaseItem();
	
	virtual float GetItemGravity_Implementation() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
