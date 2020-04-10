// Fill out your copyright notice in the Description page of Project Settings.


#include "SideRunnerSpawner.h"
#include "AI/NavigationSystemBase.h"
#include "Components/BillboardComponent.h"
#include "Components/BoxComponent.h"
#include "Engine/EngineTypes.h"
#include "Engine/Texture2D.h"
#include "Math/UnrealMathUtility.h"
#include "SideRunnerPlatform.h"

// Sets default values
ASideRunnerSpawner::ASideRunnerSpawner()
{
	Billboard = CreateDefaultSubobject<UBillboardComponent>(TEXT("Billboard"));
	Billboard->SetSprite(ConstructorHelpers::FObjectFinder<UTexture2D>(TEXT("/Engine/EditorMaterials/TargetIcon")).Object);

	RootComponent = Billboard;

	TriggerBox = CreateDefaultSubobject<UBoxComponent>(TEXT("TriggerBox"));
	TriggerBox->SetupAttachment(RootComponent);
	TriggerBox->SetRelativeLocation(FVector(400.f, 0.f, 0.f));
	TriggerBox->SetWorldScale3D(FVector(1.f, 1.f, 15.f));
	TriggerBox->AreaClass = FNavigationSystem::GetDefaultObstacleArea();

	TriggerBox->OnComponentBeginOverlap.AddDynamic(this, &ASideRunnerSpawner::OnOverlapBegin);

}

// Called when the game starts or when spawned
void ASideRunnerSpawner::BeginPlay()
{
	Super::BeginPlay();

	UWorld* world = GetWorld();

	if (ToSpawn)
		if (world)
		{
			// Set the first platform downside of player
			float NewZ = FMath::RandRange(-150.f, -200.f);
			
			// We use SpawnActorDeferred since we need to set a parameter at spawning the platform
			Ref_Platform = world->SpawnActorDeferred<ASideRunnerPlatform>(
				ToSpawn, 
				FTransform(FRotator::ZeroRotator, FVector(200.f, 15.f, NewZ), FVector::OneVector),
				NULL, 
				NULL, 
				ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding
				);
			Ref_Platform->bInitialPlatform = true;
			check(Ref_Platform);

			//TODO: Delay, It is spawning a two platfom at begin. It is possible it is setting the reference in the time: DELAY!

			SpawnPlatform();
		}
	else
		UE_LOG(LogTemp, Warning, TEXT("It seems you forgot to put the object to spawn(platform)"));
}

void ASideRunnerSpawner::SpawnPlatform(){
	
	if (Ref_Platform){
		FVector PlatformLocation = Ref_Platform->GetActorLocation();

		// 70 x 70 is the size of the spike sprite
		float NewX = (PlatformLocation.X + (Ref_Platform->TilesAmount * 70)) + FMath::RandRange(150.f, 250.f);
		// We want to spawn platforms in a wide range: downside and highside
		float NewZ = FMath::Clamp(PlatformLocation.Z + FMath::RandRange(-150.f, 150.f), -150.f, 150.f);
		
		UWorld* world = GetWorld();

		if (ToSpawn)
			if (world)
			{
				FActorSpawnParameters SpawnParams;
				// For some reason, PlayerStart spawn the player character in (0.f, 15.f, 0.f)
				Ref_Platform = world->SpawnActor<ASideRunnerPlatform>(ToSpawn, FVector(NewX, 15.f, NewZ), FRotator::ZeroRotator, SpawnParams);
			}
		else
			UE_LOG(LogTemp, Warning, TEXT("It seems you forgot to put the object to spawn(platform)"));
	}

}

void ASideRunnerSpawner::OnOverlapBegin(UPrimitiveComponent *OverlappedComp, AActor *OtherActor, UPrimitiveComponent *OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult &SweepResult){
// Other Actor is the actor that triggered the event. Check that is not ourself.
    if ((OtherActor != nullptr) && (OtherActor != this) && (OtherComp != nullptr))
    {
        ASideRunnerPlatform* Platform = Cast<ASideRunnerPlatform>(OtherActor);

		if(Platform){
			if(!Platform->bInitialPlatform && !Platform->bSpawnNext){
				Platform->bSpawnNext = true;
				SpawnPlatform();
			}
		}
	}
}

