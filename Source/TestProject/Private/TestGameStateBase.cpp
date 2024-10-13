// Fill out your copyright notice in the Description page of Project Settings.


#include "TestGameStateBase.h"

#include "NavigationPath.h"
#include "NavigationSystem.h"
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
	}
	OnGameFinished.Broadcast();
}

void ATestGameStateBase::SetWinner()
{
	TArray<FName> Names;
	TArray<int32> Scores;
	TArray<int32> TopScorers;
	int32 MaxScore;
	GetAI_Info(Names, Scores);
	TopScorers = FindTopScoreIndeces(Scores, MaxScore);
	if (TopScorers.Num() > 1)
	{
		WinnerName = "Draw";
		WinnerScore = -1;
	}
	else
	{
		WinnerName = Names[TopScorers[0]];
		WinnerScore = MaxScore;
	}
}

void ATestGameStateBase::CalculateAI_CurrentPathCosts()
{
	FVector CurrentEtimatedLandingLocation;
	NPC_ToPathCost.Empty();
	if (PlayItem)
	{
		CurrentEtimatedLandingLocation = PlayItem->EstimatedNextLocation;
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

void ATestGameStateBase::ItemLocationChanged()
{
	OnItemLocationChanged.Broadcast(PlayItem);
	CalculateAI_CurrentPathCosts();
	FindCurrentMaxPathCost();
}

void ATestGameStateBase::SetItemInGame(ABaseItem* Item)
{
	PlayItem = Item;
	if (PlayItem)
	{
		CalculateAI_CurrentPathCosts();
		FindCurrentMaxPathCost();
	}
	OnItemInGame.Broadcast(PlayItem);
	OnItemLocationChanged.Broadcast(PlayItem);
}

void ATestGameStateBase::UpdateRemainingTime(const float Time)
{
	RemainingTime = Time;
}

void ATestGameStateBase::AddAI_Character(AAIController* Controller)
{
	NPCs.AddUnique(Cast<ABaseAICharacter>(Controller->GetPawn()));
}

void ATestGameStateBase::ResetAI_NamePool(TMap<FName, EGender> NamePool)
{
	CurrentNamePool = NamePool;
}

void ATestGameStateBase::SetGameFinished(bool bFinished)
{
	bGameFinshed = bFinished;
	OnRep_GameFinished();
}

void ATestGameStateBase::GetAI_InitInfo(FName& Name, EGender& Gender)
{
	int32 Index = FMath::RandRange(0, CurrentNamePool.Num() - 1);
	TArray<FName> Keys;
	CurrentNamePool.GetKeys(Keys);
	Name = Keys[Index];
	CurrentNamePool.RemoveAndCopyValue(Name, Gender);
}

TArray<int32> ATestGameStateBase::FindTopScoreIndeces(TArray<int32> Scores, int32& MaxScore)
{
	TArray<int32> TopScorers;
	MaxScore = 0;
	for (int i = 0; i < Scores.Num(); i++)
	{
		if (MaxScore == Scores[i])
		{
			TopScorers.Add(i);
		}
		else if (MaxScore < Scores[i])
		{
			TopScorers.Empty();
			MaxScore = Scores[i];
			TopScorers.Add(i);
		}
	}
	return TopScorers;
}

void ATestGameStateBase::GetAI_Info(TArray<FName>& Names, TArray<int32>& Scores)
{
	Names.Empty();
	Scores.Empty();
	for (auto NPC : NPCs)
	{
		Names.Add(NPC->Name);
		Scores.Add(NPC->Score);
	}
}
