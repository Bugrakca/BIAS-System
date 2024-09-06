// Fill out your copyright notice in the Description page of Project Settings.

#pragma once
#include "SGameplayInterface.h"
#include "SStorageCrate.generated.h"

class UBoxComponent;
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
	UBoxComponent* BoxCollision;

	UPROPERTY(VisibleAnywhere)
	USStorageComponent* StorageComp;

	UPROPERTY(EditDefaultsOnly)
	int32 StorageSize;

	// UPROPERTY(EditDefaultsOnly, meta = (TitleProperty = "ItemName"), Replicated)
	// TArray<FStorageItemStruct> Content;
	
	void AddToStorage();

	void RemoveFromStorage();
};
