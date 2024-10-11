// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseItem.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SimpleFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class TESTPROJECT_API USimpleFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Category="Search")
	static UPARAM(DisplayName="TopScorers") TArray<int32> FindTopScoreIndeces (TArray<int32> Scores, int32& MaxScore);

	UFUNCTION(BlueprintCallable, Category="Physics")
	static UPARAM(DisplayName="LandingLocation") FVector CalculateItemLandingLocation (ABaseItem* Item);
};
