// Fill out your copyright notice in the Description page of Project Settings.

#include "SideRunnerPlatform.h"
#include "AI/NavigationSystemBase.h"
#include "Components/BoxComponent.h"
#include "Components/ChildActorComponent.h"
#include "Components/PrimitiveComponent.h"
#include "Math/UnrealMathUtility.h"
#include "PaperSprite.h"
#include "PaperSpriteComponent.h"
#include "SideRunnerGameModeBase.h"
#include "SideRunnerPaperCharacter.h"
#include "UObject/ConstructorHelpers.h"

// Sets default values
ASideRunnerPlatform::ASideRunnerPlatform()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Root = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	RootComponent = Root;

	Obstacle = CreateDefaultSubobject<UChildActorComponent>(TEXT("Obstacle"));
	Obstacle->SetupAttachment(RootComponent);
	Obstacle->CreateChildActor();

	TriggerScore = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerScore"));
	TriggerScore->SetupAttachment(RootComponent);
	TriggerScore->SetWorldScale3D(FVector(1.f, 1.f, 15.f));
	TriggerScore->AreaClass = FNavigationSystem::GetDefaultObstacleArea();

	TriggerScore->OnComponentBeginOverlap.AddDynamic(this, &ASideRunnerPlatform::OnOverlapBegin);

	// There is no a way to use FObjectFinder outside to the constructor
	static ConstructorHelpers::FObjectFinder<UPaperSprite> BoxSprite(TEXT("/Game/Textures/Sprites/box_Sprite"));
	check(BoxSprite.Succeeded());
	BlockSprites.Add(BoxSprite.Object);

	static ConstructorHelpers::FObjectFinder<UPaperSprite> BoxAltSprite(TEXT("/Game/Textures/Sprites/boxAlt_Sprite"));
	check(BoxAltSprite.Succeeded());
	BlockSprites.Add(BoxAltSprite.Object);

	InitialLifeSpan = 4.f;
}

// Called when the game starts or when spawned
void ASideRunnerPlatform::BeginPlay()
{
	Super::BeginPlay();

	UWorld *world = GetWorld();

	if (world)
	{
		Ref_GameMode = world->GetAuthGameMode<ASideRunnerGameModeBase>();
		check(Ref_GameMode);
	}

	TilesAmount = bInitialPlatform ? 10 : FMath::RandRange(MinBlocks, MaxBlocks);

	FString BlockName;

	for (int i = 0; i < TilesAmount; i++)
	{
		BlockName = "Block_" + FString::FromInt(i);

		UPaperSpriteComponent *Block = NewObject<UPaperSpriteComponent>(this, UPaperSpriteComponent::StaticClass(), FName(*BlockName));

		if (Block)
		{
			Block->RegisterComponent();
			Block->AttachTo(this->RootComponent);
			Block->SetRelativeLocation(FVector(i * 70.f, 0.0f, 0.f));
			// Block->SetSprite(ConstructorHelpers::FObjectFinder<UPaperSprite> (FMath::RandBool() ? TEXT("/Game/Textures/Sprites/box_Sprite") : TEXT("/Game/Textures/Sprites/boxAlt_Sprite")).Object);
			Block->SetSprite(FMath::RandBool() ? BlockSprites[0] : BlockSprites[1]);
		}
	}

	if (bInitialPlatform)
	{
		if (Obstacle)
		{
			Obstacle->DestroyComponent();
		}
		if (TriggerScore)
		{
			TriggerScore->DestroyComponent();
		}
	}
	else
	{
		float XLocation = FMath::RandRange(0, TilesAmount - 1) * 70;
		if (Obstacle)
			Obstacle->SetRelativeLocation(FVector(XLocation, 0.f, 35.f));
		if (TriggerScore)
			TriggerScore->SetRelativeLocation(FVector(XLocation, 0.f, 0.f));
	}
}

// Called every frame
void ASideRunnerPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	AddActorWorldOffset(FVector(DeltaTime * -WorldOffset, 0.f, 0.f));
}

void ASideRunnerPlatform::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult)
{
	// Other Actor is the actor that triggered the event. Check that is not ourself.
	if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
	{
		ASideRunnerPaperCharacter *Player = Cast<ASideRunnerPaperCharacter>(OtherActor);

		if (Player)
		{
			if (!bInitialPlatform && !Player->bDeath)
			{
				// If we don't check the value editor could be crash since Ref_GameMode could be nullptr some times.
				if (Ref_GameMode)
				{
					Ref_GameMode->UpdateScore();
				}
			}
		}
	}
}
