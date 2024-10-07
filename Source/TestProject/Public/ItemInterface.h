// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemInterface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UItemInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTPROJECT_API IItemInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="Speed")
	UPARAM(DisplayName="Gravity") float GetItemGravity();
	
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="ItemInfo")
	UPARAM(DisplayName=EstimatedLandingLocation) FVector GetEstimatedLandingLocation(); 
};
