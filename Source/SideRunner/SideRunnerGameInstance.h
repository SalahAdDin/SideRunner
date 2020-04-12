// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "SideRunnerGameInstance.generated.h"

/**
 * 
 */
UCLASS()
class SIDERUNNER_API USideRunnerGameInstance : public UGameInstance
{
	GENERATED_BODY()

	TSubclassOf<class UUserWidget> HUDWidgetClass;
	TSubclassOf<class UUserWidget> GameOverWidgetClass;

	class UUserWidget *Ref_GameOver;

public:
	USideRunnerGameInstance();

	class UUserWidget *Ref_HUD;

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void CallHUDWBP();

	UFUNCTION(BlueprintCallable, Category = "User Interface")
	void CallGameOverWBP();
};
