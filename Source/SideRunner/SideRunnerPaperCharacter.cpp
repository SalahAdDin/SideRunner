// Fill out your copyright notice in the Description page of Project Settings.


#include "SideRunnerPaperCharacter.h"
#include "Camera/CameraComponent.h"
#include "Camera/CameraTypes.h"
#include "Components/InputComponent.h"
#include "GameFramework/SpringArmComponent.h"
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

    GetSprite()->SetIsReplicated(true);
    bReplicates = true;
}


void ASideRunnerPaperCharacter::Tick(float DeltaSeconds){
    Super::Tick(DeltaSeconds);

    // TODO: implements Rotation Timeline here
    GetSprite()->SetRelativeRotation(); // Rotation here
}

void ASideRunnerPaperCharacter::BeginPlay(){
    Super::BeginPlay();

    Ref_GameMode = Cast<ASideRunnerGameModeBase>(GetGameMode());
}


void ASideRunnerPaperCharacter::Move(){
    if(GetMovementComponent()->IsMovingOnGround()){
        Jump();
        bDoubleJump = true;
    } else if(bDoubleJump) {
        LaunchCharacter(FVector(0.f, 0.f, JumpVelocity), false, true);
        bDoubleJump = false;
    }
}


void ASideRunnerPaperCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent){
    PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ASideRunnerPaperCharacter::Move);
}