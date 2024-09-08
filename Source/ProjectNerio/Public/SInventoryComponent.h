﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "SItemDataStructs.h"
#include "SInventoryComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnSlotUpdate, int32, Index);

UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class PROJECTNERIO_API USInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	TSubclassOf<UUserWidget> WidgetClass;

	UPROPERTY(BlueprintReadOnly)
	int32 MaxStackSize;

	UPROPERTY()
	APlayerController* PlayerController;

	UPROPERTY()
	bool bIsMouseVisible;
	
	// Sets default values for this component's properties
	USInventoryComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void DropItem(const FItemData& Item, int32 Quantity);

	void InitList(TArray<FSlotData>& Array, const int32 Size);
	
	int32 FindFirstAvailableSlot() const;

	int32 FindNextStackableItem(const FItemData& StackableItem);

	UFUNCTION(BlueprintCallable)
	void ToggleInventory();

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FSlotData> InventoryArray;

	UPROPERTY()
	TArray<FSlotData> WeaponData;

	UPROPERTY()
	TArray<FSlotData> ArmorData;

	UPROPERTY()
	FText Weapon;

	UPROPERTY()
	FText Armor;
	
	UPROPERTY(BlueprintAssignable)
	FOnSlotUpdate OnSlotUpdate;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* InventoryWidget;

	UFUNCTION(BlueprintCallable)
	bool IsIndexEmpty(const int32 Index);

	UFUNCTION(BlueprintCallable)
	FSlotData& GetItemAtIndex(const int32 Index);

	void Add(const FItemData& Item, const int32 Quantity);

	int32 CreateNewItem(const FItemData& Item, int32 Quantity);
	
	TArray<FSlotData> FindSimilarItems(TArray<FSlotData>& ItemArray, const FItemData& Item);
};
