// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SideRunnerSpawner.generated.h"

UCLASS()
class SIDERUNNER_API ASideRunnerSpawner : public AActor
{
	GENERATED_BODY()

	class ASideRunnerPlatform* Ref_Platform;	
	
	UPROPERTY(Category=Actor, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UBoxComponent* TriggerBox = nullptr;

	UPROPERTY(Category=Actor, VisibleAnywhere, BlueprintReadOnly, meta=(AllowPrivateAccess = "true"))
	class UBillboardComponent* Billboard = nullptr;	

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	/* Since, for now, the platform ask you to put the obstacle(by blueprint),
		we need to assing a the platform's blueprint who has the obstacle defined.
	*/

	UPROPERTY(EditDefaultsOnly, Category=Platform)
	TSubclassOf<class ASideRunnerPlatform> ToSpawn = nullptr;

	// Sets default values for this actor's properties
	ASideRunnerSpawner();

	UFUNCTION(BlueprintCallable, Category=Platform)
	void SpawnPlatform();

	UFUNCTION()
    void OnOverlapBegin(class UPrimitiveComponent* OverlappedComp, class AActor* OtherActor, class UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult); 


};
