// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SpikesObstacle.generated.h"

UCLASS()
class SIDERUNNER_API ASpikesObstacle : public AActor
{
	GENERATED_BODY()

	/** The main skeletal mesh associated with this Character (optional sub-object). */
	UPROPERTY(Category = Character, VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	class UPaperSpriteComponent *Sprite;

public:
	// Sets default values for this actor's properties
	ASpikesObstacle();

	virtual void NotifyActorBeginOverlap(AActor *OtherActor) override;
};
