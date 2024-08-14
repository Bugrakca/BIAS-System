// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SInteractionComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTNERIO_API USInteractionComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties
	USInteractionComponent();

protected:
	UPROPERTY(VisibleAnywhere)
	float InteractionDistance;

	UPROPERTY(VisibleAnywhere)
	float SphereRadius;
	

public:
	void PrimaryInteract();
};
