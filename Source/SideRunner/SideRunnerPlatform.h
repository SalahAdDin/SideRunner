// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SideRunnerPlatform.generated.h"

UCLASS()
class SIDERUNNER_API ASideRunnerPlatform : public AActor
{
	GENERATED_BODY()

	class ASideRunnerGameModeBase* Ref_GameMode;

	/* For further developments: we can choose more than two types os block and add to the platform since the editor */
	UPROPERTY()
	TArray<class UPaperSprite *> BlockSprites;

	UPROPERTY(Category=Actor, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerScore = nullptr;

	UPROPERTY(Category=Actor, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UChildActorComponent* Obstacle = nullptr;

	UPROPERTY(VisibleAnywhere)
	USceneComponent* Root;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category=Platform)
	int32 MinBlocks = 4;
	
	UPROPERTY(EditDefaultsOnly, Category=Platform)
	int32 MaxBlocks = 8;

	UPROPERTY(EditDefaultsOnly, Category=Platform, meta=(ToolTip="It will be get as a negative value"))
	float WorldOffset = 500.f;


public:	
	// Sets default values for this actor's properties
	ASideRunnerPlatform();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); 

	UPROPERTY(BlueprintReadOnly, Category=Platform)
	int TilesAmount;

	UPROPERTY(BlueprintReadOnly, Category=Platform, Meta = (ExposeOnSpawn = true))
	bool bInitialPlatform;

	UPROPERTY(BlueprintReadWrite, Category=Platform)
	bool bSpawnNext;

};
