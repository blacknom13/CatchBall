// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/Character.h"
#include "BaseAICharacter.generated.h"

class ABaseItem;

UCLASS()
class TESTPROJECT_API ABaseAICharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ABaseAICharacter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_Name)
	FName Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated)
	int32 Score;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UWidgetComponent* NamePlateC;

	UFUNCTION(BlueprintCallable)
	void PickUp(ABaseItem* Item);

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable)
	void OnRep_Name();
	void OnRep_Name_Implementation(){}
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
