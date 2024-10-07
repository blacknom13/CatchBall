// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "UObject/Interface.h"
#include "AI_Interface.generated.h"

// This class does not need to be modified.
UINTERFACE()
class UAI_Interface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class TESTPROJECT_API IAI_Interface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AI")
	UPARAM(DisplayName="AIName") FName GetAI_Name();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AI")
	UPARAM(DisplayName="AIScore") int32 GetAI_Score();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="AI")
	UPARAM(DisplayName="NamePlate") UWidgetComponent* GetAI_NamePlateWidget();
};
