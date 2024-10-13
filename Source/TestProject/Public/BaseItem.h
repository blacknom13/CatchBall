// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "DataAssets/ItemDA.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "BaseItem.generated.h"

class ATestGameStateBase;

UCLASS()
class TESTPROJECT_API ABaseItem : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABaseItem();

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UStaticMeshComponent* Item;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	UProjectileMovementComponent* ProjectileMovement;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, ReplicatedUsing=OnRep_ItemDA, meta=(ExposeOnSpawn=true))
	UItemDA* ItemDA;
	
	UPROPERTY(BlueprintGetter=GetEstimatedNextLocation)
	FVector EstimatedNextLocation;

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FVector GetEstimatedNextLocation(){return EstimatedNextLocation;}
	
	UFUNCTION(BlueprintCallable)
	void ResetOnGameState(AActor* Actor);
	
	UFUNCTION(BlueprintCallable)
	void ItemBeginOverlap(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp,
	                        int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	
	UFUNCTION(Blueprintable)
	void ProjectileMovementEnded(const FHitResult& Hit);
	
	UFUNCTION(BlueprintCallable)
	void OnRep_ItemDA();
	
	FVector CalculateNextItemLocation() const;
	
	FVector CalculateItemLandingLocation() const;

	ATestGameStateBase* GameState;

	FHitResult CurrentHit;
	
	void SetItemStaticMesh() const;

	void UpdateItemPosition();
	
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	
	virtual void OnConstruction(const FTransform& Transform) override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
};
