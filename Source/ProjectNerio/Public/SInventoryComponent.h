// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "SItemDataStructs.h"
#include "Components/ActorComponent.h"
#include "SInventoryComponent.generated.h"


UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTNERIO_API USInventoryComponent : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadOnly)
	TMap<FString, FSlotData> InventoryData;

	TArray<FSlotData> WeaponData;
	
	TArray<FSlotData> ArmorData;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	UUserWidget* InventoryWidget;
	
	// Sets default values for this component's properties
	USInventoryComponent();

	void Add(const FItemData& Item, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	void DropItem(const FItemData& Item, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	void ShowInventory();

	UFUNCTION(BlueprintCallable)
	void HideInventory();
};
