// Fill out your copyright notice in the Description page of Project Settings.


#include "SpikesObstacle.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "SideRunnerGameModeBase.h"
#include "SideRunnerPaperCharacter.h"

// Sets default values
ASpikesObstacle::ASpikesObstacle()
{
	// PrimaryActorTick.bCanEverTick = true;
	// Try to create the sprite component
	Sprite = CreateDefaultSubobject<UPaperSpriteComponent>(TEXT("SpikeSprite"));
	Sprite->SetSprite(ConstructorHelpers::FObjectFinder<UPaperSprite>(TEXT("/Game/Textures/Sprites/spikes_Sprite")).Object);
	Sprite->SetCollisionProfileName("OverlapAll");

	RootComponent = Sprite;  
}

// Called every frame
void ASpikesObstacle::NotifyActorBeginOverlap(AActor *OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if ((OtherActor != nullptr) && (OtherActor != this)){
		ASideRunnerPaperCharacter* Character = Cast<ASideRunnerPaperCharacter>(OtherActor);

		if(Character && !Character->bDeath){
			// UE_LOG(LogTemp, Warning, TEXT("I'm alive: %s"), Character->bDeath ? *FName("true").ToString() : *FName("false").ToString());
			Character->OnDeath();
		}

	}
}
