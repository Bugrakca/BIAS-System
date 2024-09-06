// Fill out your copyright notice in the Description page of Project Settings.


#include "SCharacter.h"

#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "SInputConfigData.h"
#include "SInteractionComponent.h"
#include "SInventoryComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"


// Sets default values
ASCharacter::ASCharacter()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpringArmComp = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArmComp"));
	SpringArmComp->SetupAttachment(RootComponent);

	CameraComp = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComp"));
	CameraComp->SetupAttachment(SpringArmComp);

	InteractionComp = CreateDefaultSubobject<USInteractionComponent>(TEXT("InteractionComp"));

	InventoryComp = CreateDefaultSubobject<USInventoryComponent>(TEXT("InventoryComp"));
}

// Called when the game starts or when spawned
void ASCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	
}

// Called to bind functionality to input

void ASCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	// Get character enhanced input component
	APlayerController* PlayerController = GetController<APlayerController>();
	
	UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer());
	check(Subsystem);

	Subsystem->ClearAllMappings();
	Subsystem->AddMappingContext(DefaultContext, 0);

	UEnhancedInputComponent* EnhancedInputComp = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	
	EnhancedInputComp->BindAction(InputConfigData->IA_Move, ETriggerEvent::Triggered, this, &ASCharacter::Move);
	EnhancedInputComp->BindAction(InputConfigData->IA_Look, ETriggerEvent::Triggered, this, &ASCharacter::Look);
	EnhancedInputComp->BindAction(InputConfigData->IA_Interaction, ETriggerEvent::Started, this, &ASCharacter::PrimaryInteract);
}

void ASCharacter::Move(const FInputActionValue& InputActionValue)
{
	FVector2D MovementVector = InputActionValue.Get<FVector2D>();
	const FRotator MovementRotation(0.0f, GetControlRotation().Yaw, 0.0f);

	if (MovementVector.X != 0.0f)
	{
		FVector MovementDirection = MovementRotation.RotateVector(FVector::RightVector);
		AddMovementInput(MovementDirection, MovementVector.X);
	}

	if (MovementVector.Y != 0.0f)
	{
		FVector MovementDirection = MovementRotation.RotateVector(FVector::ForwardVector);
		AddMovementInput(MovementDirection, MovementVector.Y);
	}

	/* Direction of the camera. ControlRot is the camera
	 * FRotator ControlRot = GetControlRotation();
	 * ControlRot.Pitch = 0.0f;
	 * ControlRot.Roll = 0.0f;
	 *
	 * FVector RightVector = FRotationMatrix(ControlRot).GetScaledAxis(EAxis::Y);
	 */
}

void ASCharacter::Look(const FInputActionValue& InputActionValue)
{
	FVector2D RotationValue = InputActionValue.Get<FVector2D>();

	AddControllerYawInput(RotationValue.X);
	AddControllerPitchInput(RotationValue.Y);
}

void ASCharacter::PrimaryInteract()
{
	if (InteractionComp)
	{
		InteractionComp->PrimaryInteract();
	}
}
