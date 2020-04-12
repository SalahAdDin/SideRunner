// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "SideRunnerGameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Components/SlateWrapperTypes.h"
#include "Kismet/GameplayStatics.h"
#include "SideRunnerGameInstance.h"
#include "SideRunnerSaveGame.h"
#include "SideRunnerPaperCharacter.h"

ASideRunnerGameModeBase::ASideRunnerGameModeBase()
{
    static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/Blueprints/BP_SideRunnerPlayerCharacter"));

	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
    // DefaultPawnClass = ASideRunnerPaperCharacter::StaticClass();
}

void ASideRunnerGameModeBase::BeginPlay(){

    UWorld *world = GetWorld();

    if (world)
    {
        Ref_GameInstance = world->GetGameInstance<USideRunnerGameInstance>();
    }
    
    GetSaveGame();

    UGameplayStatics::SetGamePaused(GetWorld(), false);
    
    if(Ref_GameInstance){
        Ref_GameInstance->CallHUDWBP(); 
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("There is not a reference for game instance here."));
    }

}

void ASideRunnerGameModeBase::GetSaveGame(){

    if (UGameplayStatics::DoesSaveGameExist(TEXT("Default"), 0)){
        Ref_SaveGame = Cast<USideRunnerSaveGame>(UGameplayStatics::LoadGameFromSlot(TEXT("Default"), 0));
        if(Ref_SaveGame) HighScore = Ref_SaveGame->HighScore;   
    } else if(USideRunnerSaveGame* SaveGameInstance = Cast<USideRunnerSaveGame>(UGameplayStatics::CreateSaveGameObject(USideRunnerSaveGame::StaticClass()))){
        Ref_SaveGame = SaveGameInstance;
        if(Ref_SaveGame) {
            Ref_SaveGame->HighScore = 0;
            HighScore = Ref_SaveGame->HighScore;
            UGameplayStatics::SaveGameToSlot(Ref_SaveGame, TEXT("Default"), 0);
        }
    }

    Score = 0;

}

void ASideRunnerGameModeBase::GameOver(){
    UpdateHighScore();

    UGameplayStatics::SetGamePaused(GetWorld(), true);

    if(Ref_GameInstance){
        Ref_GameInstance->Ref_HUD->SetVisibility(ESlateVisibility::Hidden);
        Ref_GameInstance->CallGameOverWBP();
    }
    else {
        UE_LOG(LogTemp, Warning, TEXT("There is not a reference for game instance here."));
    }
}

void ASideRunnerGameModeBase::UpdateHighScore(){
    if (Score > HighScore){
        HighScore = Score;
        if(Ref_SaveGame) Ref_SaveGame->HighScore = HighScore;
        UGameplayStatics::SaveGameToSlot(Ref_SaveGame, TEXT("Default"), 0);
        UE_LOG(LogTemp, Warning, TEXT("HighScore: %d"), HighScore);
    }
}

void ASideRunnerGameModeBase::UpdateScore(){
    Score++;
    UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}