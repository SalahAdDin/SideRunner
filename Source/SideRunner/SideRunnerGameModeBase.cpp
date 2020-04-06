// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.


#include "SideRunnerGameModeBase.h"
#include "Components/SlateWrapperTypes.h"	
#include "Kismet/GameplayStatics.h"
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

}

void ASideRunnerGameModeBase::GameOver(){
    UpdateHighScore();
    UGameplayStatics::SetGamePaused(GetWorld(), true);
    // InventoryWidget->SetVisibility(ESlateVisibility::Hidden);

}

void ASideRunnerGameModeBase::UpdateHighScore(){
    if (Score > HighScore){
        HighScore = Score;
        if(Ref_SaveGame) Ref_SaveGame->HighScore = HighScore;
        UGameplayStatics::SaveGameToSlot(Ref_SaveGame, TEXT("Default"), 0);
    }
}

void ASideRunnerGameModeBase::UpdateScore(){
    Score++;
    UE_LOG(LogTemp, Warning, TEXT("Score: %d"), Score);
}