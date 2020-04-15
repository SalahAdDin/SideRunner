// Fill out your copyright notice in the Description page of Project Settings.

#include "SideRunnerGameInstance.h"
#include "Blueprint/UserWidget.h"
#include "GameFramework/PlayerController.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

USideRunnerGameInstance::USideRunnerGameInstance()
{
    static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidget(TEXT("/Game/Blueprints/HUD/WBP_HUD"));
    if (HUDWidget.Class != NULL)
        HUDWidgetClass = HUDWidget.Class;

    static ConstructorHelpers::FClassFinder<UUserWidget> GameOverWidget(TEXT("/Game/Blueprints/HUD/WBP_GameOver"));
    if (GameOverWidget.Class != NULL)
        GameOverWidgetClass = GameOverWidget.Class;
}

void USideRunnerGameInstance::CallHUDWBP()
{
    if (Ref_HUD != NULL)
    {
        Ref_HUD->RemoveFromParent();
    }

    Ref_HUD = CreateWidget<UUserWidget>(UGameplayStatics::GetPlayerController(this, 0), HUDWidgetClass);

    Ref_HUD->AddToViewport();
}

void USideRunnerGameInstance::CallGameOverWBP()
{
    if (Ref_GameOver != NULL)
    {
        Ref_GameOver->RemoveFromParent();
    }
    APlayerController *PlayerController = UGameplayStatics::GetPlayerController(this, 0);
    PlayerController->bShowMouseCursor = true;

    Ref_GameOver = CreateWidget<UUserWidget>(PlayerController, GameOverWidgetClass);

    Ref_GameOver->AddToViewport();
}