// Fill out your copyright notice in the Description page of Project Settings.


#include "SideRunnerPaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraTypes.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "Engine/EngineTypes.h" 	
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/MovementComponent.h"
#include "GameFramework/PawnMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "PaperFlipbookComponent.h"
#include "SideRunnerGameModeBase.h"


DEFINE_LOG_CATEGORY_STATIC(SideScrollerCharacter, Log, All);

ASideRunnerPaperCharacter::ASideRunnerPaperCharacter(){
    bUseControllerRotationPitch = false;
    bUseControllerRotationYaw = true;
    bUseControllerRotationRoll = false;

    GetCapsuleComponent()->SetCapsuleHalfHeight(35.f);
    GetCapsuleComponent()->SetCapsuleRadius(35.f);

    CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
    CameraBoom->SetupAttachment(RootComponent);
    CameraBoom->TargetArmLength = 500.f;
    CameraBoom->SocketOffset = FVector(0.f, 0.f, 75.f);
    CameraBoom->bDoCollisionTest = false;
    CameraBoom->SetRelativeRotation(FRotator(0.f, -90.f, 0.f));

    SideViewCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("SideViewCamera"));
    SideViewCameraComponent->ProjectionMode = ECameraProjectionMode::Orthographic;
    SideViewCameraComponent->OrthoWidth = 700.f;
    SideViewCameraComponent->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
    
    CameraBoom->SetUsingAbsoluteRotation(true);
    SideViewCameraComponent->bUsePawnControlRotation = false;
    SideViewCameraComponent->bAutoActivate = true;
    GetCharacterMovement()->bOrientRotationToMovement = false;

    GetCharacterMovement()->bConstrainToPlane = true;
    GetCharacterMovement()->SetPlaneConstraintNormal(FVector(0.f, -1.f, 0.f));
	GetCharacterMovement()->SetPlaneConstraintAxisSetting(EPlaneConstraintAxisSetting::X);

    GetSprite()->SetIsReplicated(true);
    bReplicates = true;
}


void ASideRunnerPaperCharacter::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);

    // TODO: implements Rotation Timeline here
    // GetSprite()->SetRelativeRotation(); // Rotation here
}

void ASideRunnerPaperCharacter::BeginPlay(){
    Super::BeginPlay();

    Ref_GameMode = Cast<ASideRunnerGameModeBase>(GetWorld()->GetAuthGameMode());
}


void ASideRunnerPaperCharacter::Move(){
    if(GetCharacterMovement()->IsMovingOnGround()){
        Jump();
        bDoubleJump = true;
    } else if(bDoubleJump) {
        LaunchCharacter(FVector(0.f, 0.f, JumpVelocity), false, true);
        bDoubleJump = false;
    }
}


void ASideRunnerPaperCharacter::OnDeath(){
    if(!bDeath){
        bDeath = false;
        SetActorEnableCollision(false);
        LaunchCharacter(FVector(0.f, 0.f, JumpVelocity * 2), true, true);

        // TODO: Here make anotther timeline for OnDeathRotation
        // GetSprite()->SetRelativeRotation(); // Other rotation here

        // TODO: Delay
        Ref_GameMode->GameOver();

        // TODO: Delay
        GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
        GetCharacterMovement()->StopMovementImmediately();
    }
}

void ASideRunnerPaperCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent){
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASideRunnerPaperCharacter::Move);
}