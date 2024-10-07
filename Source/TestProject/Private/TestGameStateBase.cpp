// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameStateBase.h"

#include "ItemInterface.h"
#include "NavigationPath.h"
#include "NavigationSystem.h"
#include "SimpleFunctionLibrary.h"
#include "GameFramework/PawnMovementComponent.h"
#include "Net/UnrealNetwork.h"

void ATestGameStateBase::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(ATestGameStateBase, RemainingTime);
	DOREPLIFETIME(ATestGameStateBase, NPCs);
	DOREPLIFETIME(ATestGameStateBase, bGameFinshed);
	DOREPLIFETIME(ATestGameStateBase, WinnerName);
	DOREPLIFETIME(ATestGameStateBase, WinnerScore);
}

void ATestGameStateBase::OnRep_GameFinished()
{
	if (HasAuthority())
	{
		SetWinner();
		DestroyAllSessions();
	}
	OnGameFinished.Broadcast();
}

void ATestGameStateBase::SetWinner()
{
	TArray<FName> Names;
	TArray<int32> Scores;
	TArray<int32> TopScorers;
	int32 MaxScore;
	Execute_GetAI_Info(this, Names, Scores);
	TopScorers = USimpleFunctionLibrary::FindTopScoreIndeces(Scores, MaxScore);
	if (TopScorers.Num() > 1)
	{
		WinnerName = "Draw";
		WinnerScore = -1;
	}
	else
	{
		WinnerName = Names[0];
		WinnerScore = MaxScore;
	}
}

void ATestGameStateBase::CalculateAI_CurrentPathCosts()
{
	FVector CurrentEtimatedLandingLocation;
	NPC_ToPathCost.Empty();
	if (PlayItem)
	{
		if (PlayItem->Implements<UItemInterface>())
		{
			CurrentEtimatedLandingLocation = IItemInterface::Execute_GetEstimatedLandingLocation(PlayItem);
		}
		for (auto NPC : NPCs)
		{
			auto NavPath = UNavigationSystemV1::FindPathToLocationSynchronously(this, NPC->GetActorLocation(),
																				CurrentEtimatedLandingLocation, NPC);
			float PathTime = NavPath->GetPathLength() / NPC->GetMovementComponent()->GetMaxSpeed();
			NPC_ToPathCost.Add(NPC, PathTime);
		}
	}
}

void ATestGameStateBase::FindCurrentMaxPathCost()
{
	float MaxCost = -1;
	int32 NumOfMaxCostIndeces = 0;

	for (auto Path : NPC_ToPathCost)
	{
		if (FMath::IsNearlyEqual(Path.Value, MaxCost))
		{
			NumOfMaxCostIndeces++;
		}
		else if (Path.Value > MaxCost)
		{
			MaxCost = Path.Value;
			NumOfMaxCostIndeces = 1;
		}
	}
	MaxPathCost = NPCs.Num() != NumOfMaxCostIndeces ? MaxCost : MaxCost + 20;
}

void ATestGameStateBase::GetAI_Info_Implementation(TArray<FName>& Names, TArray<int32>& Scores)
{
	IGameStateInterface::GetAI_Info_Implementation(Names, Scores);
}
