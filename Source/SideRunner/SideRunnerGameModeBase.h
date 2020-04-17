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

	UPROPERTY()
	class USideRunnerGameInstance *Ref_GameInstance;
	
	UPROPERTY()
	class USideRunnerSaveGame *Ref_SaveGame;

	void BeginPlay() override;

public:
	UPROPERTY(BlueprintReadOnly, Category = Score)
	int Score;

	UPROPERTY(BlueprintReadOnly, Category = Score)
	int HighScore;

	ASideRunnerGameModeBase();

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void GameOver();

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void GetSaveGame();

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void UpdateHighScore();

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void UpdateScore();
};
