// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SideRunnerGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class SIDERUNNER_API ASideRunnerGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

	int Score;
	int HighScore;

	class USideRunnerSaveGame* Ref_SaveGame; 

	void BeginPlay() override;

public:
	ASideRunnerGameModeBase();

	UFUNCTION(BlueprintCallable, Category=Behavior)
	void GameOver();

	UFUNCTION(BlueprintCallable, Category=Behavior)
	void UpdateHighScore();

	UFUNCTION(BlueprintCallable, Category=Behavior)
	void UpdateScore();
	
};
