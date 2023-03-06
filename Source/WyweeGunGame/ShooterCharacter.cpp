// Fill out your copyright notice in the Description page of Project Settings.


#include "ShooterCharacter.h"
#include "EnhancedInputSubsystems.h"
#include "EnhancedInputComponent.h"
#include "Components/InputComponent.h"
#include "InputAction.h"

// Sets default values
AShooterCharacter::AShooterCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AShooterCharacter::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AShooterCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AShooterCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	// Get the player controller
	const APlayerController* PlayerController = Cast<APlayerController>(GetController());

	//Get the local player subsystem
	 UEnhancedInputLocalPlayerSubsystem* Subsystem =
		ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());

	// Clear out existing mapping, and add our mapping
	 if (Subsystem)
	 {
		 Subsystem->ClearAllMappings();
		 Subsystem->AddMappingContext(PlayerMappingContext, 0);
	 }
	
	 // Get the EnhancedInputComponent
	UEnhancedInputComponent* NewInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	
	//Bind the actions
	NewInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Move);
	NewInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Look);
	NewInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &AShooterCharacter::Jump);
	
}

void AShooterCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D DirectionValue = Value.Get<FVector2D>();

	if (Controller)
	{
		/*
		FVector Forward = GetActorForwardVector();
		FVector Right = GetActorRightVector();
		AddMovementInput(Forward, DirectionValue.Y);
		AddMovementInput(Right, DirectionValue.X);
		*/

		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0.f, Rotation.Yaw, 0.f);

		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(ForwardDirection, DirectionValue.Y);
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		AddMovementInput(RightDirection, DirectionValue.X);
	}

}

void AShooterCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisValue = Value.Get<FVector2D>();

	if (Controller)
	{
		AddControllerYawInput(LookAxisValue.X);
		AddControllerPitchInput(LookAxisValue.Y);
	}

}

void AShooterCharacter::JumpFunction(const FInputActionValue& Value)
{
	const bool CurrentValue = Value.Get<bool>();

	if (Controller && (CurrentValue != 0.f))
	{
		Jump();
	}
	else
	{
		StopJumping();
	}
}
