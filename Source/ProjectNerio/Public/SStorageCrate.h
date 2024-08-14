// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "GameFramework/Actor.h"
#include "SStorageCrate.generated.h"

class USStorageComponent;

UCLASS()
class PROJECTNERIO_API ASStorageCrate : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASStorageCrate();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
	
protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* BaseMesh;

	UPROPERTY(VisibleAnywhere)
	USStorageComponent* StorageComp;
};
