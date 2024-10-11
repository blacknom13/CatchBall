// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BaseAICharacter.h"
#include "BaseItem.h"
#include "Enums/Gender.h"
#include "GameFramework/GameStateBase.h"
#include "Runtime/AIModule/Classes/AIController.h"
#include "TestGameStateBase.generated.h"

/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FNoParam);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneActorParam, ABaseItem*, Item);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOneVectorParam, FVector, Location);

UCLASS()
class TESTPROJECT_API ATestGameStateBase : public AGameStateBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="PlayItem")
	ABaseItem* PlayItem;

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
	
	UFUNCTION(BlueprintCallable)
	void CalculateAI_CurrentPathCosts();

	UFUNCTION(BlueprintCallable)
	void FindCurrentMaxPathCost();

	UFUNCTION(BlueprintCallable)
	void ItemLocationChanged();

	UFUNCTION(BlueprintCallable)
	void SetItemInGame(ABaseItem* Item);

	UFUNCTION(BlueprintCallable)
	void UpdateRemainingTime(const float Time);

	UFUNCTION(BlueprintCallable)
	void AddAI_Character(AAIController* Controller);

	UFUNCTION(BlueprintCallable)
	void ResetAI_NamePool(TMap<FName, EGender> NamePool);

	UFUNCTION(BlueprintCallable)
	void SetGameFinished(bool bFinished);

	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName="InGame") bool IsItemInGame() { return IsValid(PlayItem); }

	UFUNCTION(BlueprintCallable)
	void GetWinnerInfo(FName& Name, int32& Score)
	{
		Name = WinnerName;
		Score = WinnerScore;
	}

	UFUNCTION(BlueprintCallable)
	void GetAI_InitInfo(FName& Name, EGender& Gender);

	UFUNCTION(BlueprintCallable)
	void GetCurrentPathMaxCost(TMap<APawn*, float>& AI_CharacterToCost, float& MaxCost)
	{
		AI_CharacterToCost = NPC_ToPathCost;
		MaxCost = MaxPathCost;
	}

	UFUNCTION(BlueprintCallable)
	UPARAM(DisplayName="AI_Characters") TArray<ABaseAICharacter*> GetAI_Characters()
	{
		return NPCs;
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UPARAM(DisplayName="RemainingTime") const float& GetRemainingTime(){return RemainingTime;}

	//Delegates
	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="GameState")
	FNoParam OnGameFinished;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="GameState")
	FOneActorParam OnItemInGame;

	UPROPERTY(BlueprintCallable, BlueprintAssignable, Category="GameState")
	FOneVectorParam OnItemLocationChanged;

	UFUNCTION(BlueprintCallable)
	void GetAI_Info(TArray<FName>& Names, TArray<int32>& Scores);

private:
	UPROPERTY(BlueprintGetter=GetRemainingTime, BlueprintSetter=UpdateRemainingTime, Replicated, Category="Game")
	float RemainingTime;
};
