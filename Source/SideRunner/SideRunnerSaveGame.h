// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SideRunnerSaveGame.generated.h"

/**
 * 
 */
UCLASS()
class SIDERUNNER_API USideRunnerSaveGame : public USaveGame
{
	GENERATED_BODY()

public:

	// UPROPERTY is required to enables UE4 to handle its values
	UPROPERTY()
	int HighScore = 0;
};
