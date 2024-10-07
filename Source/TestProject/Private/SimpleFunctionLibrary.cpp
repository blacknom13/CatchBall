// Fill out your copyright notice in the Description page of Project Settings.


#include "SimpleFunctionLibrary.h"

#include "ItemInterface.h"

TArray<int32> USimpleFunctionLibrary::FindTopScoreIndeces(TArray<int32> Scores, int32& MaxScore)
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

FVector USimpleFunctionLibrary::CalculateItemLandingLocation(AActor* Item)
{
	// Solve simple mechanical movement problem
	FVector Velocity=Item->GetVelocity();
	FVector VelocityXY=FVector(Velocity.X,Velocity.Y,0);
	float Gravity = -980;
	if (Item->Implements<UItemInterface>())
	{
		Gravity = IItemInterface::Execute_GetItemGravity(Item);
	}

	float A = .5 * Gravity;
	float B = -Velocity.Z;
	float C = Item->GetActorLocation().Z;
	float Delta = B * B - 4 * A * C;

	if (Delta < 0)
	{
		return FVector(0, 0,0);
	}

	float R1 = FMath::Abs((-B + FMath::Sqrt(Delta)) / (2 * A));
	float R2 = FMath::Abs((-B - FMath::Sqrt(Delta)) / (2 * A));

	float Time = Velocity.Z > 0 ? FMath::Max(R1, R2) : FMath::Min(R1, R2);

	FVector LandingLocation= VelocityXY*Time+Item->GetActorLocation();
	return FVector(LandingLocation.X,LandingLocation.Y,0);
}
