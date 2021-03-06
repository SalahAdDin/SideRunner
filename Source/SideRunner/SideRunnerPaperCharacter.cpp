// Fill out your copyright notice in the Description page of Project Settings.

#include "SideRunnerPaperCharacter.h"
// #include "Camera/CameraComponent.h"
// #include "Camera/CameraTypes.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Curves/CurveFloat.h"
#include "Engine/EngineTypes.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "SideRunnerGameModeBase.h"
#include "UObject/ConstructorHelpers.h"

DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

ASideRunnerPaperCharacter::ASideRunnerPaperCharacter()
{
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    GetCapsuleComponent()->SetCapsuleHalfHeight(35.f);
    GetCapsuleComponent()->SetCapsuleRadius(35.f);

    // CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    // CameraBoom->SetupAttachment(RootComponent);
    // CameraBoom->TargetArmLength = 500.f;
    // CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
    // CameraBoom->bDoCollisionTest = false;
    // CameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    // SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    // SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    // SideViewCameraComponent->OrthoWidth = 700.f;
    // SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);

    // CameraBoom->SetUsingAbsoluteRotation(true);
    // SideViewCameraComponent->bUsePawnControlRotation = false;
    // SideViewCameraComponent->bAutoActivate = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;

    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, -1.f, 0.f));
    GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);

    GetSprite()->SetIsReplicated(true);
    bReplicates = true;

    /* This is the way using a Blueprint Curve, 
    but the best way is leaving the designer to design and choose the curves using the curve editor.
    */
    // static ConstructorHelpers::FObjectFinder<UCurveFloat> PlayCurve(TEXT("/Game/Blueprints/Utils/Curve_PlayRotation_Float"));
    // check(PlayCurve.Succeeded());
    // PlayRotation = PlayCurve.Object;

    // static ConstructorHelpers::FObjectFinder<UCurveFloat> DeathCurve(TEXT("/Game/Blueprints/Utils/Curve_DeathRotation_Float"));
    // check(DeathCurve.Succeeded());
    // DeathRotation = DeathCurve.Object;

    /* This is the way creating the curve with code, it does not work for me 
    PlayRotation = NewObject<UCurveFloat>();
	PlayRotation->FloatCurve.AddKey(0.0f, 0.0f);
	PlayRotation->FloatCurve.AddKey(1.0f, -360.0f);

    DeathRotation = NewObject<UCurveFloat>();
    DeathRotation->FloatCurve.AddKey(0.f, 0.f);
    DeathRotation->FloatCurve.AddKey(1.f, -512.f);
    */
}

void ASideRunnerPaperCharacter::RotatePlayer()
{
	float TimelineValue;
	float RotationFloatValue;

	/* As whatever the status of the character (dead ot not) the rotation will be the same workflow,
		and as we just change the Curve values, we handle both rotation behaviour in the same function
		based on the character's status
	*/
	if (bDeath)
	{
		TimelineValue = OnDeathRotation.GetPlaybackPosition();
		if (DeathRotation)
		{
			RotationFloatValue = DeathRotation->GetFloatValue(TimelineValue);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("There is no such DeathRotation curve."));
		}
	} else
	{
		TimelineValue = OnPlayRotation.GetPlaybackPosition();

		if (PlayRotation)
		{
			RotationFloatValue = PlayRotation->GetFloatValue(TimelineValue);
		}
		else
		{
			UE_LOG(LogTemp, Warning, TEXT("There is no such PlayRotation curve."));
		}
	}

	GetSprite()->SetRelativeRotation(FRotator(RotationFloatValue, 0.f, 0.f));
}

void ASideRunnerPaperCharacter::Tick(float DeltaSeconds)
{
    Super::Tick(DeltaSeconds);

    if (OnPlayRotation.IsPlaying())
        OnPlayRotation.TickTimeline(DeltaSeconds);

	if (OnDeathRotation.IsPlaying()) 
		OnDeathRotation.TickTimeline(DeltaSeconds);
}

void ASideRunnerPaperCharacter::BeginPlay()
{
	// TODO: DeathRotation is being destriyed here
    Super::BeginPlay();

    UWorld *world = GetWorld();

    if (world)
    {
        Ref_GameMode = world->GetAuthGameMode<ASideRunnerGameModeBase>();
    }

    FOnTimelineFloat TimelineCallback;
    FOnTimelineEventStatic TimelineFinishedCallback;

    TimelineCallback.BindUFunction(this, "RotatePlayer");

    if (PlayRotation)
    {
        OnPlayRotation.AddInterpFloat(PlayRotation, TimelineCallback, TEXT("Player Rotation"));
        OnPlayRotation.SetLooping(true);
        OnPlayRotation.SetTimelineLength(1.f);
        OnPlayRotation.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
    }
    else
    {
        UE_LOG(LogTemp, Warning, TEXT("There is no Curvefloat Object here!"));
    }

	if (DeathRotation)
	{
		OnDeathRotation.AddInterpFloat(DeathRotation, TimelineCallback, TEXT("Death Rotation"));
		OnDeathRotation.SetLooping(true);
		OnDeathRotation.SetTimelineLength(1.f);
		OnDeathRotation.SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("There is no DeathRotation curve here!"));
	}

    OnPlayRotation.Play();
}

void ASideRunnerPaperCharacter::Move()
{
    if (GetCharacterMovement()->IsMovingOnGround())
    {
        Jump();
        bDoubleJump = true;
    }
    else if (bDoubleJump)
    {
        LaunchCharacter(FVector(0.f, 0.f, JumpVelocity), false, true);
        bDoubleJump = false;
    }
}

void ASideRunnerPaperCharacter::OnDeath()
{
    if (!bDeath)
    {
        bDeath = true;
        SetActorEnableCollision(false);
        LaunchCharacter(FVector(0.f, 0.f, JumpVelocity * 2), true, true);

        // TODO: Here make another timeline for OnDeathRotation
		OnPlayRotation.Stop();
		OnDeathRotation.Play();

        // This does not work: GetWorld()->GetTimerManager().SetTimer(DelayHandler, 0.2f, false);
        GetWorld()->GetTimerManager().SetTimer(ShowGameOverScreenDelayHandler, this, &ASideRunnerPaperCharacter::ShowGameOverScreen, 2.5f);

        GetWorld()->GetTimerManager().SetTimer(StopMovementDelayHandler, this, &ASideRunnerPaperCharacter::StopMovement, 4.f);
    }
}

void ASideRunnerPaperCharacter::SetupPlayerInputComponent(class UInputComponent *PlayerInputComponent)
{
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASideRunnerPaperCharacter::Move);
}

// TODO:  What's about this
void ASideRunnerPaperCharacter::StopMovement()
{
    UE_LOG(LogTemp, Warning, TEXT("Character's timer expired!"));
    GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
    GetCharacterMovement()->StopMovementImmediately();
}

void ASideRunnerPaperCharacter::ShowGameOverScreen()
{
    if (Ref_GameMode)
    {
        Ref_GameMode->GameOver();
    }
}