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

	// Sets default values for this component's properties
	USInventoryComponent();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void DropItem(const FItemData& Item, int32 Quantity);

	UFUNCTION(BlueprintCallable)
	void ShowInventory();

	UFUNCTION(BlueprintCallable)
	void HideInventory();

	void InitList(TArray<FSlotData>& Array, int32 Size);

public:
	UPROPERTY(BlueprintReadOnly)
	TArray<FSlotData> InventoryArray;

	TArray<FSlotData> WeaponData;

	TArray<FSlotData> ArmorData;

	FText Weapon;

	FText Armor;

	int32 IndexCount;

	UPROPERTY(BlueprintAssignable)
	FOnSlotUpdate OnSlotUpdate;

	UPROPERTY(BlueprintReadOnly)
	UUserWidget* InventoryWidget;

	UFUNCTION(BlueprintCallable)
	bool IsIndexEmpty(int32 Index);

	UFUNCTION(BlueprintCallable)
	FSlotData& GetItemAtIndex(int32 Index);

	void Add(const FItemData& Item, int32 Quantity);

	int32 CreateNewItem(const FItemData& Item, int32 Quantity);

	TArray<FSlotData> FindSimilarItems(TArray<FSlotData>& ItemArray, const FItemData& Item);
};
