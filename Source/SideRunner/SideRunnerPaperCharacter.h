// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "Engine/EngineTypes.h"
#include "PaperCharacter.h"
#include "SideRunnerPaperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDERUNNER_API ASideRunnerPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// class UCameraComponent* SideViewCameraComponent;

	// UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	// class USpringArmComponent* CameraBoom;

	// FTimerHandle DelayHandler;

	FTimeline OnDeathRotation;

	FTimeline OnPlayRotation;

	FTimerHandle ShowGameOverScreenDelayHandler;
	FTimerHandle StopMovementDelayHandler;

	class UCurveFloat *DeathRotation;

	class UCurveFloat *PlayRotation;

	bool bDoubleJump;

	class ASideRunnerGameModeBase* Ref_GameMode;

	class UTextRenderComponent* TextComponent;

	void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:

	UPROPERTY(EditDefaultsOnly, Category=Movement)
	float JumpVelocity = 500.f;

	void Move();

	UFUNCTION()
    void RotatePlayer();

	UFUNCTION()
	void StopMovement();

	UFUNCTION()
	void ShowGameOverScreen();

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:

	UPROPERTY(BlueprintReadWrite, Category=Behavior)
	bool bDeath;

	ASideRunnerPaperCharacter();

	UFUNCTION(BlueprintCallable, Category=Behavior)
	void OnDeath();

	// FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	// FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
};
