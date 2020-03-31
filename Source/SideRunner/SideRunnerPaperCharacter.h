// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/TimelineComponent.h"
#include "PaperCharacter.h"
#include "SideRunnerPaperCharacter.generated.h"

/**
 * 
 */
UCLASS()
class SIDERUNNER_API ASideRunnerPaperCharacter : public APaperCharacter
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	class UCameraComponent* SideViewCameraComponent;
	FTimeline OnDeathRotation;

	FTimeline OnPlayRotation;

	class UCurveFloat *DeathRotation;

	class UCurveFloat *PlayRotation;

	bool bDoubleJump;

	class ASideRunnerGameModeBase* Ref_GameMode; 

	class UTextRenderComponent* TextComponent;

	void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

protected:
	UPROPERTY(EditInstanceOnly, BlueprintReadWrite, Category=Behavior)
	bool bDeath;

	UPROPERTY(EditInstanceOnly, Category=Movement)
	float JumpVelocity = 500.f;

	void Move();

	UFUNCTION(BlueprintCallable, Category=Behavior)
	void OnDeath();

	UFUNCTION()
    void RotatePlayer();

	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

public:
	ASideRunnerPaperCharacter();

	FORCEINLINE class UCameraComponent* GetSideViewCameraComponent() const { return SideViewCameraComponent; }
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	
};
