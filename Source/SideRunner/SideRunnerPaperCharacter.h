// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TimelineComponent.h"
#include "CoreMinimal.h"
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

	FTimeline OnDeathRotation;
	FTimeline OnPlayRotation;

	FTimerHandle ShowGameOverScreenDelayHandler;
	FTimerHandle StopMovementDelayHandler;

	bool bDoubleJump;

	class ASideRunnerGameModeBase *Ref_GameMode = nullptr;

	class UTextRenderComponent *TextComponent = nullptr;

	void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = Movement)
	float JumpVelocity = 500.f;

	void Move();

	UFUNCTION()
	void RotatePlayer();

	UFUNCTION()
	void StopMovement();

	UFUNCTION()
	void ShowGameOverScreen();

	virtual void SetupPlayerInputComponent(class UInputComponent *InputComponent) override;

public:
	UPROPERTY(BlueprintReadWrite, Category = Behavior)
	bool bDeath;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	class UCurveFloat *DeathRotation = nullptr;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = Animation)
	class UCurveFloat *PlayRotation = nullptr;

	ASideRunnerPaperCharacter();

	UFUNCTION(BlueprintCallable, Category = Behavior)
	void OnDeath();

	// FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	// FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
};
