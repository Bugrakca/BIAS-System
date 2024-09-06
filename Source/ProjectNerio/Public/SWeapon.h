// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SGameplayInterface.h"
#include "SItemDataStructs.h"
#include "GameFramework/Actor.h"
#include "SWeapon.generated.h"

class USphereComponent;

UCLASS()
class PROJECTNERIO_API ASWeapon : public AActor, public ISGameplayInterface
{
	GENERATED_BODY()

protected:
	UPROPERTY(VisibleAnywhere)
	USphereComponent* SphereComp;

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* MeshComp;

	UPROPERTY(EditDefaultsOnly, Category = "Item Data")
	FItemData ItemData;
	
public:
	// Sets default values for this actor's properties
	ASWeapon();

	virtual void Interact_Implementation(APawn* InstigatorPawn) override;
};
