// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "GameStateInterface.h"
#include "Enums/Gender.h"
#include "GameFramework/GameStateBase.h"
#include "TestGameStateBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoParam);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneActorParam, AActor*, Actor);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneVectorParam, FVector, Location);

UCLASS()
class TESTPROJECT_API ATestGameStateBase : public AGameStateBase, public IGameStateInterface
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayItem")
	AActor* PlayItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Game")
	float RemainingTime;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Game")
	TArray<ABaseAICharacter*> NPCs;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Game")
	TMap<FName, EGender> CurrentNamePool;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_GameFinished, Category="Game")
	bool bGameFinshed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Game")
	FName WinnerName;	

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Replicated, Category="Game")
	int32 WinnerScore;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayItem")
	TMap<APawn*, float> NPC_ToPathCost;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayItem")
	float MaxPathCost;

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UFUNCTION(BlueprintCallable)
	void OnRep_GameFinished();

	UFUNCTION(BlueprintCallable, Category="EndGame")
	void SetWinner();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category="EndGame")
	void DestroyAllSessions();
	void DestroyAllSessions_Implementation(){}

	UFUNCTION(BlueprintCallable)
	void CalculateAI_CurrentPathCosts();

	UFUNCTION(BlueprintCallable)
	void FindCurrentMaxPathCost();
	//Delegates

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="GameState")
	FNoParam OnGameFinished;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="GameState")
	FOneActorParam OnItemInGame;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="GameState")
	FOneVectorParam OnItemLocationChanged;

	//Interface
	virtual void GetAI_Info_Implementation(TArray<FName>& Names, TArray<int32>& Scores) override;
};
