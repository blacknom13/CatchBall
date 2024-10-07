// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDA.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API UItemDA : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	FName Name;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	TSoftObjectPtr<UStaticMesh> ItemMesh;

	UPROPERTY(BlueprintReadOnly, EditDefaultsOnly)
	int32 PickupBonus;

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("ItemDA", GetFName());
	}
};
